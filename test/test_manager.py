import io
import json
import os
import tarfile
from pathlib import Path
from unittest.mock import patch

import pytest

from riptide_simulink import artifacts as a, backend as b, cleanup, cli, discovery as d, tui


def model(tmp, package='consumer'):
    root = tmp / package / 'models'
    root.mkdir(parents=True)
    (root / 'plant.slx').touch()
    return next(m for m in d.discover(tmp) if m.models_root == root)


def archive(tmp, members=None):
    path = tmp / 'plant.tgz'
    with tarfile.open(path, 'w:gz') as tar:
        for name, data in (members or {'plant/package.xml': b'<package><name>plant</name></package>'}).items():
            item = tarfile.TarInfo(name)
            item.size = len(data)
            tar.addfile(item, io.BytesIO(data))

    return path


def record(path, identity='consumer/models/plant.slx'):
    return dict(model='plant', identity=identity, architecture='x86_64', matlab='R2023a', ros='humble', asset=path.name,
                sha256=a.digest(path), packages=['plant'], source_commit='abc', generated_at='2026-01-01')


def test_discovery(tmp_path):
    m = model(tmp_path)
    model(tmp_path, 'other')
    (m.models_root / 'referenced_models').mkdir()
    (m.models_root / 'referenced_models/child.slx').touch()
    (m.models_root / 'simulink.yaml').write_text(
        'defaults:\n  setup: initialize\nmodels:\n  plant:\n    github:\n      repository: org/repo\n')
    models = d.discover(tmp_path)
    with pytest.raises(ValueError, match='2 matches'):
        d.select(models, ['plant'])
    assert d.select(models, [m.identity])[0].repository == 'org/repo'
    assert len(d.select(models, all_models=True)) == 2
    assert d.select(models, ['child'])[0].excluded
    for folder in ('build', 'install', 'simulink_models', 'controller_models', '.cache', 'matlab_msg_gen'):
        hidden = tmp_path / folder / "models"
        hidden.mkdir(parents=True)
        (hidden / "hidden.slx").touch()

    assert len(d.discover(tmp_path)) == 3


def test_project_ambiguity(tmp_path):
    m = model(tmp_path)
    (m.models_root / 'one.prj').touch()
    (m.models_root / 'two.prj').touch()
    with pytest.raises(ValueError, match='Ambiguous'):
        m.request()
    m.settings['project'] = 'one.prj'
    assert m.request()['project'].endswith('one.prj')


@pytest.mark.parametrize('alias,expected', [('arm', 'arm64'), ('aarch64', 'arm64'), ('amd64', 'x86_64'), ('x86', 'x86_64')])
def test_arches(alias, expected):
    assert d.architecture(alias) == expected


@pytest.mark.parametrize('member', ['../escape', '/absolute', 'plant/../../escape', 'plant\\evil', 'plant/.git/config'])
def test_traversal(tmp_path, member):
    with pytest.raises(ValueError, match='Unsafe'):
        a.extract(archive(tmp_path, {member: b'bad'}), tmp_path / 'out')


def test_links_and_mismatched_packages(tmp_path):
    path = tmp_path / 'bad.tgz'
    with tarfile.open(path, 'w:gz') as tar:
        link = tarfile.TarInfo('plant/link')
        link.type = tarfile.SYMTYPE
        link.linkname = '/tmp'
        tar.addfile(link)

    with pytest.raises(ValueError):
        a.extract(path, tmp_path / 'out')
    with pytest.raises(ValueError, match='metadata'):
        a.extract(archive(tmp_path), tmp_path / 'out2', ['wrong'])


def test_install_rollback_collision_and_checksum(tmp_path):
    cache = tmp_path / 'cache'
    cache.mkdir()
    r = record(archive(cache))
    src = tmp_path / 'ws/src'
    src.mkdir(parents=True)
    dest = src / 'simulink_models'
    a.install([r], cache, dest, src)
    (dest / 'plant/old').touch()
    replace = a.os.replace

    def fail(source, target):
        if Path(source).parent.name == '0':
            raise OSError('interrupted')
        return replace(source, target)

    with patch.object(a.os, 'replace', side_effect=fail), pytest.raises(OSError):
        a.install([r], cache, dest, src)
    assert (dest / 'plant/old').exists()
    (src / 'legacy').mkdir()
    (src / 'legacy/package.xml').write_text('<package><name>plant</name></package>')
    with pytest.raises(ValueError, match='collision'):
        a.install([r], cache, dest, src)
    r['sha256'] = '0' * 64
    with pytest.raises(ValueError, match='Checksum'):
        a.install([r], cache, dest, src)


def test_fallback_and_cancel(tmp_path):
    m = model(tmp_path)
    args = cli.arguments(['build', m.name, '--workspace', str(tmp_path), '--no-process-local', '--no-process-deploy'])
    with patch.object(b, 'generate', side_effect=b.GenerationError('license')), \
            patch.object(b, 'download', return_value=[]) as download:
        cli.execute(args)
        download.assert_called_once()

    args.source = 'local'
    with patch.object(b, 'generate', side_effect=b.GenerationError('license')), patch.object(b, 'download') as download:
        with pytest.raises(b.GenerationError):
            cli.execute(args)
        download.assert_not_called()

    args.source = 'auto'
    with patch.object(b, 'generate', side_effect=KeyboardInterrupt), patch.object(b, 'download') as download:
        with pytest.raises(KeyboardInterrupt):
            cli.execute(args)
        download.assert_not_called()


def test_local_setup_fallback(tmp_path):
    m = model(tmp_path)
    (m.models_root / 'simulink.yaml').write_text('defaults:\n  messages: [missing_messages]\n')
    args = cli.arguments(['build', 'plant', '--workspace', str(tmp_path), '--archives-dir', str(tmp_path / 'cache'),
        '--no-process-local', '--no-process-deploy'])
    with patch.object(b.shutil, 'which', return_value='/usr/bin/matlab'), \
            patch.object(b, 'download', return_value=[]) as download:
        cli.execute(args)
        download.assert_called_once()


def test_colcon_failure_does_not_fallback(tmp_path):
    m = model(tmp_path)
    args = cli.arguments(['build', m.name, '--workspace', str(tmp_path),
                          '--local-dir', str(tmp_path / 'dev/src/simulink_models')])
    r = dict(architecture=d.architecture(__import__('platform').machine()))
    with patch.object(b, 'generate', return_value=[r]), patch.object(cli, 'install', return_value=['plant']), \
            patch.object(b, 'run', side_effect=RuntimeError('colcon')) as run, \
            patch.object(b, 'download') as download:
        with pytest.raises(RuntimeError, match='colcon'):
            cli.execute(args)
        download.assert_not_called()
        assert run.call_args.args[1] == args.local_dir.parent.parent


def test_cleanup(tmp_path):
    args = cli.arguments(['clean', '--archives-dir', str(tmp_path / 'cache'),
                          '--local-dir', str(tmp_path / 'ws/src/simulink_models'),
                          '--deploy-dir', str(tmp_path / 'rel/src/simulink_models')])
    args.archives_dir.mkdir()
    r = record(archive(args.archives_dir))
    b.update_index(args.archives_dir, [r])
    a.install([r], args.archives_dir, args.local_dir, args.local_dir.parent)
    args.yes = True
    with pytest.raises(ValueError, match='--all --yes'):
        cleanup.execute(args, lambda _: True)
    args.include = ['plant']
    cleanup.execute(args, lambda _: False)
    assert not (args.local_dir / 'plant').exists()
    with pytest.raises(ValueError, match='escapes'):
        cleanup.safe_path(tmp_path.parent, tmp_path)
    (tmp_path / 'link').symlink_to(tmp_path.parent, target_is_directory=True)
    with pytest.raises(ValueError, match='symlink'):
        cleanup.safe_path(tmp_path / 'link/anything', tmp_path)


def test_cache_and_message_keys(tmp_path):
    m = model(tmp_path)
    r = record(archive(tmp_path), m.identity)
    b.update_index(tmp_path, [r])
    assert b.cached([m], ['x86_64'], tmp_path, 'R2023a', 'humble') == [r]
    with pytest.raises(ValueError):
        b.cached([m], ['arm64'], tmp_path, 'R2023a', 'humble')
    root = tmp_path / 'msgs'
    root.mkdir()
    (root / 'Test.msg').write_text('int32 value')
    before = b.message_key([root], 'R2023a')
    (root / 'Test.msg').write_text('float64 value')
    assert b.message_key([root], 'R2023a') != before


def test_download(tmp_path):
    m = model(tmp_path)
    path = archive(tmp_path)
    r = record(path, 'models/plant.slx')

    class GitHub:
        def release(self, repo, tag, channel):
            return dict(tag_name='latest', assets=[dict(name=a.INDEX, url='index'), dict(name=path.name, url='archive')])

        def get(self, url, binary=False):
            return json.dumps(dict(schema=1, artifacts=[r])).encode() if url == 'index' else path.read_bytes()

    records = b.download([m], ['x86_64'], tmp_path / 'cache', 'R2023a', 'humble', github=GitHub())
    assert records[0]['identity'] == m.identity
    assert records[0]['release_identity'] == 'models/plant.slx'


def test_cli_and_terminal():
    assert cli.arguments(['plant']).models == ['plant']
    assert cli.arguments(['generate_packages', '--models-select', 'plant']).source == 'local'
    with patch('sys.stdin.isatty', return_value=False), pytest.raises(ValueError):
        tui.choose('Models', ['plant'])

    class Screen:
        def __init__(self, keys):
            self.keys = iter(keys)

        def keypad(self, *_):
            pass

        def timeout(self, *_):
            pass

        def erase(self):
            pass

        def getmaxyx(self):
            return 20, 80

        def addnstr(self, *_):
            pass

        def refresh(self):
            pass

        def getch(self):
            return next(self.keys)

    assert tui._menu(Screen([ord(' '), tui.curses.KEY_DOWN, ord(' '), 10]), 'Models', ['a', 'b'], True) == [0, 1]
    with pytest.raises(KeyboardInterrupt):
        tui._menu(Screen([ord('q')]), 'Models', ['a'], False)


def test_jobs_are_bounded():
    env = b.build_environment(1)
    assert env['MAKEFLAGS'] == '-j1 -l1'
    assert env['CMAKE_BUILD_PARALLEL_LEVEL'] == '1'
    with pytest.raises(ValueError):
        b.build_environment(0)
    from riptide_simulink.resources import available_cpus
    assert cli.arguments(['build', 'plant']).jobs == available_cpus()
    assert cli.arguments(['build', 'plant', '--jobs', '3']).jobs == 3


def test_manifest_missing_and_bad_types(tmp_path):
    model(tmp_path)
    with pytest.raises(ValueError, match='Manifest not found'):
        d.discover(tmp_path, tmp_path / 'absent.yaml')
    bad = tmp_path / 'bad.yaml'
    bad.write_text('defaults: [wrong]\n')
    with pytest.raises(ValueError, match='mappings'):
        d.discover(tmp_path, bad)


def test_plain_menu_fallback():
    with patch('sys.stdin.isatty', return_value=True), patch('sys.stdout.isatty', return_value=True), \
            patch.object(tui.curses, 'wrapper', side_effect=tui.curses.error), \
            patch('builtins.input', return_value='2'):
        assert tui.choose('Models', ['a', 'b']) == [1]


def test_cleanup_preserves_tracked_files(tmp_path):
    import subprocess
    root = tmp_path / 'managed'
    root.mkdir()
    package = root / 'plant'
    package.mkdir()
    (package / 'source.cpp').write_text('important')
    subprocess.run(['git', 'init', '-q', str(root)], check=True)
    subprocess.run(['git', '-C', str(root), 'add', '.'], check=True)
    with pytest.raises(ValueError, match='tracked'):
        cleanup.safe_path(package, root)


@pytest.mark.parametrize('field,value', [('packages', 'plant'), ('packages', [{}]), ('sha256', 123), ('architecture', 'mips')])
def test_invalid_index_types(tmp_path, field, value):
    r = record(archive(tmp_path))
    r[field] = value
    a.atomic_json(tmp_path / a.INDEX, dict(schema=1, artifacts=[r]))
    with pytest.raises(ValueError):
        a.read_index(tmp_path / a.INDEX)


def test_single_manager_lock(tmp_path):
    from riptide_simulink.locking import manager_lock
    with patch('riptide_simulink.locking.tempfile.gettempdir', return_value=str(tmp_path)):
        with manager_lock():
            with pytest.raises(RuntimeError, match='Another model manager'):
                with manager_lock():
                    pass


def test_cleanup_parent_traversal(tmp_path):
    root = tmp_path / 'root'
    root.mkdir()
    with pytest.raises(ValueError, match='escapes'):
        cleanup.safe_path(root / '..' / 'outside', root)


def test_clean_preserves_sources_and_unowned_cache(tmp_path):
    args = cli.arguments(['clean', '--all', '--yes', '--no-delete-packages', '--full-clean',
        '--archives-dir', str(tmp_path / 'cache'), '--local-dir', str(tmp_path / 'ws/src/simulink_models'),
        '--deploy-dir', str(tmp_path / 'release/src/simulink_models')])
    args.archives_dir.mkdir()
    r = record(archive(args.archives_dir))
    b.update_index(args.archives_dir, [r])
    a.install([r], args.archives_dir, args.local_dir, args.local_dir.parent)
    build = tmp_path / 'ws/build/plant'
    build.mkdir(parents=True)
    owned = args.archives_dir / 'messages/owned'
    owned.mkdir(parents=True)
    (owned / '.riptide-simulink-cache').touch()
    unrelated = args.archives_dir / 'messages/other'
    unrelated.mkdir()
    builds = args.archives_dir / 'builds/plant-0123'
    builds.mkdir(parents=True)
    (builds / '.riptide-simulink-cache').touch()
    unowned_builds = args.archives_dir / 'builds/other'
    unowned_builds.mkdir()
    cleanup.execute(args, lambda _: False)
    assert not build.exists() and not owned.exists() and not builds.exists()
    assert unrelated.exists() and unowned_builds.exists() and (args.local_dir / 'plant').exists()
    assert (args.archives_dir / r['asset']).exists()


def test_discover_from_models_root(tmp_path):
    m = model(tmp_path)
    (m.models_root / 'nested').mkdir()
    (m.models_root / 'nested/other.slx').touch()
    assert len(d.discover(m.models_root)) == 2


def test_malformed_archive_errors(tmp_path):
    path = tmp_path / 'bad.tgz'
    path.write_bytes(b'not an archive')
    with pytest.raises(ValueError, match='Malformed'):
        a.extract(path, tmp_path / 'out')
    path = archive(tmp_path, {'plant/package.xml': b'<invalid'})
    with pytest.raises(ValueError, match='Malformed'):
        a.extract(path, tmp_path / 'out')


def test_release_identity_independent_of_checkout_folder(tmp_path):
    import subprocess
    m = model(tmp_path)
    subprocess.run(['git', 'init', '-q', str(m.models_root.parent)], check=True)
    assert m.repository_identity == 'models/plant.slx'
    assert m.dirty  # New, untracked models also differ from the recorded commit.
    path = archive(tmp_path)
    r = record(path, 'models/plant.slx')
    r['repository_identity'] = 'models/plant.slx'
    other = dict(r, identity='other/models/plant.slx', repository_identity='other/models/plant.slx')

    class GitHub:
        def release(self, *_):
            return dict(tag_name='latest', assets=[dict(name=a.INDEX, url='index'), dict(name=path.name, url='archive')])

        def get(self, url, binary=False):
            return json.dumps(dict(schema=1, artifacts=[r, other])).encode() if url == 'index' else path.read_bytes()

    records = b.download([m], ['x86_64'], tmp_path / 'cache', 'R2023a', 'humble', github=GitHub())
    assert records[0]['repository_identity'] == 'models/plant.slx'


def test_collision_respects_colcon_ignore(tmp_path):
    cache = tmp_path / 'cache'
    cache.mkdir()
    r = record(archive(cache))
    src = tmp_path / 'src'
    legacy = src / 'legacy'
    legacy.mkdir(parents=True)
    (legacy / 'package.xml').write_text('<package><name>plant</name></package>')
    (legacy / 'COLCON_IGNORE').touch()
    assert a.install([r], cache, src / 'simulink_models', src) == ['plant']


@pytest.mark.parametrize('personal_startup', [False, True])
def test_batch_startup_is_isolated_unless_explicitly_enabled(tmp_path, personal_startup):
    m = model(tmp_path)

    def run(command, cwd, env):
        assert command[command.index('-sd') + 1] == cwd
        assert command[-2:] == ['-batch', 'riptide_driver']
        assert '-singleCompThread' not in command
        assert (cwd / 'startup.m').exists() is not personal_startup
        if not personal_startup:
            assert 'satk' not in (cwd / 'startup.m').read_text().lower()
        assert (cwd / 'riptide_driver.m').is_file()

    with patch.object(b.shutil, 'which', return_value='/fake/matlab'), \
            patch.object(b, 'run', side_effect=run), patch.object(b, 'collect', return_value=[]):
        assert b.generate([m], ['x86_64'], tmp_path / 'cache', user_startup=personal_startup) == []


def test_stale_legacy_build_directory_is_reset(tmp_path):
    source = tmp_path / 'src' / 'simulink_models'
    (source / 'plant').mkdir(parents=True)
    for name, home in (('plant', tmp_path / 'src/controller_models/plant'), ('kept', source / 'kept')):
        (tmp_path / 'build' / name).mkdir(parents=True)
        (tmp_path / 'build' / name / 'CMakeCache.txt').write_text(f'CMAKE_HOME_DIRECTORY:INTERNAL={home}\n')

    (tmp_path / 'build' / 'unconfigured').mkdir()
    b.reset_stale_builds(tmp_path, source, ['plant', 'kept', 'unconfigured', 'missing'])
    assert not (tmp_path / 'build' / 'plant').exists()
    assert (tmp_path / 'build' / 'kept' / 'CMakeCache.txt').is_file()
    assert (tmp_path / 'build' / 'unconfigured').is_dir()


def test_generation_uses_persistent_owned_build_cache(tmp_path):
    m = model(tmp_path)
    seen = []

    def run(command, cwd, env):
        seen.append(json.loads((cwd / 'request.json').read_text())['models'][0]['build_cache'])

    with patch.object(b.shutil, 'which', return_value='/fake/matlab'), \
            patch.object(b, 'run', side_effect=run), patch.object(b, 'collect', return_value=[]):
        b.generate([m], ['x86_64'], tmp_path / 'cache')
        b.generate([m], ['x86_64'], tmp_path / 'cache')

    assert seen[0] == seen[1] and Path(seen[0]).parent == tmp_path / 'cache' / 'builds'
    assert Path(seen[0]).name.startswith('plant-') and (Path(seen[0]) / '.riptide-simulink-cache').is_file()
    assert b.build_cache(tmp_path / 'cache', m, 'R2023a', 'jazzy') != Path(seen[0])
    (Path(seen[0]) / '.riptide-simulink-cache').unlink()
    with patch.object(b.shutil, 'which', return_value='/fake/matlab'), \
            pytest.raises(b.GenerationError, match='unowned build cache'):
        b.generate([m], ['x86_64'], tmp_path / 'cache')


def test_cached_builds_keep_build_provenance_for_later_upload(tmp_path):
    from riptide_simulink import publishing
    m = model(tmp_path)
    output = tmp_path / 'output'
    output.mkdir()
    (output / 'result.json').write_text(json.dumps(
        [dict(model=str(m.path), architecture='x86_64', archive=str(archive(tmp_path)))]))
    request = dict(model=str(m.path), source_commit='c' * 40, dirty=False, repository_identity='models/plant.slx',
                   source_branch='mpc', source_tag='v3', built_by='ethan@desk')
    (tmp_path / 'cache').mkdir()
    b.collect([m], ['x86_64'], output, tmp_path / 'cache', 'R2023a', 'humble', [request])

    # the publish bundle (used by "Publish a cached local build") and the local index both carry it
    bundled = next((tmp_path / 'cache').glob('publish-*.json'))
    for path in (bundled, tmp_path / 'cache' / a.INDEX):
        saved = a.read_index(path)['artifacts'][0]
        assert (saved['source_branch'], saved['source_tag'], saved['built_by']) == ('mpc', 'v3', 'ethan@desk')
    body = publishing.release_body('org/repo', a.read_index(bundled)['artifacts'])
    assert 'on `mpc`' in body and 'by `ethan@desk`' in body


def test_symlinks_above_the_workspace_are_allowed(tmp_path):
    cache = tmp_path / 'cache'
    cache.mkdir()
    (tmp_path / 'drive/ws/src').mkdir(parents=True)
    (tmp_path / 'ws').symlink_to(tmp_path / 'drive/ws', target_is_directory=True)
    src = tmp_path / 'ws/src'
    assert a.install([record(archive(cache))], cache, src / 'simulink_models', src) == ['plant']
    assert cleanup.safe_path(src / 'simulink_models/plant', src)

    # but not inside it
    (src / 'linked').symlink_to(tmp_path / 'drive', target_is_directory=True)
    with pytest.raises(ValueError, match='symlink'):
        a.install([record(archive(cache))], cache, src / 'linked/models', src)


def test_prune_cache_keeps_recent_builds_of_only_the_built_model(tmp_path):
    cache = tmp_path / 'cache'
    cache.mkdir()

    def saved(name, generation, age):
        path = cache / f'{name}-R2023a-humble-x86_64-{generation:016x}.tgz'
        path.write_bytes(archive(tmp_path).read_bytes())
        r = dict(record(path), model=name)
        index = cache / f'publish-{name}{generation}.json'
        a.atomic_json(index, dict(schema=1, artifacts=[r]))
        os.utime(index, (age, age))
        return r

    for generation in range(8):
        r = saved('plant', generation, 1000 + generation)
    saved('other', 0, 1)
    orphan = cache / f'plant-R2023a-humble-x86_64-{99:016x}.tgz'
    orphan.touch()
    installed = cache / f'plant-R2023a-humble-x86_64-{98:016x}-{0:032x}.tgz'
    installed.touch()
    a.atomic_json(cache / a.INDEX, dict(schema=1, artifacts=[dict(r, asset=installed.name)]))
    (cache / 'notes.tgz').touch()

    b.prune_cache(cache, [r])
    left = sorted(p.name for p in cache.glob('publish-*.json'))
    assert left == ['publish-other0.json'] + [f'publish-plant{g}.json' for g in range(3, 8)]
    assert not (cache / f'plant-R2023a-humble-x86_64-{0:016x}.tgz').exists() and not orphan.exists()
    assert (cache / f'plant-R2023a-humble-x86_64-{3:016x}.tgz').exists() and installed.exists()
    assert (cache / f'other-R2023a-humble-x86_64-{0:016x}.tgz').exists() and (cache / 'notes.tgz').exists()


def test_no_archive_builds_keep_the_persistent_build_cache(tmp_path):
    m = model(tmp_path)
    seen = []

    def run(command, cwd, env):
        request = json.loads((cwd / 'request.json').read_text())['models'][0]
        seen.append((request['build_cache'], request['message_cache']))

    with patch.object(b.shutil, 'which', return_value='/fake/matlab'), \
            patch.object(b, 'run', side_effect=run), patch.object(b, 'collect', return_value=[]):
        b.generate([m], ['x86_64'], tmp_path / 'throwaway', work_cache=tmp_path / 'cache')

    assert all(Path(path).is_relative_to(tmp_path / 'cache') for path in seen[0])
