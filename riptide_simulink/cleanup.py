"""Cleanup restricted to recorded outputs; never glob arbitrary model sources."""
import json
import os
from pathlib import Path
import shutil
import subprocess

from .artifacts import INDEX, RECEIPT, atomic_json, read_index

CACHE_MARKER = '.riptide-simulink-cache'


def safe_path(path, root):
    path, root = Path(os.path.abspath(path)), Path(os.path.abspath(root))
    if path == root or not path.is_relative_to(root):
        raise ValueError(f'Cleanup path escapes managed root: {path}')

    # no symlinks between the root and the target
    current = path
    while current != root:
        if current.is_symlink():
            raise ValueError(f'Refusing cleanup through symlink: {current}')
        current = current.parent

    # never delete anything git knows about
    if path.exists():
        if path.is_dir() and next(path.rglob('.git'), None) is not None:
            raise ValueError(f'Refusing to delete a nested Git repository: {path}')
        location = path if path.is_dir() else path.parent
        args = ['git', '-C', str(location), 'ls-files', '--', '.' if path.is_dir() else path.name]
        result = subprocess.run(args, text=True, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        if result.returncode == 0 and result.stdout.strip():
            raise ValueError(f'Refusing to delete tracked files: {path}')
    return path


def plan(args):
    # find packages with ownership receipts, plus their colcon build/install dirs
    outputs = {}
    for destination in (args.local_dir, args.deploy_dir):
        root = Path(destination)
        if (args.exclude_local and root == args.local_dir) or (args.exclude_deploy and root == args.deploy_dir):
            continue
        if root.is_symlink():
            raise ValueError(f'Managed directory is a symlink: {root}')
        if not root.exists():
            continue

        for folder in root.iterdir():
            if folder.is_symlink():
                continue
            receipt = folder / RECEIPT
            if folder.is_dir() and receipt.is_file():
                record = json.loads(receipt.read_text())
                if folder.name not in record.get('packages', []):
                    raise ValueError(f'Invalid ownership receipt: {receipt}')
                outputs.setdefault(folder.name, []).append((folder, root))

                # Only standard isolated-colcon package directories, never merged install.
                workspace = root.parent.parent
                if root.parent.name == 'src':
                    for kind in ('build', 'install'):
                        if kind == 'install' and (workspace / kind / '.colcon_install_layout').exists():
                            if (workspace / kind / '.colcon_install_layout').read_text().strip() == 'merged':
                                print(f'Skipping merged install cleanup: {workspace / kind}')
                                continue
                        path = workspace / kind / folder.name
                        if path.exists():
                            outputs[folder.name].append((path, workspace / kind))

    # apply the package filters
    index = Path(args.archives_dir) / INDEX
    records = read_index(index)['artifacts'] if index.exists() else []
    names = set(outputs) | {p for a in records for p in a['packages']}
    requested = set(args.include or []) | set(args.ignore or [])
    if requested - names:
        raise ValueError('Unknown managed packages: ' + ', '.join(sorted(requested - names)))
    chosen = set(args.include) if args.include else names - set(args.ignore or [])

    paths = []
    for name in sorted(chosen):
        for path, root in outputs.get(name, []):
            if args.no_delete_packages and root in (Path(args.local_dir), Path(args.deploy_dir)):
                continue
            paths.append((path, root))

    # archives go only when all of their packages are selected
    remove_records = []
    if not args.exclude_archive and not args.no_delete_packages:
        for record in records:
            intersection = set(record['packages']) & chosen
            if intersection and intersection != set(record['packages']):
                raise ValueError('An archive contains multiple packages; select all its packages together')
            if intersection:
                paths.append((Path(args.archives_dir) / record['asset'], Path(args.archives_dir)))
                remove_records.append(record)

    # shared caches, only the ones we marked as ours
    if args.full_clean:
        if args.include or args.ignore:
            raise ValueError('--full-clean clears shared message/build caches and cannot use package filters')
        for kind in ('messages', 'builds'):
            cache_root = Path(args.archives_dir) / kind
            safe_path(cache_root, Path(args.archives_dir))
            if cache_root.exists():
                for cache in sorted(cache_root.iterdir()):
                    if cache.is_dir() and (cache / CACHE_MARKER).is_file():
                        paths.append((cache, cache_root))

    paths = list(dict.fromkeys(paths))
    for path, root in paths:
        safe_path(path, root)
    return sorted(chosen), paths, records, remove_records


def execute(args, confirm):
    names, paths, records, removed = plan(args)
    print('Packages: ' + (', '.join(names) or '(none)'))
    for path, _ in paths:
        print(f'  delete {path}')
    if args.dry_run:
        return

    unfiltered = not args.include and not args.ignore
    if unfiltered and args.yes and not args.all:
        raise ValueError('Unfiltered unattended cleanup requires --all --yes')
    if not args.yes and not confirm('Clean ALL managed packages?' if unfiltered else 'Delete these generated outputs?'):
        return

    for path, root in paths:
        safe_path(path, root)  # recheck immediately before deletion
        if path.is_dir():
            shutil.rmtree(path)
        else:
            path.unlink(missing_ok=True)
    if removed:
        atomic_json(Path(args.archives_dir) / INDEX,
                    dict(schema=1, artifacts=[a for a in records if a not in removed]))
