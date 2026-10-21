"""Workspace discovery and optional, relative-path model manifests."""
from dataclasses import dataclass, field
from pathlib import Path
import fnmatch
import getpass
import re
import socket
import subprocess

import yaml

# directories never searched for models
PRUNE = {'.git', 'build', 'install', 'log', 'slprj', '__pycache__', 'simulink_models',
         'controller_models', 'generated_models', 'codegen', 'validation', '.cache',
         'cache', '.venv', 'node_modules', 'simulink_model_archives', 'matlab_msg_gen',
         'matlab_msg_gen_ros2'}
ARCHES = {'x86': 'x86_64', 'amd64': 'x86_64', 'x86_64': 'x86_64',
          'arm': 'arm64', 'aarch64': 'arm64', 'arm64': 'arm64'}


class AmbiguousProjectError(ValueError):
    """Require a deliberate selection before choosing a generation context."""


def architecture(value):
    try:
        return ARCHES[value.lower()]
    except KeyError:
        raise ValueError(f'Unsupported architecture {value}; choose x86_64 or arm64')


def git(path, *args):
    result = subprocess.run(['git', '-C', str(path), *args], text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    return result.stdout.strip() if result.returncode == 0 else ''


def github_repo(remote):
    # owner/repo from an https or ssh remote url
    match = re.fullmatch(r'(?:https://github.com/|git@github.com:|ssh://git@github.com/)([^/]+/[^/]+?)(?:\.git)?/?',
                         remote)
    return match.group(1) if match else ''


def walk(root):
    import os
    for directory, dirs, files in os.walk(root, followlinks=False):
        dirs[:] = sorted(d for d in dirs if d not in PRUNE and not d.endswith('_ert_rtw')
                         and not Path(directory, d).is_symlink())
        yield Path(directory), sorted(files)


@dataclass
class Model:
    path: Path
    workspace: Path
    models_root: Path
    settings: dict = field(default_factory=dict)
    excluded: bool = False

    @property
    def name(self):
        return self.path.stem

    @property
    def identity(self):
        return self.path.relative_to(self.workspace).as_posix()

    @property
    def repository(self):
        return (self.settings.get('github', {}).get('repository')
                or github_repo(git(self.path.parent, 'remote', 'get-url', 'origin')))

    @property
    def revision(self):
        return git(self.path.parent, 'rev-parse', 'HEAD') or 'unversioned'

    @property
    def branch(self):
        return git(self.path.parent, 'rev-parse', '--abbrev-ref', 'HEAD')

    @property
    def tag(self):
        return git(self.path.parent, 'describe', '--exact-match', '--tags', 'HEAD')

    @property
    def dirty(self):
        return bool(git(self.path.parent, 'status', '--porcelain', '--untracked-files=normal'))

    @property
    def repository_identity(self):
        root = git(self.path.parent, 'rev-parse', '--show-toplevel')
        return self.path.relative_to(root).as_posix() if root else self.identity

    def request(self):
        settings = dict(self.settings)
        base = Path(settings.pop('_base', self.models_root))

        # explicit project, otherwise the nearest .prj going up toward the workspace
        project = settings.get('project')
        if project:
            project = (base / project).resolve()
            if not project.is_file():
                raise ValueError(f'Project not found: {project}')
        else:
            candidates = []
            parent = self.path.parent
            while parent.is_relative_to(self.workspace):
                candidates = sorted(parent.glob('*.prj'))
                if candidates or parent == self.workspace:
                    break
                parent = parent.parent
            if len(candidates) > 1:
                raise AmbiguousProjectError(f'Ambiguous MATLAB project for {self.identity}; set project in simulink.yaml')
            project = candidates[0] if candidates else ''

        messages = [(base / p).resolve() for p in settings.get('messages', [])]
        if any(not p.is_dir() or not (p / 'package.xml').is_file() for p in messages):
            raise ValueError('Each messages entry must be a ROS interface package directory')

        return dict(model=str(self.path), identity=self.identity, repository_identity=self.repository_identity,
                    project=str(project),
                    setup=settings.get('setup', ''), messages=[str(p) for p in messages],
                    configurations=settings.get('configurations', {}), config_base=str(base),
                    repository=self.repository, source_commit=self.revision,
                    dirty=self.dirty, source_branch=self.branch, source_tag=self.tag,
                    built_by=f'{getpass.getuser()}@{socket.gethostname()}')


def discover(workspace, manifest=None):
    workspace = Path(workspace).resolve()
    if not workspace.is_dir():
        raise ValueError(f'Workspace source directory not found: {workspace}')
    if manifest and not Path(manifest).is_file():
        raise ValueError(f'Manifest not found: {manifest}')

    results, manifests = [], {}
    for directory, files in walk(workspace):
        # only look inside models/ directories
        relative = directory.relative_to(workspace)
        if 'models' not in relative.parts and directory.name != 'models' and workspace.name != 'models':
            continue
        root = directory
        while root.name != 'models':
            root = root.parent

        # load and check each manifest once
        source = Path(manifest).resolve() if manifest else root / 'simulink.yaml'
        if source not in manifests:
            try:
                data = yaml.safe_load(source.read_text()) if source.exists() else {}
            except yaml.YAMLError as error:
                raise ValueError(f'Invalid YAML in {source}: {error}') from error
            data = data or {}
            if not isinstance(data, dict) or data.get('version', 1) != 1:
                raise ValueError(f'Invalid manifest: {source}')
            if not isinstance(data.get('defaults', {}), dict) or not isinstance(data.get('models', {}), dict):
                raise ValueError(f'Manifest defaults/models must be mappings: {source}')
            if not isinstance(data.get('exclude', []), list):
                raise ValueError(f'Manifest exclude must be a list: {source}')
            manifests[source] = data
        data = manifests[source]

        for name in files:
            path = directory / name
            if path.suffix not in ('.slx', '.mdl') or path.is_symlink():
                continue

            # defaults, then the entry by relative path or by name
            rel = path.relative_to(root).as_posix()
            settings = dict(data.get('defaults', {}))
            entries = data.get('models', {})
            entry = entries.get(rel, entries.get(path.stem, {})) or {}
            if not isinstance(entry, dict):
                raise ValueError(f'Model entry must be a mapping: {source}: {rel}')
            settings.update(entry)

            if (not isinstance(settings.get('messages', []), list)
                    or not isinstance(settings.get('configurations', {}), dict)
                    or not isinstance(settings.get('github', {}), dict)):
                raise ValueError(f'Invalid messages/configurations/github settings in {source}')
            if any(not isinstance(v, str) for k, v in settings.get('github', {}).items()
                   if k in ('repository', 'release', 'publish_release')):
                raise ValueError(f'GitHub repository and release settings must be strings: {source}')
            if (any(not isinstance(p, str) for p in settings.get('messages', []))
                    or any(not isinstance(p, str) for p in data.get('exclude', []))):
                raise ValueError(f'Message paths and exclusion patterns must be strings: {source}')
            settings['_base'] = str(source.parent)

            # referenced models and exclude patterns are hidden by default, unless the entry says otherwise
            excluded = ('referenced_models' in path.parts
                        or any(fnmatch.fnmatch(rel, p) for p in data.get('exclude', [])))
            excluded = settings.get('exclude', excluded)
            results.append(Model(path, workspace, root, settings, excluded))

    return sorted(results, key=lambda m: m.identity)


def select(models, names=(), ignore=(), all_models=False):
    chosen = []
    for name in names:
        matches = [m for m in models if name in (m.name, m.identity, str(m.path))]
        if not matches:
            raise ValueError(f'No model named {name!r} in this workspace; the list command shows the available '
                             f'models (use the name without .slx, a workspace-relative path, or an absolute path)')
        if len(matches) != 1:
            candidates = ', '.join(m.identity for m in matches)
            raise ValueError(f'{name!r} matches {len(matches)} models ({candidates}); use a workspace-relative path')
        if matches[0] not in chosen:
            chosen.append(matches[0])

    if not names and (all_models or ignore):
        chosen = [m for m in models if not m.excluded]
    if ignore:
        excluded = select(models, ignore)
        chosen = [m for m in chosen if m not in excluded]

    if not chosen:
        raise ValueError('Select at least one model, or use --all')
    return chosen
