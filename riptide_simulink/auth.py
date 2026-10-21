"""On-demand GitHub App device login and Secret Service credential storage."""
import json
import os
import re
import shutil
import subprocess
import time
import urllib.error
import webbrowser

from .github import request
from . import tui

# The organization App's public client ID; not a secret.
DEFAULT_CLIENT_ID = 'Iv23liVPcmYLaOKXjqtu'
SERVICE = 'riptide_simulink.github'
REVOKE_URL = 'https://github.com/settings/apps/authorizations'
_store = None


def open_browser(url):
    """xdg-open with its output discarded, so browser noise can't corrupt the TUI."""
    opener = shutil.which('xdg-open')
    if opener and (os.environ.get('DISPLAY') or os.environ.get('WAYLAND_DISPLAY')):
        subprocess.Popen([opener, url], stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
                         stderr=subprocess.DEVNULL, start_new_session=True)
        return True
    return webbrowser.open(url)


def external_token():
    return os.environ.get('GITHUB_TOKEN') or os.environ.get('GH_TOKEN')


class CredentialStore:
    """Only use Secret Service, never arbitrary or plaintext keyring backends."""

    def __init__(self):
        self.memory = {}
        self.warned = False
        try:
            from keyring.backends.SecretService import Keyring
            import secretstorage

            class UnlockedSecretService(Keyring):
                def get_preferred_collection(self):
                    collection = secretstorage.get_default_collection(secretstorage.dbus_init())
                    if collection.is_locked():
                        raise RuntimeError('System keyring is locked')
                    return collection

            # Keyring's default backend may display an unlock dialog. Never do that
            # implicitly during an unattended command or a local-only account query.
            self.backend = UnlockedSecretService()
        except ImportError:
            self.backend = None

    def get(self, key):
        if key in self.memory:
            return self.memory[key]
        if self.backend:
            try:
                value = self.backend.get_password(SERVICE, key)
                return json.loads(value) if value else None
            except Exception:
                return None
        return None

    def put(self, key, value):
        self.memory[key] = value
        if self.backend:
            try:
                self.backend.set_password(SERVICE, key, json.dumps(value))
                return
            except Exception:
                pass

        # no keyring: keep it in memory and say so once
        if not self.warned:
            hint = '' if self.backend else ' (install python3-keyring)'
            print(f'Secure keyring unavailable{hint}: login is kept only for this session; '
                  'the next session may require browser sign-in.')
            self.warned = True

    def delete(self, key):
        self.memory.pop(key, None)
        if self.backend:
            try:
                if self.backend.get_password(SERVICE, key) is not None:
                    self.backend.delete_password(SERVICE, key)
            except Exception as error:
                raise RuntimeError('Could not remove saved login; unlock your system keyring '
                                   f'and retry. Revoke access at {REVOKE_URL}') from error


class Auth:
    def __init__(self, store=None, http=request, now=time.time, monotonic=time.monotonic,
                 sleep=None, browser=open_browser):
        global _store
        self.client_id = os.environ.get('RIPTIDE_SIMULINK_GITHUB_CLIENT_ID', DEFAULT_CLIENT_ID)
        if store is None and _store is None:
            _store = CredentialStore()
        self.store = store if store is not None else _store
        self.http, self.now, self.monotonic = http, now, monotonic
        self.sleep, self.browser = sleep or tui.wait, browser

    def _save(self, data):
        if not isinstance(data.get('access_token'), str) or not data['access_token']:
            raise RuntimeError('GitHub did not return an access token')
        saved = {k: data[k] for k in ('access_token', 'refresh_token') if k in data}
        # Apps with token expiration disabled return no expiry
        saved['expires_at'] = self.now() + int(data['expires_in']) if 'expires_in' in data else 2 ** 62
        saved['refresh_expires_at'] = self.now() + int(data.get('refresh_token_expires_in', 0))
        self.store.put(self.client_id, saved)
        return saved['access_token']

    def token(self, interactive=False):
        external = os.environ.get('GITHUB_TOKEN') or os.environ.get('GH_TOKEN')
        if external:
            return external

        if not self.client_id:
            raise RuntimeError('GitHub App is not configured. Ask the administrator to register '
                               'the App and set RIPTIDE_SIMULINK_GITHUB_CLIENT_ID; see docs/PUBLISHING.md.')

        # saved token if still good, otherwise try a refresh
        saved = self.store.get(self.client_id)
        if isinstance(saved, dict):
            if saved.get('expires_at', 0) > self.now() + 60:
                return saved['access_token']
            if saved.get('refresh_token') and saved.get('refresh_expires_at', 0) > self.now():
                try:
                    data = self.http('https://github.com/login/oauth/access_token', method='POST',
                                     data=dict(client_id=self.client_id, grant_type='refresh_token',
                                               refresh_token=saved['refresh_token']))
                except urllib.error.HTTPError as error:
                    if error.code not in (400, 401):
                        raise
                    data = {'error': 'bad_refresh_token'}
                if 'error' not in data:
                    return self._save(data)
                if data['error'] not in ('bad_refresh_token', 'invalid_grant', 'expired_token'):
                    raise RuntimeError(f'GitHub token refresh failed: {data["error"]}')
                self.store.delete(self.client_id)

        if not interactive:
            raise RuntimeError('GitHub sign-in is needed. Retry publishing in an interactive '
                               'terminal, or supply GITHUB_TOKEN for unattended publication.')
        return self.login()

    def login(self):
        data = self.http('https://github.com/login/device/code', method='POST',
                         data=dict(client_id=self.client_id, scope='offline_access'))
        if 'error' in data:
            raise RuntimeError(f'GitHub device login failed: {data["error"]}')
        url = data['verification_uri']
        if url != 'https://github.com/login/device':
            raise RuntimeError('Unexpected GitHub device verification URL')

        print(f'Sign in to GitHub: {url}\nEnter code: {data["user_code"]}\n'
              'Waiting for authorization (Ctrl-C cancels).', flush=True)
        try:
            opened = self.browser(url)
        except Exception:
            opened = False
        if not opened:
            print('Open the URL above in any browser to continue.', flush=True)
        tui.attention(f'Enter code {data["user_code"]} at {url}')
        try:
            return self._poll(data)
        finally:
            tui.attention('')

    def _poll(self, data):
        # poll until approved, declined, or expired
        deadline = self.monotonic() + int(data['expires_in'])
        interval = max(1, int(data['interval']))
        while self.monotonic() + interval < deadline:
            self.sleep(interval)
            result = self.http('https://github.com/login/oauth/access_token', method='POST',
                               data=dict(client_id=self.client_id, device_code=data['device_code'],
                                         grant_type='urn:ietf:params:oauth:grant-type:device_code'))
            error = result.get('error')
            if not error:
                return self._save(result)
            if error == 'authorization_pending':
                continue
            if error == 'slow_down':
                interval = max(interval + 5, int(result.get('interval', 0)))
                continue
            if error == 'access_denied':
                raise RuntimeError('GitHub sign-in was declined; local build is preserved.')
            if error in ('expired_token', 'token_expired'):
                break
            raise RuntimeError(f'GitHub device login failed: {error}')
        raise RuntimeError('GitHub sign-in expired; retry publishing to get a new device code.')

    def identity(self, interactive=False):
        """Check revoked credentials before any publication writes."""
        token = self.token(interactive)
        try:
            return token, self.http('https://api.github.com/user', token=token)['login']
        except urllib.error.HTTPError as error:
            if error.code == 403 and (os.environ.get('GITHUB_TOKEN') or os.environ.get('GH_TOKEN')):
                # Installation tokens have no /user identity. Their actual
                # repository access is checked by the publisher before any upload.
                return token, 'automation (environment token)'
            if error.code != 401:
                raise
            if os.environ.get('GITHUB_TOKEN') or os.environ.get('GH_TOKEN'):
                raise RuntimeError('The supplied GitHub token was rejected; replace it and retry.') from error
            self.store.delete(self.client_id)
            if not interactive:
                raise RuntimeError('Saved GitHub login was revoked. Retry publishing interactively.') from error
            token = self.login()
            return token, self.http('https://api.github.com/user', token=token)['login']

    def check(self, repo, interactive=False):
        """Sign in if needed and report whether uploads to repo would work; writes nothing."""
        if not re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+', repo or ''):
            raise ValueError(f'Invalid GitHub repository: {repo!r}; use owner/name')
        tui.pipeline([('auth', 'GitHub sign-in'), ('access', 'Repository access')])
        tui.stage('auth', 'running', 'Signing in')
        token, login = self.identity(interactive)
        tui.stage('auth', 'done', login)
        lines = [f'Signed in as {login}' + (' (environment token)' if external_token() else ' (GitHub App)')]

        # repository write access is the user's own permission
        tui.stage('access', 'running', repo)
        try:
            permissions = self.http(f'https://api.github.com/repos/{repo}', token=token).get('permissions', {})
        except urllib.error.HTTPError as error:
            if error.code != 404:
                raise
            permissions = None
        if permissions is None:
            lines.append(f'{repo}: not found or not visible to this login')
        else:
            lines.append(f'{repo}: ' + ('write access' if permissions.get('push') else
                                        'read-only; ask an organization owner for write access'))

        # App tokens also need the App installed on the repository
        if not external_token():
            installed = False
            for installation in self.http('https://api.github.com/user/installations', token=token)['installations']:
                repos = self.http(f'https://api.github.com/user/installations/{installation["id"]}/repositories'
                                  '?per_page=100', token=token)['repositories']
                installed |= any(r['full_name'].lower() == repo.lower() for r in repos)
            lines.append('App installed on repository: ' + ('yes' if installed else
                         'NO; an organization owner must add it to the App installation'))
            ready = bool(permissions and permissions.get('push')) and installed
        else:
            ready = bool(permissions and permissions.get('push'))
        tui.stage('access', 'done' if ready else 'failed')
        lines.append('Ready to publish.' if ready else 'Publishing will fail until the items above are fixed.')
        return '\n'.join(lines)

    def status(self):
        # Local-only: never trigger login or refresh just to show status.
        if os.environ.get('GITHUB_TOKEN') or os.environ.get('GH_TOKEN'):
            return 'GitHub credentials supplied by the environment (not verified).'
        saved = self.store.get(self.client_id) if self.client_id else None
        return ('Saved GitHub login available; verified when uploading.' if saved
                else 'No saved GitHub login. Sign-in is offered when uploading.')

    def logout(self):
        if self.client_id:
            self.store.delete(self.client_id)
        print(f'Local GitHub login removed. Revoke authorization at {REVOKE_URL}')
        if os.environ.get('GITHUB_TOKEN') or os.environ.get('GH_TOKEN'):
            print('Environment credentials remain active until you unset GITHUB_TOKEN/GH_TOKEN.')
