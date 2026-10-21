"""One generation/download backend for the CLI and TUI."""
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import signal
import selectors
import codecs
import shlex
import subprocess
import tempfile
import urllib.error
import urllib.parse
import urllib.request
import uuid

from .auth import Auth
from .artifacts import INDEX, atomic_json, digest, extract, read_index
from .cleanup import CACHE_MARKER, safe_path
from .discovery import AmbiguousProjectError
from .github import GitHubError, unreachable
from . import tui
from .resources import job_count

SAVED_BUILDS = 5


class GenerationError(RuntimeError):
    pass


def run(command, cwd=None, env=None):
    command = list(map(str, command))
    print('+ ' + shlex.join(command), flush=True)

    # Always relayed: progress events become stage details and timers keep ticking.
    with subprocess.Popen(command, cwd=cwd, env=env, start_new_session=True,
                          stdout=subprocess.PIPE, stderr=subprocess.STDOUT) as process:
        try:
            decoder = codecs.getincrementaldecoder('utf-8')(errors='replace')
            relay = tui.Relay()
            with selectors.DefaultSelector() as selector:
                selector.register(process.stdout, selectors.EVENT_READ)
                while selector.get_map():
                    tui.check_cancel()
                    for event, _ in selector.select(0.1):
                        data = os.read(event.fileobj.fileno(), 4096)
                        if data:
                            relay.feed(decoder.decode(data))
                        else:
                            selector.unregister(event.fileobj)
                relay.feed(decoder.decode(b'', final=True))
                relay.close()

            while process.poll() is None:
                tui.wait(0.1)
            code = process.wait()
        except KeyboardInterrupt:
            # escalate on the whole process group until it exits
            for sig in (signal.SIGINT, signal.SIGTERM, signal.SIGKILL):
                try:
                    os.killpg(process.pid, sig)
                except ProcessLookupError:
                    break
                try:
                    process.wait(timeout=3)
                    break
                except subprocess.TimeoutExpired:
                    continue
            raise

        if code:
            raise subprocess.CalledProcessError(code, command)


def matlab_resources():
    # source tree first, then the installed share directory
    local = Path(__file__).resolve().parent.parent / 'matlab'
    if local.is_dir():
        return local
    from ament_index_python.packages import get_package_share_directory
    return Path(get_package_share_directory('riptide_simulink')) / 'matlab'


def message_key(paths, version):
    h = hashlib.sha256(version.encode())
    for directory in sorted(map(Path, paths)):
        for path in sorted(directory.rglob('*')):
            if path.is_file() and (path.suffix in ('.msg', '.srv', '.action', '.idl')
                                   or path.name in ('package.xml', 'CMakeLists.txt')):
                h.update((directory.name + '/' + path.relative_to(directory).as_posix()).encode())
                h.update(path.read_bytes())
    return h.hexdigest()


def build_cache(cache, model, version, ros):
    """Persistent Simulink CodeGen/Cache folders so unchanged models build incrementally."""
    key = hashlib.sha256('\0'.join((model.identity, version, ros)).encode()).hexdigest()[:16]
    path = Path(cache) / 'builds' / f'{model.name}-{key}'
    marker = path / CACHE_MARKER
    if path.exists() and not marker.is_file():
        raise ValueError(f'Refusing to use unowned build cache: {path}')

    path.mkdir(parents=True, exist_ok=True)
    marker.touch()
    return path


def build_environment(jobs=None):
    jobs = job_count(jobs)
    env = os.environ.copy()
    env['MAKEFLAGS'] = f'-j{jobs} -l{jobs}'
    env['CMAKE_BUILD_PARALLEL_LEVEL'] = str(jobs)
    env['COLCON_DEFAULT_EXECUTOR'] = 'sequential'
    return env


def reset_stale_builds(workspace, source, packages):
    """Drop colcon build directories configured from another source path.

    The legacy manager staged packages under src/controller_models; reusing that
    build directory makes CMake abort because its cached source directory differs.
    """
    root = Path(workspace) / 'build'
    for name in packages:
        build = root / name
        cache = build / 'CMakeCache.txt'
        if not cache.is_file():
            continue

        configured = next((line.split('=', 1)[1].strip() for line in cache.read_text(errors='replace').splitlines()
                           if line.startswith('CMAKE_HOME_DIRECTORY:')), '')
        if not configured or Path(configured).resolve() == (Path(source) / name).resolve():
            continue

        safe_path(build, root)
        print(f'Removing stale build directory {build}; it was configured from {configured}', flush=True)
        shutil.rmtree(build)


def prepare_startup(work, user_startup=False):
    if not user_startup:
        (Path(work) / 'startup.m').write_text(
            '% Manager-owned batch startup. Project/manifest setup runs explicitly.\n')


def message_summary(requests):
    """Custom message counts for the results summary, before MATLAB fills the cache."""
    sources = {source for request in requests for source in request['messages']}
    if not sources:
        return
    interfaces = sum(1 for source in sources for path in Path(source).rglob('*')
                     if path.suffix in ('.msg', '.srv', '.action'))
    cached = all((Path(r['message_cache']) / '.complete').is_file() for r in requests if r['messages'])
    tui.fact('Custom messages', f'{len(sources)} packages, {interfaces} interfaces '
                                f'({"cached" if cached else "generated"})')


def generate(models, arches, cache, matlab='matlab', version='R2023a', ros='humble', operation='generate',
             jobs=None, user_startup=False, work_cache=None):
    jobs = job_count(jobs)
    executable = find_matlab(matlab)
    cache = Path(cache).resolve()
    cache.mkdir(parents=True, exist_ok=True)
    work_cache = Path(work_cache).resolve() if work_cache else cache

    # resolve each model's request and its message/build caches
    try:
        requests = [model.request() for model in models]
        for model, request in zip(models, requests):
            request['message_cache'] = str(work_cache / 'messages' / message_key(request['messages'], version))
            if operation == 'generate':
                request['build_cache'] = str(build_cache(work_cache, model, version, ros))
    except AmbiguousProjectError:
        raise
    except (ValueError, OSError) as error:
        raise GenerationError(f'Local model setup failed: {error}') from error
    message_summary(requests)

    with tempfile.TemporaryDirectory(prefix='riptide-matlab-') as tmp:
        work = Path(tmp)
        prepare_startup(work, user_startup)
        request = dict(models=requests, architectures=arches, matlab=version, ros=ros,
                       output=str(work / 'output'), operation=operation, jobs=jobs)
        atomic_json(work / 'request.json', request)

        # A bare script name also works with legacy wrappers that expand unquoted $*.
        resource = str(matlab_resources()).replace("'", "''")
        (work / 'riptide_driver.m').write_text(
            f"addpath('{resource}');\nriptide_generate(fullfile(pwd,'request.json'));\n")
        try:
            run([executable, '-sd', work, '-batch', 'riptide_driver'], cwd=work, env=build_environment(jobs))
        except subprocess.CalledProcessError as error:
            raise GenerationError(matlab_failure(work / 'output', error, operation)) from error
        except OSError as error:
            raise GenerationError(f'Could not start MATLAB ({executable}): {error}') from error
        if operation != 'generate':
            return []

        try:
            return collect(models, arches, work / 'output', cache, version, ros, requests)
        except (OSError, ValueError, KeyError) as error:
            raise GenerationError(f'MATLAB finished, but its generated packages could not be collected: '
                                  f'{error}') from error


def find_matlab(matlab):
    executable = shutil.which(matlab)
    if not executable:
        raise GenerationError(f'MATLAB executable not found: {matlab}. Install R2023a, or set '
                              f'"MATLAB executable" in Advanced options (--matlab)')
    return executable


def matlab_failure(output, error, operation):
    """Lead with the error the MATLAB driver recorded, not the command line."""
    task = 'message generation' if operation == 'messages' else 'code generation'
    try:
        failure = json.loads((Path(output) / 'error.json').read_text())
        where = ' '.join(filter(None, (Path(failure.get('model') or '').stem, failure.get('architecture'))))
        lines = [line.rstrip() for line in re.sub(r'<[^>]+>', '', failure['message']).splitlines() if line.strip()]
    except (OSError, ValueError, KeyError, TypeError, AttributeError):
        # crashed, licensing, or died before the driver ran
        return (f'MATLAB {task} stopped with exit status {error.returncode} before reporting an error:\n'
                '  crash, license or startup problem; see the MATLAB output for details')

    # the error goes on its own indented lines; long build reports keep their first few
    message = '\n'.join('  ' + line for line in lines[:6]) + ('\n  ...' if len(lines) > 6 else '')
    return f'MATLAB {task} failed' + (f' for {where}' if where else '') + f':\n{message}'


def collect(models, arches, output, cache, version, ros, requests):
    cache = Path(cache)
    generated = json.loads((Path(output) / 'result.json').read_text())
    if isinstance(generated, dict):
        generated = [generated]

    artifacts = []
    generation_id = uuid.uuid4().hex
    for record in generated:
        model = next((m for m in models if str(m.path) == record['model']), None)
        if model is None:
            raise GenerationError(f'MATLAB returned an unrequested model: {record["model"]}')
        archive = Path(record['archive'])

        # check the archive before caching it under a checksummed name
        with tempfile.TemporaryDirectory() as stage:
            packages = sorted(extract(archive, stage))
        checksum = digest(archive)
        filename = f'{model.name}-{version}-{ros}-{record["architecture"]}-{checksum[:16]}.tgz'
        captured = next(r for r in requests if r['model'] == str(model.path))
        shutil.copyfile(archive, cache / filename)

        artifacts.append(dict(model=model.name, identity=model.identity,
                              repository_identity=captured.get('repository_identity', model.identity),
                              repository=model.repository, architecture=record['architecture'],
                              matlab=version, ros=ros, asset=filename, sha256=checksum,
                              packages=packages, source_commit=captured['source_commit'],
                              **{k: captured[k] for k in ('source_branch', 'source_tag', 'built_by') if captured.get(k)},
                              dirty=(captured['dirty'] or model.dirty or model.revision != captured['source_commit']),
                              origin='local', generation_id=generation_id,
                              build_architectures=list(arches),
                              publish_release=model.settings.get('github', {}).get('publish_release', 'models-latest'),
                              generated_at=datetime.now(timezone.utc).isoformat()))

    required = {(m.identity, a) for m in models for a in arches}
    if {(a['identity'], a['architecture']) for a in artifacts} != required:
        raise GenerationError('MATLAB returned an incomplete model/architecture set')

    update_index(cache, artifacts)
    from .publishing import bundle
    bundle(cache, artifacts)
    prune_cache(cache, artifacts)
    return artifacts


def update_index(cache, additions):
    # new records replace old ones for the same model/arch/tool versions
    path = Path(cache) / INDEX
    old = read_index(path)['artifacts'] if path.exists() else []
    keys = {(a['identity'], a['architecture'], a['matlab'], a['ros']) for a in additions}
    old = [a for a in old if (a['identity'], a['architecture'], a['matlab'], a['ros']) not in keys]
    atomic_json(path, dict(schema=1, artifacts=old + additions))


def prune_cache(cache, records, keep=SAVED_BUILDS):
    """Keep the newest saved builds of these models and drop their unreferenced archives."""
    cache = Path(cache)
    models = {r['model'] for r in records}
    patterns = [re.escape(f'{r["model"]}-{r["matlab"]}-{r["ros"]}-{r["architecture"]}-')
                + r'[0-9a-f]{16}(-[0-9a-f]{32})?\.tgz' for r in records]

    # saved builds of only these models, newest first; anything mixed or unreadable stays
    saved, referenced = [], set()
    for path in sorted(cache.glob('publish-*.json'), key=lambda p: p.stat().st_mtime, reverse=True):
        try:
            artifacts = read_index(path)['artifacts']
        except (OSError, ValueError, KeyError, TypeError):
            continue
        if {a['model'] for a in artifacts} <= models:
            if len(saved) >= keep:
                path.unlink()
                continue
            saved.append(path)
        referenced |= {a['asset'] for a in artifacts}

    if (cache / INDEX).exists():
        referenced |= {a['asset'] for a in read_index(cache / INDEX)['artifacts']}
    for path in cache.glob('*.tgz'):
        if path.name not in referenced and any(re.fullmatch(p, path.name) for p in patterns):
            path.unlink()


def saved_token():
    # env token or the saved login, refreshed silently; never prompts
    try:
        return Auth().token(interactive=False)
    except (RuntimeError, OSError):
        return None


class GitHub:
    def __init__(self, token=None):
        self.token = token or saved_token()

    def get(self, url, binary=False):
        parsed = urllib.parse.urlparse(url)
        if parsed.scheme != 'https' or parsed.hostname not in ('api.github.com', 'github.com',
                                                               'objects.githubusercontent.com',
                                                               'release-assets.githubusercontent.com'):
            raise ValueError('Release downloads require a GitHub HTTPS URL')

        headers = {'Accept': 'application/octet-stream' if binary else 'application/vnd.github+json',
                   'User-Agent': 'riptide-simulink'}
        if self.token and parsed.hostname == 'api.github.com':
            headers['Authorization'] = f'Bearer {self.token}'

        # don't leak the token to another host on redirect
        class Redirect(urllib.request.HTTPRedirectHandler):
            def redirect_request(self, req, fp, code, msg, headers, newurl):
                redirected = super().redirect_request(req, fp, code, msg, headers, newurl)
                if (redirected
                        and urllib.parse.urlparse(newurl).hostname != urllib.parse.urlparse(req.full_url).hostname):
                    redirected.remove_header('Authorization')
                return redirected

        try:
            with urllib.request.build_opener(Redirect).open(urllib.request.Request(url, headers=headers),
                                                            timeout=60) as response:
                content = response.read()
        except urllib.error.HTTPError as error:
            if error.code != 401 or 'Authorization' not in headers:
                raise GitHubError(error, url) from error

            # a stale login shouldn't block public releases
            print('GitHub rejected the saved login; continuing without it.')
            self.token = None
            return self.get(url, binary)
        except (urllib.error.URLError, TimeoutError) as error:
            raise unreachable(error) from error
        return content if binary else json.loads(content)

    def hidden(self):
        """Why a release may be invisible, given whether a login was used."""
        if self.token:
            return 'or your GitHub login cannot see that repository'
        return ('or the repository is private (sign in from Advanced options > GitHub account, '
                'or set GITHUB_TOKEN)')

    def release(self, repo, tag, channel='models-latest'):
        if (not repo or len(repo.split('/')) != 2
                or any(c not in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-./' for c in repo)):
            raise ValueError('Set github.repository to owner/repository in simulink.yaml or use --repository')
        # 'latest' means the publication channel, so a hand-made release can't hijack downloads
        base = f'https://api.github.com/repos/{repo}/releases/'
        lookup = channel if tag == 'latest' else tag
        try:
            release = self.get(base + 'tags/' + urllib.parse.quote(lookup, safe=''))
        except urllib.error.HTTPError as error:
            if error.code != 404:
                raise
            if tag != 'latest':
                raise ValueError(f'No release {tag} in {repo}, {self.hidden()}') from error
            try:
                release = self.get(base + 'latest')
            except urllib.error.HTTPError as error:
                if error.code != 404:
                    raise
                raise ValueError(f'No {channel} release in {repo} yet; nobody has published models there, '
                                 f'{self.hidden()}') from error

        # page through every asset
        assets, page = [], 1
        while True:
            batch = self.get(
                f'https://api.github.com/repos/{repo}/releases/{release["id"]}/assets?per_page=100&page={page}')
            assets.extend(batch)
            if len(batch) < 100:
                break
            page += 1
        release['assets'] = assets
        return release


def describe_download(model, repo, release, records, prefixed):
    """Say exactly which build was downloaded, and how it compares with the local checkout."""
    from .publishing import build_details, built_at
    published = built_at({'generated_at': release['published_at']}) if release.get('published_at') else 'unknown'
    details = [('Release', f'{repo}@{release["tag_name"]}, published {published}')]
    details += build_details(records)

    same = model.revision == records[0]['source_commit']
    local = (f'{model.branch or "unknown"} @ {model.revision[:7]}'
             + (' (same commit)' if same else ' (different commit)')
             + (', with uncommitted changes' if model.dirty else ''))
    details.append(('Your checkout', local))

    width = max(len(label) for label, _ in details) + 2
    print(f'Downloaded {model.name}:\n' + '\n'.join(f'  {label + ":":<{width}}{value}' for label, value in details))
    if not same or model.dirty or records[0].get('dirty'):
        print('  NOTICE: the downloaded sources differ from your checkout or include uncommitted changes')
    for label, value in details:
        tui.fact(f'{model.name} {label.lower()}' if prefixed else label, value)


def download(models, arches, cache, version, ros, release=None, repository=None, github=None):
    github = github or GitHub()
    cache = Path(cache)
    cache.mkdir(parents=True, exist_ok=True)
    selected, releases = [], {}
    with tempfile.TemporaryDirectory(prefix='riptide-download-') as tmp:
        tmp = Path(tmp)
        for model in models:
            repo = repository or model.repository
            settings = model.settings.get('github', {})
            tag = release or settings.get('release', 'latest')
            key = (repo, tag)

            # fetch each release's index once
            if key not in releases:
                data = github.release(repo, tag, settings.get('publish_release', 'models-latest'))
                assets = {a['name']: a for a in data['assets']}
                if INDEX not in assets:
                    raise ValueError(f'{repo}@{tag} has no {INDEX}; this release predates the shared artifact format')
                (tmp / INDEX).write_bytes(github.get(assets[INDEX]['url'], binary=True))
                releases[key] = (data, assets, read_index(tmp / INDEX)['artifacts'])
            data, assets, records = releases[key]
            chosen = []

            for arch in arches:
                # prefer an exact workspace match, then a repository match
                matches = [a for a in records if a['model'] == model.name and a['architecture'] == arch
                           and a['matlab'] == version and a['ros'] == ros]
                exact = [a for a in matches if a['identity'] == model.identity]
                repo_exact = [a for a in matches
                              if a.get('repository_identity', a['identity']) == model.repository_identity]
                matches = exact or repo_exact or matches
                if not matches:
                    raise ValueError(f'{repo}@{data["tag_name"]} has no {arch} build of {model.name} for '
                                     f'MATLAB {version} / ROS {ros}; build it locally or publish one first')
                if len(matches) != 1:
                    raise ValueError(f'{repo}@{data["tag_name"]} has {len(matches)} {arch} builds of '
                                     f'{model.name} and none matches {model.identity}; select it by path')

                record = dict(matches[0])
                asset = assets.get(record['asset'])
                if not asset:
                    raise ValueError(f'{repo}@{data["tag_name"]} lists {record["asset"]} in its index but the '
                                     f'file is missing; republish {model.name}')

                # download, checksum, and test-extract
                target = tmp / record['asset']
                target.write_bytes(github.get(asset['url'], binary=True))
                if digest(target) != record['sha256']:
                    raise ValueError(f'Downloaded {target.name} does not match its published checksum; '
                                     f'try again, or republish {model.name} if it keeps happening')
                with tempfile.TemporaryDirectory() as stage:
                    extract(target, stage, record['packages'])

                chosen.append(dict(record))
                record['release_identity'] = record['identity']
                record['origin'] = 'release'
                record['identity'] = model.identity
                selected.append(record)
            describe_download(model, repo, data, chosen, len(models) > 1)

        # only cache once everything checked out
        for record in selected:
            shutil.copyfile(tmp / record['asset'], cache / record['asset'])
        update_index(cache, selected)
    prune_cache(cache, selected)
    return selected


def cached(models, arches, cache, version, ros):
    if not (Path(cache) / INDEX).is_file():
        raise ValueError(f'No cached builds in {cache}; build or download models first')
    records = read_index(Path(cache) / INDEX)['artifacts']
    selected = []
    for model in models:
        for arch in arches:
            matches = [a for a in records
                       if (a['identity'], a['architecture'], a['matlab'], a['ros']) == (model.identity, arch, version, ros)]
            if not matches:
                raise ValueError(f'No cached {arch} build of {model.name} for MATLAB {version} / ROS {ros} '
                                 f'in {cache}; build or download it first')
            if len(matches) != 1:
                raise ValueError(f'The cache index lists {len(matches)} {arch} builds of {model.identity}; '
                                 f'rebuild it to replace them')
            selected.extend(matches)
    return selected


def open_model(model, matlab='matlab'):
    executable = find_matlab(matlab)

    request = model.request()
    with tempfile.TemporaryDirectory(prefix='riptide-open-') as tmp:
        work = Path(tmp)
        atomic_json(work / 'request.json', request)
        (work / 'riptide_open.m').write_text(
            "r=jsondecode(fileread('request.json'));\n"
            "if ~isempty(r.project), openProject(r.project); end\n"
            "addpath(fileparts(r.model),r.config_base);\n"
            "if ~isempty(r.setup), feval(r.setup); end\n"
            "open_system(r.model);\n")
        run([executable, '-desktop', '-r', 'riptide_open'], cwd=work)
