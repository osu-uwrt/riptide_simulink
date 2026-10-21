"""Validated, serialized release updates."""
from collections import defaultdict
from datetime import datetime, timezone
import json
from pathlib import Path
import re
import tempfile
import urllib.error
import urllib.parse
import uuid

from .artifacts import INDEX, atomic_json, digest, extract, read_index, validate_index
from . import tui

LOCK = 'riptide-publish-lock.json'


def key(record):
    return (record.get('repository', ''), record.get('repository_identity', record['identity']),
            record['architecture'], record['matlab'], record['ros'])


def validate_records(records):
    seen, owners = set(), {}
    for record in records:
        identity = key(record)
        if identity in seen:
            raise ValueError(f'Duplicate model/architecture entry: {identity}')
        seen.add(identity)
        for package in record['packages']:
            # Architecture/tool variants of the same model may share package names.
            owner_key = (identity[0], package)
            owner = identity[:2]
            if owner_key in owners and owners[owner_key] != owner:
                raise ValueError(f'Conflicting package ownership: {package}')
            owners[owner_key] = owner


def validate_local(index, directory, allow_dirty=False):
    validate_index({'schema': 1, **index})
    records = index['artifacts']
    if not records:
        raise ValueError('Refusing empty model publication')
    validate_records(records)

    # the index must still hold exactly the captured selection
    expected = index.get('publication', {}).get('expected')
    if expected is not None and (len(expected) != len(records)
                                 or {tuple(k) for k in expected} != {key(a) for a in records}):
        raise ValueError('Incomplete selected build set in publication index')

    # per record: provenance, cleanliness, checksum, and a test extraction
    groups = defaultdict(list)
    for record in records:
        if not re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+', record.get('repository', '')):
            raise ValueError('Publication requires a GitHub repository for every model')
        if record.get('origin', 'local') != 'local' or 'release_identity' in record:
            raise ValueError('Downloaded release artifacts cannot be published as local builds')
        if not re.fullmatch(r'[0-9a-f]{40}', record['source_commit']):
            raise ValueError('Publication requires committed source provenance')
        if not isinstance(record.get('dirty'), bool):
            raise ValueError('Missing captured source cleanliness in artifact metadata')
        if record['dirty'] and not allow_dirty:
            raise ValueError('Build captured modified sources; rebuild from clean sources or '
                             'explicitly use --allow-dirty')
        archive = Path(directory) / record['asset']
        if digest(archive) != record['sha256']:
            raise ValueError(f'Artifact checksum mismatch before publication: {archive.name}')
        with tempfile.TemporaryDirectory(prefix='riptide-publish-check-') as stage:
            extract(archive, stage, record['packages'])
        groups[(key(record)[:2], record['matlab'], record['ros'])].append(record)

    # all architectures of one model must come from the same build
    for records in groups.values():
        if len({(r['source_commit'], r.get('generation_id')) for r in records}) != 1:
            raise ValueError('Selected architectures came from different source revisions/builds')
        for record in records:
            if 'build_architectures' in record and set(record['build_architectures']) != {
                    r['architecture'] for r in records}:
                raise ValueError('Incomplete selected architecture set; publish the full captured build')


def bundle(directory, records):
    """Snapshot the precise selection, independent of future cache updates."""
    generation_ids = {r.get('generation_id') for r in records}
    build_id = next(iter(generation_ids)) if len(generation_ids) == 1 else None

    # reuse the build's generation id in the name when there is exactly one
    if not isinstance(build_id, str) or not re.fullmatch('[0-9a-f]{32}', build_id):
        build_id = uuid.uuid4().hex
    path = Path(directory) / f'publish-{build_id}.json'
    atomic_json(path, dict(schema=1, artifacts=records,
                           publication=dict(expected=[key(r) for r in records])))
    return path


def asset_map(api, release):
    result = {}
    for asset in api.assets(release):
        if asset['name'] in result:
            raise ValueError('Release contains duplicate asset names')
        result[asset['name']] = asset
    return result


def _read_remote(api, asset):
    with tempfile.TemporaryDirectory(prefix='riptide-release-index-') as tmp:
        path = Path(tmp) / INDEX
        path.write_bytes(api.download(asset))
        return read_index(path)


def _lock_message(api, asset):
    try:
        info = json.loads(api.download(asset))
        return f'owner {info.get("owner", "unknown")}, started {info.get("created_at", "unknown")}'
    except (ValueError, OSError):
        return 'owner/time unavailable'


def built_at(record):
    """Build time in UTC, minute resolution, for display."""
    try:
        moment = datetime.fromisoformat(re.sub(r'\.\d+', '', record['generated_at']).replace('Z', '+00:00'))
        return moment.astimezone(timezone.utc).strftime('%Y-%m-%d %H:%M UTC')
    except ValueError:
        return record['generated_at']


def build_details(records):
    """Label/value description of one model build (all its architectures), as shown on download."""
    first = records[0]
    tag = first.get('source_tag')
    kind = ('Dev build: uncommitted or untracked changes' if first.get('dirty') else
            f'Tagged build ({tag})' if tag else 'Clean build: committed sources only')
    return [('Build', kind), ('Branch', first.get('source_branch') or 'unknown'),
            ('Commit', first['source_commit']), *([('Tag', tag)] if tag else []),
            ('Built', f'{built_at(first)} by {first.get("built_by") or "unknown"}'),
            ('Architectures', ', '.join(sorted(r['architecture'] for r in records))),
            ('Tools', f'MATLAB {first["matlab"]}, ROS 2 {first["ros"]}'),
            ('Packages', ', '.join(sorted({p for r in records for p in r['packages']})))]


def build_tag(records):
    """Titan-style tag columns for a saved build: models, branch/rev-dirty (tag), time, builder, architectures."""
    first = min(records, key=lambda r: r['generated_at'])
    revision = first['source_commit'][:7] + ('-dirty' if any(r.get('dirty') for r in records) else '')
    if first.get('source_branch'):
        revision = f'{first["source_branch"]}/{revision}'
    if first.get('source_tag'):
        revision += f' ({first["source_tag"]})'
    return [', '.join(sorted({r['model'] for r in records})), revision, built_at(first),
            first.get('built_by') or '', '+'.join(sorted({r['architecture'] for r in records}))]


def release_body(repo, records):
    """Markdown release text: one section per model build (newest first), like Titan's build tag."""
    builds = defaultdict(list)
    for record in records:
        builds[(record.get('repository_identity', record['identity']), record.get('generation_id'))].append(record)
    ordered = sorted(builds.values(), key=lambda group: group[0]['generated_at'], reverse=True)

    lines = ['Generated ROS 2 source packages from Simulink models, built locally with MATLAB.', '']
    for group in ordered:
        first = group[0]
        commit, tag = first['source_commit'], first.get('source_tag')
        if first.get('dirty'):
            state = '⚠️ **Dev build**: uncommitted or untracked changes in the working tree'
        elif tag:
            state = f'🏷️ **Tagged build** (`{tag}`), clean working tree'
        else:
            state = '✅ **Clean build**: committed sources only'

        lines += [f'### {first["model"]}', state, '',
                  f'- **Commit:** [`{commit[:7]}`](https://github.com/{repo}/commit/{commit})'
                  f' on `{first.get("source_branch") or "unknown branch"}`',
                  f'- **Built:** {built_at(first)} by `{first.get("built_by") or "unknown"}`',
                  f'- **Architectures:** {", ".join(sorted(r["architecture"] for r in group))}'
                  f' · MATLAB {first["matlab"]} · ROS 2 {first["ros"]}',
                  f'- **Packages:** {", ".join(f"`{p}`" for p in sorted({p for r in group for p in r["packages"]}))}',
                  '']
    lines.append(f'<sub>Full commit hashes, checksums and file names are in `{INDEX}`.</sub>')
    return '\n'.join(lines)


def prune(api, release, published, kept, previous, progress):
    """Drop the pushed models' superseded archives and stale index backups; other models are untouched."""
    patterns = [re.escape(f'{r["model"]}-{r["matlab"]}-{r["ros"]}-{r["architecture"]}-')
                + r'[0-9a-f]{16}-[0-9a-f]{32}\.tgz' for r in published]
    keep = {INDEX, LOCK, previous} | {r['asset'] for r in kept}

    # best effort: the release is already published, and the next publish retries
    try:
        stale = [a for a in api.assets(release) if a['name'] not in keep and (
            re.fullmatch(r'(previous-|failed-)?index-[0-9a-f]{32}\.json', a['name'])
            or any(re.fullmatch(pattern, a['name']) for pattern in patterns))]
    except Exception as error:
        progress(f'Could not list old release assets for cleanup: {error}')
        return
    failed = 0
    for asset in stale:
        try:
            api.call('DELETE', f'/releases/assets/{asset["id"]}')
        except Exception:
            failed += 1
    if failed:
        progress(f'Could not remove {failed} old release assets; the next publish will retry')


def publish(directory, api, tag, revision=None, *, records=None, owner='unknown',
            allow_dirty=False, progress=print, latest=None):
    """Publish one repository/tag; hold a remotely exclusive asset lock throughout."""
    directory = Path(directory)
    index = read_index(directory / INDEX) if records is None else dict(schema=1, artifacts=records)
    validate_local(index, directory, allow_dirty)
    records = index['artifacts']
    if {r['repository'] for r in records} != {api.repo}:
        raise ValueError('Publication records do not belong to the target repository')
    if not tag or tag == 'latest':
        raise ValueError('Choose a concrete publication tag (default: models-latest)')

    # find or create the release
    endpoint = '/releases/tags/' + urllib.parse.quote(tag, safe='')
    tui.check_cancel()
    try:
        release = api.call('GET', endpoint)
    except urllib.error.HTTPError as error:
        if error.code != 404:
            raise
        release = None
    if release is None:
        # an earlier failed first publication leaves a draft, which tag lookup can't see
        release = next((r for r in api.call('GET', '/releases?per_page=100')
                        if r.get('draft') and r.get('tag_name') == tag), None)
    if release is None:
        # A rolling tag is a channel. Anchor a newly created tag to the remote default
        # branch, rather than silently pushing a possibly local-only source revision.
        repository = api.call('GET', '')
        try:
            release = api.call('POST', '/releases', dict(
                tag_name=tag, target_commitish=repository['default_branch'],
                name='Simulink model builds', draft=True))
        except urllib.error.HTTPError as creation_error:
            if creation_error.code != 422:
                raise
            release = api.call('GET', endpoint)  # Another publisher created it first.

    if release.get('immutable'):
        raise ValueError('Target release is immutable; choose a mutable rolling release')
    if release.get('prerelease'):
        raise ValueError('Refusing to convert an existing prerelease into a rolling release')

    lock = None
    try:
        # take the lock: the upload fails if the asset already exists
        info = dict(owner=owner, created_at=datetime.now(timezone.utc).isoformat(),
                    operation=uuid.uuid4().hex)
        try:
            lock = api.upload(release, LOCK, json.dumps(info).encode())
        except urllib.error.HTTPError as error:
            if error.code != 422:
                raise
            asset = asset_map(api, release).get(LOCK)
            if not asset:
                raise
            raise RuntimeError(
                f'Publication busy ({_lock_message(api, asset)}). Retry after it finishes. '
                f'If interrupted, an administrator must confirm no publisher is running and '
                f'delete only {LOCK} from {api.repo}@{tag}.') from error

        # merge with the current remote index
        assets = asset_map(api, release)
        old_index = assets.get(INDEX)
        old = _read_remote(api, old_index)['artifacts'] if old_index else []
        # older indexes may lack a repository; assume the release's own
        for record in old:
            record.setdefault('repository', api.repo)
            if record['repository'] != api.repo:
                raise ValueError('Existing release index includes a different repository')
        validate_records(old)
        for record in old:
            if record['asset'] not in assets:
                raise ValueError(f'Existing index references missing asset: {record["asset"]}')
        incoming = {key(r) for r in records}
        additions = [dict(r, identity=r.get('repository_identity', r['identity'])) for r in records]
        merged = [r for r in old if key(r) not in incoming] + additions
        validate_records(merged)

        # upload archives, then stage the new index next to the old one
        unique = uuid.uuid4().hex
        for record in additions:
            tui.check_cancel()
            path = directory / record['asset']
            # Unique names also distinguish identical archive bytes with new provenance.
            record['asset'] = f'{path.stem}-{unique}.tgz'
            progress(f'Uploading {record["model"]}/{record["architecture"]} to {api.repo}@{tag}')
            uploaded = api.upload(release, record['asset'], path)
            if uploaded.get('state') != 'uploaded' or uploaded.get('size') != path.stat().st_size:
                raise RuntimeError('GitHub did not confirm the complete archive upload')
            remote_digest = uploaded.get('digest')
            if remote_digest and remote_digest != 'sha256:' + record['sha256']:
                raise RuntimeError('GitHub archive checksum differs from the local checksum')

        tui.check_cancel()
        staged = api.upload(release, f'index-{unique}.json',
                            json.dumps(dict(schema=1, artifacts=merged), indent=2).encode())

        # swap the index names and publish the release
        try:
            if old_index:
                api.call('PATCH', f'/releases/assets/{old_index["id"]}',
                         dict(name=f'previous-index-{unique}.json'))
            api.call('PATCH', f'/releases/assets/{staged["id"]}', dict(name=INDEX))
            # the publication channel becomes the repository's latest release; test tags don't
            latest = tag == 'models-latest' if latest is None else latest
            api.call('PATCH', f'/releases/{release["id"]}',
                     dict(draft=False, make_latest='true' if latest else 'false', body=release_body(api.repo, merged)))
        except BaseException:
            try:
                # Inspect actual remote state: a PATCH may succeed before its response
                # is lost. Local flags cannot safely distinguish that from failure.
                current = {a['id']: a for a in api.assets(release)}
                if current.get(staged['id'], {}).get('name') == INDEX:
                    api.call('PATCH', f'/releases/assets/{staged["id"]}',
                             dict(name=f'failed-index-{unique}.json'))
                if old_index and current.get(old_index['id'], {}).get('name') != INDEX:
                    api.call('PATCH', f'/releases/assets/{old_index["id"]}', dict(name=INDEX))
            except BaseException as rollback_error:
                # Leave the lock in place so another publisher cannot obscure recovery.
                lock = None
                raise RuntimeError('Publication failed and rollback could not complete. '
                                   f'Administrator recovery required on {api.repo}@{tag}; '
                                   'previous archives/index backups were retained.') from rollback_error
            raise

        prune(api, release, additions, merged + old, f'previous-index-{unique}.json', progress)
        url = f'https://github.com/{api.repo}/releases/tag/{urllib.parse.quote(tag, safe="")}'
        progress(f'Published: {url}')
        return url
    finally:
        if lock:
            try:
                api.call('DELETE', f'/releases/assets/{lock["id"]}')
            except Exception as error:
                raise RuntimeError(f'Could not remove {LOCK} on {api.repo}@{tag}; '
                                   'check release status before retrying. Administrator recovery '
                                   'may be needed.') from error
