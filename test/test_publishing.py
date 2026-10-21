"""Offline GitHub tests: no credentials, browser or network are used."""
import io
import json
import tarfile
from unittest.mock import Mock, patch
from urllib.error import HTTPError

import pytest

from riptide_simulink import artifacts, auth, backend, cli, publishing as p
from riptide_simulink.github import API, Redirects


real_saved_token = backend.saved_token


@pytest.fixture(autouse=True)
def no_credentials(monkeypatch):
    for key in ('GITHUB_TOKEN', 'GH_TOKEN', 'RIPTIDE_SIMULINK_GITHUB_CLIENT_ID'):
        monkeypatch.delenv(key, raising=False)
    monkeypatch.setenv('RIPTIDE_SIMULINK_GITHUB_CLIENT_ID', 'public-client-id')
    monkeypatch.setattr(backend, 'saved_token', lambda: None)


def record(tmp, name='plant', arch='x86_64', repo='org/repo'):
    path = tmp / f'{name}-{arch}.tgz'
    with tarfile.open(path, 'w:gz') as archive:
        data = f'<package><name>{name}</name></package>'.encode()
        member = tarfile.TarInfo(f'{name}/package.xml')
        member.size = len(data)
        archive.addfile(member, io.BytesIO(data))

    return dict(model=name, identity=f'consumer/models/{name}.slx',
                repository_identity=f'models/{name}.slx', repository=repo,
                architecture=arch, matlab='R2023a', ros='humble', packages=[name],
                asset=path.name, sha256=artifacts.digest(path), source_commit='a' * 40,
                dirty=False, origin='local', generation_id='build-1',
                build_architectures=[arch], generated_at='2026-10-19T12:00:00Z')


def error(code):
    return HTTPError('https://api.github.com/test', code, 'test error', {}, None)


class MemoryStore:
    def __init__(self):
        self.data = {}

    def get(self, key):
        return self.data.get(key)

    def put(self, key, value):
        self.data[key] = value

    def delete(self, key):
        self.data.pop(key, None)


class Timer:
    def __init__(self):
        self.time = 1000
        self.sleeps = []

    def now(self):
        return self.time

    def sleep(self, duration):
        self.sleeps.append(duration)
        self.time += duration


def device():
    return dict(device_code='private-device-code', user_code='ABCD-EFGH',
                verification_uri='https://github.com/login/device', expires_in=900, interval=5)


def tokens():
    return dict(access_token='private-access-token', refresh_token='private-refresh-token',
                expires_in=28800, refresh_token_expires_in=15897600)


def client(responses, browser=None):
    timer = Timer()
    http = Mock(side_effect=responses)
    store = MemoryStore()
    obj = auth.Auth(store=store, http=http, now=timer.now, monotonic=timer.now,
                    sleep=timer.sleep, browser=browser or Mock(return_value=True))
    return obj, http, timer, store


def test_device_backoff_browser_failure_and_remembered_login(capsys):
    obj, http, timer, store = client([device(), {'error': 'authorization_pending'},
                                   {'error': 'slow_down'}, tokens()], Mock(side_effect=OSError))
    assert obj.token(True) == 'private-access-token'
    assert timer.sleeps == [5, 5, 10]
    count = http.call_count
    assert obj.token(False) == 'private-access-token'
    assert http.call_count == count
    output = capsys.readouterr().out
    assert 'ABCD-EFGH' in output and 'Open the URL' in output
    assert 'private-' not in output
    assert store.data['public-client-id']['refresh_token'] == 'private-refresh-token'


@pytest.mark.parametrize('failure,match', [('access_denied', 'declined'), ('expired_token', 'expired'),
                                         ('device_flow_disabled', 'disabled')])
def test_login_errors(failure, match):
    obj, _, _, store = client([device(), {'error': failure}])
    with pytest.raises(RuntimeError, match=match):
        obj.token(True)
    assert not store.data


def test_login_deadline_and_cancellation():
    d = device()
    d['expires_in'] = 5
    obj, http, _, _ = client([d])
    with pytest.raises(RuntimeError, match='expired'):
        obj.token(True)
    assert http.call_count == 1
    obj, _, _, _ = client([device(), KeyboardInterrupt()])
    with pytest.raises(KeyboardInterrupt):
        obj.token(True)


def test_refresh_and_noninteractive_missing_auth():
    obj, http, timer, store = client([tokens()])
    with pytest.raises(RuntimeError, match='interactive terminal'):
        obj.token(False)
    http.assert_not_called()
    store.put('public-client-id', dict(access_token='expired', refresh_token='old',
                                     expires_at=1, refresh_expires_at=99999))
    assert obj.token(False) == 'private-access-token'
    assert http.call_args.kwargs['data']['grant_type'] == 'refresh_token'
    assert 'client_secret' not in http.call_args.kwargs['data']


def test_revoked_login_and_bad_refresh():
    obj, http, _, store = client([error(401), device(), tokens(), {'login': 'ethan'}])
    store.put('public-client-id', dict(access_token='revoked', expires_at=99999))
    assert obj.identity(True) == ('private-access-token', 'ethan')
    obj, http, _, store = client([error(401)])
    store.put('public-client-id', dict(access_token='revoked', expires_at=99999))
    with pytest.raises(RuntimeError, match='revoked'):
        obj.identity(False)
    assert not store.data
    obj, http, _, store = client([{'error': 'bad_refresh_token'}])
    store.put('public-client-id', dict(access_token='expired', refresh_token='old',
                                     expires_at=1, refresh_expires_at=99999))
    with pytest.raises(RuntimeError, match='interactive terminal'):
        obj.token(False)
    assert not store.data


def test_environment_auth_and_status_logout(monkeypatch, capsys):
    obj, http, _, store = client([])
    monkeypatch.setenv('GITHUB_TOKEN', 'environment-secret')
    assert obj.token() == 'environment-secret'
    assert 'environment' in obj.status()
    store.put('public-client-id', tokens())
    obj.logout()
    assert not store.data
    assert 'unset GITHUB_TOKEN' in capsys.readouterr().out
    http.assert_not_called()


def test_keyring_unavailable_never_writes_plaintext(capsys):
    store = auth.CredentialStore()
    store.backend = Mock()
    store.backend.get_password.side_effect = RuntimeError('locked')
    store.backend.set_password.side_effect = RuntimeError('locked')
    assert store.get('client') is None
    store.put('client', tokens())
    assert store.get('client') == tokens()
    assert 'only for this session' in capsys.readouterr().out
    with pytest.raises(RuntimeError, match='Could not remove'):
        store.delete('client')


class FakeAPI:
    repo = 'org/repo'

    def __init__(self):
        self.release = dict(id=1, draft=False, immutable=False)
        self.files = {}
        self.serial = 0
        self.fail = None
        self.promoted = False
        self.drafts = []

    def assets(self, release):
        return [dict(value, id=k) for k, value in self.files.items()]

    def upload(self, release, name, data):
        if self.fail == 'upload' and name.endswith('.tgz'):
            raise OSError('upload interrupted')
        if any(a['name'] == name for a in self.files.values()):
            raise error(422)
        self.serial += 1
        payload = data if isinstance(data, bytes) else data.read_bytes()
        value = dict(name=name, data=payload, size=len(payload), state='uploaded')
        self.files[self.serial] = value
        return dict(value, id=self.serial)

    def download(self, asset):
        return self.files[asset['id']]['data']

    def call(self, method, path, body=None):
        if method == 'GET' and path.startswith('/releases?'):
            return self.drafts
        if method == 'GET':
            return dict(self.release)
        if method == 'PATCH' and path == '/releases/1':
            if self.fail == 'promotion':
                raise RuntimeError('promotion failed')
            self.promoted = body
        elif method == 'PATCH':
            self.files[int(path.rsplit('/', 1)[1])]['name'] = body['name']
        elif method == 'DELETE':
            del self.files[int(path.rsplit('/', 1)[1])]
        else:
            raise AssertionError((method, path, body))

    def named(self, name):
        return next(dict(value, id=k) for k, value in self.files.items() if value['name'] == name)

    def seed(self, tmp, records):
        for r in records:
            self.upload(self.release, r['asset'], tmp / r['asset'])
        self.upload(self.release, artifacts.INDEX, json.dumps(dict(schema=1, artifacts=records)).encode())

    def index(self):
        return json.loads(self.named(artifacts.INDEX)['data'])['artifacts']


def test_partial_publication_merges_and_uses_unique_names(tmp_path):
    old = record(tmp_path, 'other')
    new = record(tmp_path)
    api = FakeAPI()
    api.seed(tmp_path, [old])
    url = p.publish(tmp_path, api, 'models-latest', records=[new])
    result = api.index()
    assert len(result) == 2 and result[0] == old
    assert result[1]['asset'] != new['asset']
    assert result[1]['identity'] == 'models/plant.slx'
    assert api.promoted and url.endswith('/models-latest')
    assert p.LOCK not in [x['name'] for x in api.assets(api.release)]
    assert any(x['name'].startswith('previous-index-') for x in api.assets(api.release))


@pytest.mark.parametrize('failure', ['upload', 'promotion'])
def test_failure_preserves_previous_release(tmp_path, failure):
    old = record(tmp_path, 'other')
    new = record(tmp_path)
    api = FakeAPI()
    api.seed(tmp_path, [old])
    api.fail = failure
    with pytest.raises((RuntimeError, OSError)):
        p.publish(tmp_path, api, 'models-latest', records=[new])
    assert api.index() == [old]
    assert sum(a['name'] == artifacts.INDEX for a in api.assets(api.release)) == 1
    assert p.LOCK not in [x['name'] for x in api.assets(api.release)]


def test_busy_lock_and_immutable_release(tmp_path):
    r = record(tmp_path)
    api = FakeAPI()
    lock = json.dumps(dict(owner='another-user', created_at='yesterday')).encode()
    api.upload(api.release, p.LOCK, lock)
    with pytest.raises(RuntimeError, match='another-user.*yesterday'):
        p.publish(tmp_path, api, 'models-latest', records=[r])
    assert api.named(p.LOCK)['data'] == lock
    api.release['immutable'] = True
    with pytest.raises(ValueError, match='immutable'):
        p.publish(tmp_path, api, 'models-latest', records=[r])


def test_publication_provenance_and_complete_sets(tmp_path):
    r = record(tmp_path)
    path = p.bundle(tmp_path, [r])
    index = artifacts.read_index(path)
    p.validate_local(index, tmp_path)
    r['dirty'] = True
    with pytest.raises(ValueError, match='modified'):
        p.validate_local(dict(artifacts=[r]), tmp_path)
    p.validate_local(dict(artifacts=[r]), tmp_path, allow_dirty=True)
    r['release_identity'] = r['identity']
    with pytest.raises(ValueError, match='Downloaded'):
        p.validate_local(dict(artifacts=[r]), tmp_path, True)
    r.pop('release_identity')
    r['dirty'] = False
    r['build_architectures'].append('arm64')
    with pytest.raises(ValueError, match='Incomplete'):
        p.validate_local(dict(artifacts=[r]), tmp_path)
    index['artifacts'] = []
    with pytest.raises(ValueError):
        p.validate_local(index, tmp_path)


def test_bad_checksums_duplicates_package_collision(tmp_path):
    r = record(tmp_path)
    with pytest.raises(ValueError, match='Duplicate'):
        p.validate_records([r, r])
    other = dict(r, identity='other.slx', repository_identity='other.slx')
    with pytest.raises(ValueError, match='ownership'):
        p.validate_records([r, other])
    r['sha256'] = '0' * 64
    with pytest.raises(ValueError, match='checksum'):
        p.validate_local(dict(artifacts=[r]), tmp_path)


def test_cli_multi_repository_and_retry(tmp_path, capsys):
    records = [record(tmp_path), record(tmp_path, 'other', repo='org/second')]
    path = p.bundle(tmp_path, records)
    args = cli.arguments(['publish', '--index', str(path), '--non-interactive'])
    with patch.object(cli, 'Auth') as login, patch.object(p, 'publish') as publish:
        login.return_value.identity.return_value = ('token', 'user')
        cli.execute(args)
        assert publish.call_count == 2
        assert [c.args[1].repo for c in publish.call_args_list] == ['org/repo', 'org/second']
        publish.side_effect = OSError('offline')
        with pytest.raises(OSError):
            cli.execute(args)
        assert f'--index {path}' in capsys.readouterr().err

    assert path.exists() and (tmp_path / records[0]['asset']).exists()


def test_no_login_when_skipped_and_successful_interactive_offer(tmp_path):
    r = record(tmp_path)
    args = cli.arguments(['build', 'plant', '--archives-dir', str(tmp_path)])
    with patch.object(cli, 'Auth') as login, patch.object(cli, 'can_interact', return_value=True), \
            patch.object(cli.tui, 'choose', return_value=[1]):
        cli.offer_publication(args, [r])
        login.assert_not_called()

    with patch.object(cli, 'Auth') as login, patch.object(p, 'publish') as publish, \
            patch.object(cli, 'can_interact', return_value=True), \
            patch.object(cli.tui, 'choose', return_value=[0]):
        login.return_value.identity.return_value = ('token', 'user')
        cli.offer_publication(args, [r])
        publish.assert_called_once()
        login.return_value.identity.assert_called_once_with(True)

    args.publish = False
    with patch.object(cli, 'Auth') as login:
        cli.offer_publication(args, [r])
        login.assert_not_called()


def test_no_archive_preserves_retry_copy(tmp_path):
    temporary = tmp_path / 'temporary'
    temporary.mkdir()
    saved = tmp_path / 'saved'
    r = record(temporary)
    args = cli.arguments(['build', 'plant', '--publish', '--archives-dir', str(temporary)])
    args.publication_cache = saved
    with patch.object(cli, 'publish_index', side_effect=RuntimeError('authentication unavailable')):
        with pytest.raises(RuntimeError):
            cli.offer_publication(args, [r])

    assert (saved / r['asset']).exists()
    assert args.index.parent == saved


def test_default_build_and_fallback_never_authenticate(tmp_path):
    models = tmp_path / 'models'
    models.mkdir()
    (models / 'plant.slx').touch()
    args = cli.arguments(['build', 'plant', '--workspace', str(tmp_path), '--no-process-local',
                          '--no-process-deploy', '--non-interactive'])
    with patch.object(cli, 'Auth') as login, patch.object(backend, 'generate', return_value=[record(tmp_path)]):
        cli.execute(args)
        login.assert_not_called()

    args.publish = True
    with patch.object(cli, 'Auth') as login, \
            patch.object(backend, 'generate', side_effect=backend.GenerationError('license')), \
            patch.object(backend, 'download', return_value=[]):
        with pytest.raises(ValueError, match='release sources'):
            cli.execute(args)
        login.assert_not_called()


def test_redirect_strips_credentials_and_blocks_untrusted_hosts():
    import urllib.request
    req = urllib.request.Request('https://api.github.com/releases/1', headers={'Authorization': 'Bearer secret'})
    redirects = Redirects()
    target = redirects.redirect_request(req, None, 302, '', {}, 'https://release-assets.githubusercontent.com/file')
    assert target.get_header('Authorization') is None
    with pytest.raises(ValueError):
        redirects.redirect_request(req, None, 302, '', {}, 'https://attacker.test/file')
    req = urllib.request.Request('https://github.com/login/oauth/access_token', data=b'secret')
    with pytest.raises(ValueError):
        redirects.redirect_request(req, None, 302, '', {}, 'https://github.com/other')


def test_asset_pagination():
    api = API('org/repo', 'secret')
    with patch.object(api, 'call', side_effect=[[{}] * 100, [{'name': 'last'}]]) as call:
        assert len(api.assets({'id': 1})) == 101
        assert call.call_args.args[1].endswith('page=2')


def test_remote_replacement_keeps_other_models(tmp_path):
    old = record(tmp_path)
    other = record(tmp_path, 'other')
    updated = dict(old, source_commit='b' * 40, generation_id='next-build')
    api = FakeAPI()
    api.seed(tmp_path, [old, other])
    p.publish(tmp_path, api, 'models-latest', records=[updated])
    assert len(api.index()) == 2
    assert next(r for r in api.index() if r['model'] == 'plant')['source_commit'] == 'b' * 40
    assert next(r for r in api.index() if r['model'] == 'other') == other


def test_lost_patch_response_rolls_back_actual_remote_state(tmp_path):
    old = record(tmp_path, 'other')
    new = record(tmp_path)
    api = FakeAPI()
    api.seed(tmp_path, [old])
    real = api.call
    failed = False

    def call(method, path, body=None):
        nonlocal failed
        result = real(method, path, body)
        if method == 'PATCH' and body.get('name', '').startswith('previous-index-') and not failed:
            failed = True
            raise OSError('PATCH succeeded, response lost')
        return result

    api.call = call
    with pytest.raises(OSError):
        p.publish(tmp_path, api, 'models-latest', records=[new])
    assert api.index() == [old]
    assert p.LOCK not in [a['name'] for a in api.assets(api.release)]


def test_rollback_failure_retains_lock_for_admin_recovery(tmp_path):
    old = record(tmp_path, 'other')
    new = record(tmp_path)
    api = FakeAPI()
    api.seed(tmp_path, [old])
    api.fail = 'promotion'
    original = api.call

    def call(method, path, body=None):
        if method == 'PATCH' and body.get('name', '').startswith('failed-index-'):
            raise OSError('rollback failed')
        return original(method, path, body)

    api.call = call
    with pytest.raises(RuntimeError, match='Administrator recovery'):
        p.publish(tmp_path, api, 'models-latest', records=[new])
    assert api.named(p.LOCK)
    assert any(a['name'].startswith('previous-index-') for a in api.assets(api.release))


def test_cancelled_upload_preserves_index_and_releases_lock(tmp_path):
    old = record(tmp_path, 'other')
    new = record(tmp_path)
    api = FakeAPI()
    api.seed(tmp_path, [old])
    original = api.upload

    def upload(release, name, data):
        if name.endswith('.tgz'):
            raise KeyboardInterrupt
        return original(release, name, data)

    api.upload = upload
    with pytest.raises(KeyboardInterrupt):
        p.publish(tmp_path, api, 'models-latest', records=[new])
    assert api.index() == [old]
    assert p.LOCK not in [a['name'] for a in api.assets(api.release)]


def test_new_release_is_created_as_draft_on_remote_default_branch(tmp_path):
    api = FakeAPI()
    original = api.call
    created = []

    def call(method, path, body=None):
        if method == 'GET' and path.startswith('/releases/tags/') and not created:
            raise error(404)
        if method == 'GET' and path == '':
            return {'default_branch': 'dev'}
        if method == 'POST' and path == '/releases':
            created.append(body)
            return dict(api.release, draft=True)
        return original(method, path, body)

    api.call = call
    p.publish(tmp_path, api, 'models-latest', records=[record(tmp_path)])
    assert created[0]['draft'] is True and created[0]['target_commitish'] == 'dev'
    assert api.promoted


def test_dual_architecture_validation_and_malformed_selection(tmp_path):
    records = [record(tmp_path, arch=a) for a in ('x86_64', 'arm64')]
    for r in records:
        r['build_architectures'] = ['x86_64', 'arm64']
    p.validate_local({'artifacts': records}, tmp_path)
    records[1]['source_commit'] = 'b' * 40
    with pytest.raises(ValueError, match='different source'):
        p.validate_local({'artifacts': records}, tmp_path)
    index = {'schema': 1, 'artifacts': records, 'publication': {'expected': [123]}}
    with pytest.raises(ValueError, match='selection'):
        artifacts.validate_index(index)
    records[0]['asset'] = '../escape.tgz'
    with pytest.raises(ValueError):
        p.validate_local({'artifacts': records}, tmp_path)


def test_downloader_paginates_asset_lists():
    client = backend.GitHub()
    with patch.object(client, 'get', side_effect=[{'id': 9}, [{}] * 100, [{'name': 'last'}]]) as get:
        data = client.release('org/repo', 'latest')
    assert len(data['assets']) == 101
    assert get.call_args.args[0].endswith('page=2')


def test_refresh_http_rejection_and_memory_login_survives_new_auth_object():
    obj, _, _, store = client([error(401)])
    store.put('public-client-id', dict(access_token='expired', refresh_token='old',
                                     expires_at=1, refresh_expires_at=99999))
    with pytest.raises(RuntimeError, match='interactive terminal'):
        obj.token(False)
    assert not store.data
    store.put('public-client-id', dict(access_token='remembered', expires_at=999999999999))
    with patch.object(auth, '_store', store):
        assert auth.Auth().token(False) == 'remembered'


def test_published_sources_download_round_trip(tmp_path, capsys):
    r = dict(record(tmp_path), source_branch='mpc', built_by='ethan@desk')
    api = FakeAPI()
    p.publish(tmp_path, api, 'models-latest', records=[r])
    workspace = tmp_path / 'workspace'
    models = workspace / 'consumer/models'
    models.mkdir(parents=True)
    (models / 'plant.slx').touch()
    model = next(iter(cli.discover(workspace)))
    model.settings['github'] = {'repository': 'org/repo'}

    class Releases:
        def release(self, repo, tag, channel):
            return dict(tag_name='models-latest', published_at='2026-10-21T04:17:35Z',
                        assets=[dict(name=a['name'], url=str(a['id']))
                                                        for a in api.assets(api.release)])

        def get(self, url, binary=False):
            return api.files[int(url)]['data']

    cache = tmp_path / 'download'
    downloaded = backend.download([model], ['x86_64'], cache, 'R2023a', 'humble', github=Releases())
    assert downloaded[0]['origin'] == 'release'
    fields = dict(' '.join(line.split()).split(': ', 1) for line in capsys.readouterr().out.splitlines()
                  if line.startswith('  ') and ': ' in line)
    assert fields['Release'] == 'org/repo@models-latest, published 2026-10-21 04:17 UTC'
    assert (fields['Branch'], fields['Commit']) == ('mpc', r['source_commit'])
    assert fields['Built'] == '2026-10-19 12:00 UTC by ethan@desk'
    assert fields['Your checkout'].endswith('(different commit)')
    assert artifacts.digest(cache / downloaded[0]['asset']) == r['sha256']
    assert artifacts.extract(cache / downloaded[0]['asset'], tmp_path / 'extracted')['plant'].is_dir()
    with pytest.raises(ValueError, match='Downloaded'):
        p.validate_local({'artifacts': downloaded}, cache)


def test_environment_installation_token_does_not_require_user_endpoint(monkeypatch):
    monkeypatch.setenv('GITHUB_TOKEN', 'automation-token')
    obj, _, _, _ = client([error(403)])
    assert obj.identity(False) == ('automation-token', 'automation (environment token)')


def test_failed_first_publication_draft_is_reused_and_test_tags_never_become_latest(tmp_path):
    api = FakeAPI()
    original = api.call
    api.drafts = [dict(api.release, draft=True, tag_name='models-test')]

    def call(method, path, body=None):
        if method == 'GET' and path.startswith('/releases/tags/'):
            raise error(404)
        assert not (method == 'POST' and path == '/releases'), 'a second draft was created'
        return original(method, path, body)

    api.call = call
    p.publish(tmp_path, api, 'models-test', records=[record(tmp_path)])
    assert api.promoted['make_latest'] == 'false' and api.promoted['draft'] is False


def test_check_reports_identity_write_access_and_app_installation():
    responses = {'https://api.github.com/user': {'login': 'octo'},
                 'https://api.github.com/repos/org/repo': {'permissions': {'push': True}},
                 'https://api.github.com/user/installations': {'installations': [{'id': 7}]},
                 'https://api.github.com/user/installations/7/repositories?per_page=100':
                     {'repositories': [{'full_name': 'org/other'}]}}
    obj, _, _, store = client([])
    obj.http = Mock(side_effect=lambda url, **_: responses[url])
    store.data['public-client-id'] = dict(access_token='saved', expires_at=2 ** 62)
    with patch.dict('os.environ', {}, clear=True):
        report = obj.check('org/repo')
    assert 'Signed in as octo (GitHub App)' in report and 'org/repo: write access' in report
    assert 'App installed on repository: NO' in report and 'Publishing will fail' in report


def test_login_pins_the_code_in_the_panel_until_authorized():
    pinned = []
    obj, _, _, _ = client([device(), tokens()])
    with patch.object(auth.tui, 'attention', side_effect=pinned.append):
        obj.token(True)
    assert pinned == ['Enter code ABCD-EFGH at https://github.com/login/device', '']


def test_latest_downloads_prefer_the_channel_tag_over_githubs_latest():
    requested = []

    def get(url, binary=False):
        requested.append(url)
        if url.endswith('/releases/tags/models-latest') and 'missing' in url:
            raise error(404)
        if url.endswith('/releases/tags/v9'):
            raise error(404)
        return [] if '/assets' in url else dict(id=1, tag_name=url.rsplit('/', 1)[1])

    github = backend.GitHub()
    github.get = get
    assert github.release('org/repo', 'latest')['tag_name'] == 'models-latest'
    assert github.release('org/missing', 'latest')['tag_name'] == 'latest'  # no channel yet: GitHub's latest
    assert github.release('org/repo', 'latest', 'controller-models')['tag_name'] == 'controller-models'
    with pytest.raises(ValueError, match='No release v9'):
        github.release('org/repo', 'v9')  # explicit tags never fall back
    assert not any(url.endswith('/releases/latest') for url in requested if 'org/repo' in url)


def test_release_body_lists_each_build_newest_first(tmp_path):
    api = FakeAPI()
    old = dict(record(tmp_path, name='old'), source_branch='master', built_by='team@lab',
               generated_at='2026-10-01T10:00:00.123456+00:00', source_tag='v2.0.2', generation_id='1')
    new = dict(record(tmp_path), source_branch='mpc-map-frames', built_by='ethan@desk',
               generated_at='2026-10-20T23:37:12.5-04:00', dirty=True, generation_id='2')
    p.publish(tmp_path, api, 'models-latest', records=[old, new], allow_dirty=True)

    body = api.promoted['body']
    commit = new['source_commit']
    assert body.index('### plant') < body.index('### old')
    assert '⚠️ **Dev build**' in body and '🏷️ **Tagged build** (`v2.0.2`)' in body
    assert f'- **Commit:** [`{commit[:7]}`](https://github.com/org/repo/commit/{commit}) on `mpc-map-frames`' in body
    assert '- **Built:** 2026-10-21 03:37 UTC by `ethan@desk`' in body
    assert '- **Architectures:** x86_64 · MATLAB R2023a · ROS 2 humble' in body
    assert api.promoted['make_latest'] == 'true'


def test_build_captures_branch_tag_and_builder(tmp_path):
    import subprocess
    run = lambda *a: subprocess.run(['git', '-C', str(tmp_path), *a], check=True, capture_output=True)
    run('init', '-q', '-b', 'feature')
    run('-c', 'user.name=t', '-c', 'user.email=t@t', 'commit', '-q', '--allow-empty', '-m', 'x')
    run('tag', 'v1.2')
    (tmp_path / 'models').mkdir()
    (tmp_path / 'models' / 'plant.slx').touch()
    request = next(iter(cli.discover(tmp_path))).request()
    assert request['source_branch'] == 'feature' and request['source_tag'] == 'v1.2'
    assert '@' in request['built_by'] and request['dirty'] is True  # the untracked model counts, like Titan


def test_build_tag_reads_like_titan_version(tmp_path):
    x86 = dict(record(tmp_path), source_branch='master', built_by='ethan@desk', source_tag='v2.0.2')
    arm = dict(record(tmp_path, arch='arm64'), dirty=True)
    assert p.build_tag([x86, arm]) == ['plant', 'master/aaaaaaa-dirty (v2.0.2)', '2026-10-19 12:00 UTC',
                                      'ethan@desk', 'arm64+x86_64']

    # older indexes have no branch or builder
    assert p.build_tag([record(tmp_path)])[1:4] == ['aaaaaaa', '2026-10-19 12:00 UTC', '']


def test_missing_release_explains_itself():
    github = backend.GitHub()
    github.get = Mock(side_effect=error(404))
    with pytest.raises(ValueError, match='No models-latest release in org/new yet'):
        github.release('org/new', 'latest')


def built(tmp, name, generation):
    # a record named like collect() names local archives
    r = record(tmp, name)
    path = tmp / f'{name}-R2023a-humble-x86_64-{generation:016x}.tgz'
    (tmp / r['asset']).rename(path)
    return dict(r, asset=path.name, generation_id=f'{name}-{generation}')


def test_publish_prunes_only_the_pushed_models_older_archives(tmp_path):
    api = FakeAPI()
    api.upload(api.release, 'notes.txt', b'hand attached')
    api.upload(api.release, 'plant-x86_64.tgz', b'not ours')
    for name, generation in [('plant', 0), ('other', 0), ('other', 1), ('plant', 1), ('plant', 2), ('plant', 3)]:
        p.publish(tmp_path, api, 'models-latest', records=[built(tmp_path, name, generation)])

    names = {a['name'] for a in api.files.values()}
    plant = sorted(n.split('-')[4] for n in names if n.startswith('plant-R2023a'))
    other = sorted(n.split('-')[4] for n in names if n.startswith('other-R2023a'))
    assert plant == [f'{2:016x}', f'{3:016x}']  # current and previous
    assert other == [f'{0:016x}', f'{1:016x}']  # another model's history is left alone
    assert {'notes.txt', 'plant-x86_64.tgz', artifacts.INDEX} <= names
    assert len([n for n in names if n.endswith('index.json') or 'index-' in n]) == 2
    assert p.LOCK not in names


def test_failed_pruning_does_not_fail_publication(tmp_path):
    class Stubborn(FakeAPI):
        def call(self, method, path, body=None):
            if method == 'DELETE' and self.files[int(path.rsplit('/', 1)[1])]['name'].endswith('.tgz'):
                raise error(500)
            return super().call(method, path, body)

    api, messages = Stubborn(), []
    for generation in range(3):
        url = p.publish(tmp_path, api, 'models-latest', records=[built(tmp_path, 'plant', generation)],
                        progress=messages.append)
    assert url.endswith('/models-latest')
    assert any('Could not remove 1 old release assets' in m for m in messages)
    assert p.LOCK not in {a['name'] for a in api.files.values()}


def test_downloads_use_the_saved_login_without_prompting(monkeypatch):
    auth = Mock()
    auth.return_value.token.return_value = 'saved-token'
    monkeypatch.setattr(backend, 'Auth', auth)
    assert real_saved_token() == 'saved-token'
    auth.return_value.token.assert_called_once_with(interactive=False)

    auth.return_value.token.side_effect = RuntimeError('GitHub sign-in is needed')
    assert real_saved_token() is None


def test_rejected_login_retries_anonymously(capsys):
    sent = []

    class Opener:
        def open(self, request, timeout):
            sent.append(request.get_header('Authorization'))
            if request.get_header('Authorization'):
                raise error(401)
            response = Mock()
            response.__enter__ = Mock(return_value=Mock(read=Mock(return_value=b'{"id": 1}')))
            response.__exit__ = Mock(return_value=False)
            return response

    github = backend.GitHub('stale-token')
    with patch.object(backend.urllib.request, 'build_opener', return_value=Opener()):
        assert github.get('https://api.github.com/repos/org/repo') == {'id': 1}
        github.get('https://api.github.com/repos/org/repo')
    assert sent == ['Bearer stale-token', None, None]
    assert 'continuing without it' in capsys.readouterr().out
