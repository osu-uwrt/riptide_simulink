"""Versioned source archives, safe extraction, transactional installation."""
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import tarfile
import tempfile
import xml.etree.ElementTree as ET

PACKAGE = re.compile(r'^[a-z][a-z0-9_]*$')
INDEX = 'simulink-index.json'
RECEIPT = '.riptide-simulink.json'


def digest(path):
    with open(path, 'rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest() if hasattr(hashlib, 'file_digest') else _digest(stream)


def _digest(stream):
    # fallback for pythons without hashlib.file_digest
    h = hashlib.sha256()
    for chunk in iter(lambda: stream.read(1024 * 1024), b''):
        h.update(chunk)
    return h.hexdigest()


def atomic_json(path, data):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile('w', dir=path.parent, delete=False) as stream:
        tmp = Path(stream.name)
        json.dump(data, stream, indent=2)
        stream.write('\n')

    try:
        os.replace(tmp, path)
    finally:
        tmp.unlink(missing_ok=True)


def read_index(path):
    data = json.loads(Path(path).read_text())
    return validate_index(data)


def validate_index(data):
    if not isinstance(data, dict) or data.get('schema') != 1 or not isinstance(data.get('artifacts'), list):
        raise ValueError('Unsupported or malformed release index')

    fields = ('model', 'identity', 'architecture', 'matlab', 'ros', 'asset', 'sha256', 'source_commit', 'generated_at')
    for a in data['artifacts']:
        # required strings
        if not isinstance(a, dict) or any(not isinstance(a.get(k), str) or not a[k] for k in fields):
            raise ValueError('Invalid artifact metadata strings')

        # packages, asset name, checksum, architecture
        packages = a.get('packages')
        if (not isinstance(packages, list) or not packages
                or any(not isinstance(p, str) or not PACKAGE.fullmatch(p) for p in packages)
                or len(set(packages)) != len(packages)
                or Path(a['asset']).name != a['asset'] or not a['asset'].endswith('.tgz')
                or not re.fullmatch('[a-f0-9]{64}', a['sha256'])
                or a['architecture'] not in ('x86_64', 'arm64')):
            raise ValueError('Invalid artifact metadata')

        # optional fields
        for field in ('repository', 'repository_identity', 'origin', 'generation_id', 'publish_release',
                      'source_branch', 'source_tag', 'built_by'):
            if field in a and not isinstance(a[field], str):
                raise ValueError(f'Invalid artifact {field}')
        if 'dirty' in a and not isinstance(a['dirty'], bool):
            raise ValueError('Invalid source cleanliness metadata')
        if 'build_architectures' in a and (
                not isinstance(a['build_architectures'], list)
                or not a['build_architectures']
                or any(v not in ('x86_64', 'arm64') for v in a['build_architectures'])
                or len(set(a['build_architectures'])) != len(a['build_architectures'])):
            raise ValueError('Invalid captured architecture selection')

    publication = data.get('publication', {})
    if not isinstance(publication, dict):
        raise ValueError('Invalid publication metadata')
    if 'expected' in publication and (not isinstance(publication['expected'], list) or any(
            not isinstance(k, (list, tuple)) or len(k) != 5 or any(not isinstance(v, str) for v in k)
            for k in publication['expected'])):
        raise ValueError('Invalid captured publication selection')
    return data


def extract(archive, output, expected=None):
    """Only regular files/directories; no links, devices, traversal or duplicates."""
    try:
        return _extract(archive, output, expected)
    except (tarfile.TarError, ET.ParseError) as error:
        raise ValueError(f'Malformed source archive {archive}: {error}') from error


def _extract(archive, output, expected):
    output = Path(output)
    with tarfile.open(archive, 'r:gz') as tar:
        # vet every member before writing anything
        members = tar.getmembers()
        seen = set()
        total = 0
        for member in members:
            path = PurePosixPath(member.name)
            if (path.is_absolute() or '..' in path.parts or '.git' in path.parts or '\\' in member.name
                    or not (member.isfile() or member.isdir()) or str(path) in seen):
                raise ValueError(f'Unsafe archive member: {member.name}')
            seen.add(str(path))
            total += member.size
            if total > 4 * 1024**3 or len(members) > 100000:
                raise ValueError('Archive exceeds extraction limits')

        # copy contents out ourselves, only keeping the exec bit
        for member in members:
            target = output / member.name
            if member.isdir():
                target.mkdir(parents=True, exist_ok=True)
            else:
                target.parent.mkdir(parents=True, exist_ok=True)
                with tar.extractfile(member) as src, target.open('wb') as dst:
                    shutil.copyfileobj(src, dst)
                target.chmod(0o755 if member.mode & 0o111 else 0o644)

    # each top level dir must be a ROS package named after itself
    packages = {}
    for xml in output.rglob('package.xml'):
        name = ET.parse(xml).getroot().findtext('name', '').strip()
        if not PACKAGE.fullmatch(name) or name in packages:
            raise ValueError(f'Invalid or duplicate package name: {name}')
        if xml.parent.parent != output or xml.parent.name != name:
            raise ValueError('Archives must contain top-level directories named after their ROS packages')
        packages[name] = xml.parent

    if not packages or (expected is not None and set(packages) != set(expected)):
        raise ValueError('Archive package names do not match metadata')
    return packages


def workspace_packages(root, excluded):
    for directory, dirs, files in os.walk(root, followlinks=False):
        directory = Path(directory)
        if directory == excluded or 'COLCON_IGNORE' in files:
            dirs[:] = []
            continue

        dirs[:] = [name for name in dirs if name not in ('.git', 'build', 'install', 'log', 'slprj')
                   and not name.endswith('_ert_rtw') and not (directory / name).is_symlink()]
        if 'package.xml' in files:
            yield directory / 'package.xml'
            dirs[:] = []  # colcon does not discover nested packages inside a package


def install(artifacts, cache, destination, workspace_src):
    """Validate the complete selected set, then replace with rollback on failure."""
    original = Path(destination).absolute()
    managed = Path(workspace_src).absolute()

    # symlinks above the workspace are fine (e.g. a workspace on another drive)
    inside = [p for p in original.parents if p != managed and p.is_relative_to(managed)]
    if any(p.is_symlink() for p in (original, *inside)):
        raise ValueError(f'Refusing symlink installation root: {original}')
    destination, workspace_src = original.resolve(), Path(workspace_src).resolve()
    destination.mkdir(parents=True, exist_ok=True)

    # keep generated packages out of git
    ignored = destination / '.gitignore'
    if ignored.is_symlink():
        raise ValueError(f'Refusing symlink ignore file: {ignored}')
    if not ignored.exists():
        ignored.write_text('*\n')

    # colcon must ignore staging even when interrupted.
    work = Path(tempfile.mkdtemp(prefix='.transaction-', dir=destination))
    (work / 'COLCON_IGNORE').touch()
    planned = {}
    backups, installed = [], []
    try:
        # extract everything into staging first
        for i, artifact in enumerate(artifacts):
            archive = Path(cache) / artifact['asset']
            if digest(archive) != artifact['sha256']:
                raise ValueError(f'Checksum mismatch: {archive.name}')
            for name, path in extract(archive, work / str(i), artifact['packages']).items():
                if name in planned:
                    raise ValueError(f'Multiple selected models generate package {name}')
                planned[name] = (path, artifact)

        # Include existing legacy generated folders in collision detection.
        for xml in workspace_packages(workspace_src, destination):
            try:
                name = ET.parse(xml).getroot().findtext('name')
            except ET.ParseError:
                continue
            if name in planned:
                raise ValueError(f'Package collision: {name} already exists at {xml.parent}')

        # only replace packages we installed before, then write receipts
        for name, (path, artifact) in planned.items():
            target = destination / name
            if target.is_symlink():
                raise ValueError(f'Refusing symlink destination {target}')
            if target.exists():
                from .cleanup import safe_path
                safe_path(target, destination)
                receipt = target / RECEIPT
                if not receipt.is_file() or json.loads(receipt.read_text()).get('identity') != artifact['identity']:
                    raise ValueError(f'Refusing to replace unowned package {target}')
            atomic_json(path / RECEIPT, artifact)

        # swap in, keeping backups for rollback
        for name, (path, _) in planned.items():
            target = destination / name
            if target.exists():
                backup = work / ('backup-' + name)
                os.replace(target, backup)
                backups.append((backup, target))
            os.replace(path, target)
            installed.append(target)

        print(f'Installed {", ".join(planned)} into {destination}')
        return list(planned)
    except BaseException:
        for target in reversed(installed):
            shutil.rmtree(target)
        for backup, target in reversed(backups):
            os.replace(backup, target)
        raise
    finally:
        shutil.rmtree(work)
