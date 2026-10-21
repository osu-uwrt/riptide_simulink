"""Default-first application pages with grouped advanced options."""
from copy import deepcopy
from pathlib import Path

from . import cleanup, publishing, tui
from .artifacts import read_index
from .auth import Auth
from .discovery import discover
from .locking import manager_lock

ACTIONS = [('Build models', 'build'), ('Download release', 'download'),
           ('List available models', 'list'), ('List generated packages', 'packages'),
           ('Advanced options', 'advanced'), ('Quit', 'quit')]
TOOLS = {
    'Cached builds': [('Process cached packages', 'process-cached'),
                      ('Publish a cached local build', 'publish')],
    'Maintenance': [('Clean outputs', 'clean'), ('Delete generated packages', 'delete'),
                    ('Refresh messages', 'messages')],
    'MATLAB': [('Open model/project', 'open')],
}
RUN_LABELS = {'build': 'Build', 'download': 'Download', 'process-cached': 'Install cached packages',
              'clean': 'Clean', 'delete': 'Delete', 'messages': 'Refresh messages',
              'open': 'Open in MATLAB', 'publish': 'Publish'}
TEXT = {
    'source': 'Build source', 'arch': 'Architectures', 'no_build': 'Skip development compilation',
    'jobs': 'Maximum compiler jobs', 'local_config': 'Host configuration factory',
    'deploy_config': 'ARM64 configuration factory', 'user_startup': 'Run personal MATLAB startup',
    'matlab': 'MATLAB executable', 'matlab_release': 'MATLAB release', 'ros_distro': 'ROS distribution',
    'release': 'Download release tag', 'repository': 'GitHub repository override',
    'publish': 'Publication after local build', 'publish_release': 'Publication release tag',
    'allow_dirty': 'Allow marked modified-source publication', 'index': 'Saved publication index',
    'local_dir': 'Development output', 'deploy_dir': 'Release output', 'archives_dir': 'Archive/cache directory',
    'no_process_local': 'Skip development staging', 'no_process_deploy': 'Skip release staging',
    'no_archive': 'Use temporary archives', 'deploy': 'Deploy explicitly', 'deploy_target': 'Deployment target',
    'scope': 'Package selection', 'dry_run': 'Preview cleanup only',
    'full_clean': 'Clean shared message and build caches too',
    'no_delete_packages': 'Keep generated source packages and archives during cleanup',
    'exclude_local': 'Exclude development outputs from cleanup',
    'exclude_deploy': 'Exclude release outputs from cleanup', 'exclude_archive': 'Exclude archive cleanup',
    'workspace': 'Development source workspace', 'manifest': 'Manifest override'}
BOOL = {'no_build', 'user_startup', 'allow_dirty', 'no_process_local', 'no_process_deploy',
        'no_archive', 'deploy', 'dry_run', 'full_clean', 'no_delete_packages',
        'exclude_local', 'exclude_deploy', 'exclude_archive'}
PATHS = {'workspace', 'manifest', 'local_dir', 'deploy_dir', 'archives_dir', 'index'}
GROUPS = {
    'Build and MATLAB': ['source', 'arch', 'no_build', 'jobs', 'local_config', 'deploy_config',
                         'matlab', 'user_startup', 'matlab_release', 'ros_distro'],
    'GitHub and publishing': ['release', 'publish', 'publish_release', 'repository', 'allow_dirty', 'index'],
    'Output and deployment': ['local_dir', 'deploy_dir', 'archives_dir', 'no_process_local',
                              'no_process_deploy', 'no_archive', 'deploy', 'deploy_target'],
    'Cleanup': ['scope', 'dry_run', 'no_delete_packages', 'exclude_local', 'exclude_deploy',
                'exclude_archive', 'full_clean'],
    'Workspace': ['workspace', 'manifest'],
}


def configure(args, models):
    # publish: pick a saved index, newest first
    if args.command == 'publish':
        indexes = sorted(args.archives_dir.glob('publish-*.json'),
                         key=lambda p: p.stat().st_mtime, reverse=True)
        if not indexes:
            raise ValueError(f'No saved local builds in {args.archives_dir}; build models first')
        labels = saved_builds(indexes)
        while True:
            args.index = indexes[tui.choose('Choose a saved local build', labels)[0]]
            try:
                return review(args)
            except tui.Back:
                continue

    # cleanup: pick packages unless a scope is already set
    if args.command in ('clean', 'delete'):
        select_packages = not args.all and not args.include and not args.ignore
        while True:
            if select_packages:
                names, _, _, _ = cleanup.plan(args)
                if not names:
                    raise ValueError('No generated packages to clean')
                args.include = [names[i] for i in tui.choose('Packages to clean', names, multiple=True)]
                args.all, args.ignore = False, []
            try:
                return review(args)
            except tui.Back:
                if not select_packages:
                    raise

    # everything else: pick models
    primary = [m for m in models if not m.excluded]
    excluded = [m for m in models if m.excluded]
    candidates = primary + excluded
    while True:
        args.models = [candidates[i].identity for i in tui.models(
            'Choose models', [m.identity for m in primary], [m.identity for m in excluded],
            multiple=args.command != 'open')]
        try:
            return review(args)
        except tui.Back:
            continue


def saved_builds(indexes):
    """Menu labels for saved publication indexes, as aligned build tags."""
    rows = []
    for path in indexes:
        try:
            rows.append(publishing.build_tag(read_index(path)['artifacts']))
        except (OSError, ValueError, KeyError, TypeError):
            rows.append([path.name])

    # line up the columns, but don't pad the last one
    widths = [max(len(r[i]) for r in rows if len(r) > i) for i in range(max(map(len, rows)))]
    return ['  '.join(value.ljust(widths[i]) for i, value in enumerate(r)).rstrip() for r in rows]


def review(args):
    selected = ', '.join(Path(m).stem for m in args.models)
    if args.command in ('clean', 'delete'):
        selected = display(args, 'scope')
    if args.command == 'publish':
        selected = ' '.join(saved_builds([args.index])[0].split()) if args.index else 'saved build'

    while True:
        choice = tui.choose(f'{RUN_LABELS[args.command]}: {selected}',
                            [RUN_LABELS[args.command], 'Advanced options', 'Back'])[0]
        if choice == 0:
            return
        if choice == 2:
            raise tui.Back
        settings(args)


def relevant_fields(args):
    command = args.command
    if command is None:
        return set(TEXT)
    fields = {'workspace', 'manifest'}
    if command in ('build', 'messages', 'open'):
        fields.add('matlab')
    if command in ('build', 'messages'):
        fields |= {'user_startup', 'jobs', 'matlab_release', 'ros_distro'}
    if command in ('build', 'download', 'process-cached'):
        fields |= {'arch', 'no_build', 'jobs', 'matlab_release', 'ros_distro', 'local_dir',
                   'deploy_dir', 'archives_dir', 'no_process_local', 'no_process_deploy',
                   'no_archive', 'deploy', 'deploy_target'}
    if command == 'build':
        fields |= {'source', 'local_config', 'deploy_config', 'publish', 'publish_release', 'allow_dirty'}
    if command in ('build', 'download', 'publish'):
        fields.add('repository')
    if command in ('build', 'download'):
        fields.add('release')
    if command == 'publish':
        fields |= {'publish_release', 'allow_dirty', 'index'}
    if command in ('clean', 'delete'):
        fields |= set(GROUPS['Cleanup']) | {'local_dir', 'deploy_dir', 'archives_dir'}
    if command == 'messages':
        fields.add('archives_dir')
    return fields


def display(args, field):
    if field == 'scope':
        return ('all managed packages' if args.all else
                'all except ' + ', '.join(args.ignore) if args.ignore else
                ', '.join(args.include) or 'select packages')
    value = getattr(args, field)
    if field in BOOL:
        return 'yes' if value else 'no'
    if field == 'arch':
        return ' + '.join(value)
    if field == 'publish':
        return {None: 'ask after build', True: 'publish', False: 'skip'}[value]
    return str(value) if value is not None else '(default)'


def edit(args, field):
    if field in BOOL:
        setattr(args, field, not getattr(args, field))
        return

    # fixed choices
    if field in ('source', 'arch', 'publish'):
        choices = {
            'source': (['Auto: local then release', 'Local only', 'Release only'], ['auto', 'local', 'release']),
            'arch': (['x86_64 and ARM64', 'x86_64 only', 'ARM64 only'],
                     [['x86_64', 'arm64'], ['x86_64'], ['arm64']]),
            'publish': (['Ask after local build', 'Publish after local build', 'Skip publication'],
                        [None, True, False])}
        labels, values = choices[field]
        setattr(args, field, values[tui.choose(TEXT[field], labels)[0]])
        return

    # cleanup scope: all, only selected, or all except selected
    if field == 'scope':
        while True:
            mode = tui.choose('Package selection',
                              ['Select packages', 'All managed packages', 'All except selected packages'])[0]
            chosen = []
            try:
                if mode != 1:
                    names, _, _, _ = cleanup.plan(args)
                    chosen = [names[i] for i in tui.choose('Packages', names, multiple=True)]
            except tui.Back:
                continue
            args.include = chosen if mode == 0 else []
            args.ignore = chosen if mode == 2 else []
            args.all = mode == 1
            return

    # free text, checked for jobs and paths
    value = tui.prompt(TEXT[field], getattr(args, field) or '')
    if field == 'jobs':
        value = int(value)
        if value < 1:
            raise ValueError('Compiler jobs must be positive')
    elif field in PATHS:
        if not value and field in ('workspace', 'local_dir', 'deploy_dir', 'archives_dir'):
            raise ValueError('Output/workspace paths cannot be empty')
        value = Path(value).expanduser() if value else None
    setattr(args, field, value or None)


def settings(args):
    allowed = relevant_fields(args)
    groups = [(name, [f for f in fields if f in allowed]) for name, fields in GROUPS.items()]
    groups = [(name, fields) for name, fields in groups if fields]
    fields = [field for _, entries in groups for field in entries]

    selected = 0
    while True:
        try:
            labels = [(name, [f'{TEXT[f]}: {display(args, f)}' for f in entries])
                      for name, entries in groups] + [('', ['Back'])]
            selected = tui.sections('Advanced options', labels, selected)
            if selected == len(fields):
                return
        except tui.Back:
            return

        try:
            edit(args, fields[selected])
        except tui.Back:
            continue  # Editor -> same settings page, retaining the selected option.
        except ValueError as error:
            tui.page('Invalid setting', str(error))


def default_repository(defaults):
    if defaults.repository:
        return defaults.repository
    try:
        return next((m.repository for m in discover(defaults.workspace, defaults.manifest)
                     if m.repository and not m.excluded), '')
    except (ValueError, OSError):
        return ''


def account(defaults):
    while True:
        try:
            selected = tui.choose('GitHub account', ['Sign in and check repository access', 'Show login status',
                                                     'Sign out', 'Back'])[0]
            if selected == 3:
                return
            client = Auth()
            if selected == 0:
                repo = tui.prompt('Repository (owner/name)', default_repository(defaults))

                tui.operation('GitHub access check', lambda: print(client.check(repo, interactive=True)))
            elif selected == 1:
                tui.page('GitHub account', client.status())
            elif tui.confirm('Remove this machine’s saved GitHub login?'):
                client.logout()
                tui.page('Signed out', 'Local credentials removed. Revoke GitHub authorization at\n'
                         'https://github.com/settings/apps/authorizations\n'
                         'Environment tokens, if present, must be unset separately.')
        except tui.Back:
            return


def advanced_home(defaults, perform):
    groups = [(name, [title for title, _ in options]) for name, options in TOOLS.items()]
    groups += [('GitHub', ['GitHub account']), ('Preferences', ['Session defaults']), ('', ['Back'])]
    commands = [command for options in TOOLS.values() for _, command in options]

    # tool rows first, then account, defaults, back
    selected = 0
    while True:
        try:
            selected = tui.sections('Advanced options', groups, selected)
            if selected == len(commands) + 2:
                return
            if selected == len(commands):
                account(defaults)
            elif selected == len(commands) + 1:
                settings(defaults)
            else:
                perform(commands[selected])
        except tui.Back:
            return


def run(defaults):
    from .cli import execute

    def application(ui):
        def perform(command):
            try:
                # each action gets its own copy of the session defaults
                args = deepcopy(defaults)
                args.command, args.non_interactive, args.interactive = command, False, True
                if command not in ('build', 'publish'):
                    args.publish = None

                if command == 'packages':
                    names, _, _, _ = cleanup.plan(args)
                    tui.page('Generated packages', '\n'.join(names) or 'No managed packages yet.')
                    return
                models = ([] if command in ('publish', 'clean', 'delete')
                          else discover(args.workspace, args.manifest))
                if command == 'list':
                    tui.models('Available models', [m.identity for m in models if not m.excluded],
                               [m.identity for m in models if m.excluded], browse=True)
                    return

                configure(args, models)

                def operation():
                    if command == 'open':
                        execute(args)
                    else:
                        with manager_lock():
                            execute(args)

                ui.operation(RUN_LABELS[command], operation)
            except tui.Back:
                return
            except (ValueError, OSError, RuntimeError) as error:
                tui.page('Unable to continue', str(error))

        # home page
        while True:
            try:
                selected = tui.choose('Simulink model manager', [title for title, _ in ACTIONS])[0]
                command = ACTIONS[selected][1]
                if command == 'quit':
                    return
                if command == 'advanced':
                    advanced_home(defaults, perform)
                else:
                    perform(command)
            except (tui.Back, KeyboardInterrupt):
                return

    return tui.run(application)
