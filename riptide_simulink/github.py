"""Small GitHub HTTP client; no gh executable or application secret."""
import json
import re
import urllib.error
import urllib.parse
import urllib.request


class Redirects(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        parsed = urllib.parse.urlsplit(newurl)
        if (parsed.scheme != 'https' or parsed.hostname not in (
                'github.com', 'api.github.com', 'objects.githubusercontent.com',
                'release-assets.githubusercontent.com')
                or parsed.username or parsed.password or parsed.port not in (None, 443)):
            raise ValueError('Refusing unexpected GitHub redirect')
        if req.get_method() != 'GET':
            raise ValueError('Refusing redirect of a credential or upload request')

        # drop credentials when the host changes
        redirected = super().redirect_request(req, fp, code, msg, headers, newurl)
        if parsed.hostname != urllib.parse.urlsplit(req.full_url).hostname:
            redirected.remove_header('Authorization')
        return redirected


def request(url, *, method='GET', data=None, token=None, raw=False, length=None):
    parsed = urllib.parse.urlsplit(url)
    if (parsed.scheme != 'https' or parsed.hostname not in (
            'github.com', 'api.github.com', 'uploads.github.com')
            or parsed.username or parsed.password or parsed.port not in (None, 443)):
        raise ValueError('Refusing unexpected GitHub URL')

    headers = {'Accept': 'application/octet-stream' if raw else 'application/vnd.github+json',
               'User-Agent': 'riptide-simulink', 'X-GitHub-Api-Version': '2022-11-28'}
    if parsed.hostname == 'github.com':
        headers['Accept'] = 'application/json'
    if token:
        if parsed.hostname not in ('api.github.com', 'uploads.github.com'):
            raise ValueError('Refusing to send API credentials to this host')
        headers['Authorization'] = f'Bearer {token}'
    if isinstance(data, dict):
        data = json.dumps(data).encode()
        headers['Content-Type'] = 'application/json'
    elif data is not None:
        headers['Content-Type'] = 'application/octet-stream'
    if length is not None:
        headers['Content-Length'] = str(length)

    req = urllib.request.Request(url, data=data, headers=headers, method=method)
    with urllib.request.build_opener(Redirects()).open(req, timeout=60) as response:
        payload = response.read()
    return payload if raw else (json.loads(payload) if payload else None)


class API:
    def __init__(self, repo, token):
        if not re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+', repo):
            raise ValueError(f'Invalid GitHub repository: {repo}')
        self.repo = repo
        self.base = f'https://api.github.com/repos/{repo}'
        self.token = token

    def call(self, method, path, body=None):
        return request(self.base + path, method=method, data=body, token=self.token)

    def assets(self, release):
        assets, page = [], 1
        while True:
            batch = self.call('GET', f'/releases/{release["id"]}/assets?per_page=100&page={page}')
            assets.extend(batch)
            if len(batch) < 100:
                return assets
            page += 1

    def download(self, asset):
        return request(f'{self.base}/releases/assets/{asset["id"]}', token=self.token, raw=True)

    def upload(self, release, name, data):
        # Construct the URL ourselves instead of trusting metadata supplied by a server.
        url = (f'https://uploads.github.com/repos/{self.repo}/releases/{release["id"]}/assets?name='
               + urllib.parse.quote(name, safe=''))
        if isinstance(data, bytes):
            return request(url, method='POST', token=self.token, data=data, length=len(data))
        with data.open('rb') as stream:
            return request(url, method='POST', token=self.token, data=stream,
                           length=data.stat().st_size)
