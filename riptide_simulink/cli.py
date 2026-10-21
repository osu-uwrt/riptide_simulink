"""ROS executable and standalone CLI; both use the same operations as the TUI."""
import argparse
from pathlib import Path
import platform
import shlex
import shutil
import subprocess
import sys
import tempfile

from .locking import manager_lock
from . import backend, cleanup, tui, publishing
from . import resources
from .auth import Auth
from .github import API
from .artifacts import install, read_index
from .discovery import architecture, discover, select

ROOT = Path.home() / 'osu-uwrt'

# legacy model_manager.py task names
ALIASES = {'generate_packages': 'build', 'download_packages': 'download',
           'process_cached': 'process-cached', 'delete_packages': 'delete',
           'clean_workspace': 'clean', 'refresh_custom_msg_support': 'messages', 'open_project': 'open'}
COMMANDS = ('list', 'build', 'download', 'process-cached', 'clean', 'delete', 'messages', 'open',
            'publish', 'account', 'packages')


def parser():
    p = argparse.ArgumentParser(description='Discover, generate and manage ROS 2 Simulink source packages.')
    p.add_argument('command', choices=COMMANDS, nargs='?', default=None)
    p.add_argument('models', nargs='*')
    p.add_argument('--workspace', type=Path)
    p.add_argument('--manifest', type=Path)

    # build and MATLAB
    p.add_argument('--source', choices=('auto', 'local', 'release'), default='auto')
    p.add_argument('--arch', nargs='+', type=architecture, default=['x86_64', 'arm64'])
    p.add_argument('--models-select', nargs='+')
    p.add_argument('--models-ignore', nargs='+', default=[])
    p.add_argument('--configs-select', nargs='+', help='Legacy aliases: x86_cfg, arm_cfg')
    p.add_argument('--configs-ignore', nargs='+', default=[])
    p.add_argument('--local-config', help='MATLAB config factory for the host architecture')
    p.add_argument('--deploy-config', help='MATLAB config factory for ARM64')
    p.add_argument('--all', action='store_true')
    p.add_argument('--yes', '-y', '--assume-yes', action='store_true')
    p.add_argument('--jobs', type=int, default=resources.available_cpus(),
                   help='Maximum compiler jobs (default: full available CPU count)')
    p.add_argument('--matlab', default='matlab')
    p.add_argument('--user-startup', action='store_true', help='Allow personal startup.m in batch MATLAB sessions')
    p.add_argument('--matlab-release', default='R2023a')
    p.add_argument('--ros-distro', default='humble')

    # github and publishing
    p.add_argument('--release', '--from-release', default=None)
    p.add_argument('--repository')
    publication = p.add_mutually_exclusive_group()
    publication.add_argument('--publish', dest='publish', action='store_true', default=None)
    publication.add_argument('--no-publish', dest='publish', action='store_false')
    p.add_argument('--publish-release', help='Publication tag, independent of --release downloads')
    p.add_argument('--index', type=Path, help='Captured publication index for publish retries')
    p.add_argument('--allow-dirty', action='store_true', help='Publish explicitly marked modified sources')
    mode = p.add_mutually_exclusive_group()
    mode.add_argument('--interactive', action='store_true', help='Open the persistent guided TUI')
    mode.add_argument('--non-interactive', action='store_true', help='Never prompt or open browser sign-in')

    # output and deployment
    p.add_argument('--archives-dir', type=Path, default=ROOT / 'simulink_model_archives')
    p.add_argument('--local-dir', type=Path, default=ROOT / 'development/software/src/simulink_models')
    p.add_argument('--deploy-dir', type=Path, default=ROOT / 'release/src/simulink_models')
    p.add_argument('--no-archive', action='store_true', help='Use temporary archives for this build/download')
    p.add_argument('--no-build', action='store_true')
    p.add_argument('--build', action='store_true', help='Legacy explicit build; building is already the default')
    p.add_argument('--no-process-local', action='store_true')
    p.add_argument('--no-process-deploy', action='store_true')
    p.add_argument('--deploy', action='store_true')
    p.add_argument('--deploy-target', default='orin')

    # cleanup
    filters = p.add_mutually_exclusive_group()
    filters.add_argument('--include', nargs='+', default=[])
    filters.add_argument('--ignore', nargs='+', default=[])
    p.add_argument('--full-clean', action='store_true')
    p.add_argument('--no-delete-packages', action='store_true')
    p.add_argument('--exclude-local', action='store_true')
    p.add_argument('--exclude-deploy', action='store_true')
    p.add_argument('--exclude-archive', action='store_true')
    p.add_argument('--dry-run', action='store_true')
    return p


def arguments(argv):
    argv = list(argv)

    # map legacy task names onto the new commands
    if argv and argv[0] in ALIASES:
        legacy = argv[0]
        argv[0] = ALIASES[legacy]
        if legacy == 'generate_packages' and '--source' not in argv:
            argv += ['--source', 'local']

    # bare model names mean build
    if argv and not argv[0].startswith('-') and argv[0] not in COMMANDS:
        argv.insert(0, 'build')

    args = parser().parse_args(argv)

    # the TUI only reuses the last workspace when none was given
    args.workspace_given = args.workspace is not None
    args.workspace = args.workspace or ROOT / 'development/software/src'
    return args


def execute(args):
    if args.jobs < 1:
        raise ValueError('--jobs must be positive')
    if args.publish is True and args.command not in ('build', 'publish', None):
        raise ValueError('--publish applies to local builds; use publish --index PATH for retries')

    # commands that don't touch models
    if args.command == 'account':
        account = Auth()
        action = args.models or ['status']
        if action == ['status']:
            print(account.status())
        elif action == ['logout']:
            account.logout()
        elif action[0] == 'check' and len(action) <= 2:
            repo = action[1] if len(action) == 2 else args.repository
            if not repo:
                raise ValueError('Use account check OWNER/REPO, or --repository')
            sign_in = getattr(args, 'sign_in', False) and sys.stdin.isatty() and sys.stdout.isatty()
            print(account.check(repo, interactive=sign_in))
        else:
            raise ValueError('Use account status, account check OWNER/REPO, or account logout')
        return

    if args.command == 'publish':
        if not args.index:
            raise ValueError('Specify publish --index PATH (the captured local build index)')
        return publish_index(args)

    if args.command == 'packages':
        names, _, _, _ = cleanup.plan(args)
        print('\n'.join(names) or 'No managed packages yet.')
        return

    # rerun with a throwaway archive dir, keeping the real one for publication
    if args.no_archive and args.command in ('build', 'download'):
        with tempfile.TemporaryDirectory(prefix='riptide-archives-') as temporary:
            args.publication_cache = args.archives_dir
            args.archives_dir = Path(temporary)
            args.no_archive = False
            return execute(args)

    if args.command in ('clean', 'delete'):
        if args.models:
            if args.include or args.ignore:
                raise ValueError('Use package positionals or --include/--ignore, not both')
            args.include = args.models

        def confirm_cleanup(question):
            if args.non_interactive:
                raise ValueError('CLI cleanup requires --yes; unfiltered cleanup requires --all --yes')
            if tui.active():
                names, paths, _, _ = cleanup.plan(args)
                tui.page('Cleanup preview', 'Packages: ' + (', '.join(names) or '(none)') + '\n' +
                         '\n'.join(str(path) for path, _ in paths), back_cancels=True)
            return tui.confirm(question)

        return cleanup.execute(args, confirm_cleanup)

    models = discover(args.workspace, args.manifest)
    if args.command is None:
        if args.non_interactive:
            raise ValueError('Specify a command when using --non-interactive')
        from .interactive import run
        return run(args)

    if args.command == 'list':
        for model in models:
            print(f'{model.identity}' + (' [excluded by default]' if model.excluded else ''))
        return

    # pick models, asking if none were given
    names = args.models_select or args.models
    if not names and not args.all and not args.models_ignore:
        if args.non_interactive:
            raise ValueError('Specify model names or --all when using --non-interactive')
        available = [m for m in models if not m.excluded]
        names = [available[i].identity
                 for i in tui.choose('Choose models', [m.identity for m in available], multiple=True)]
    chosen = select(models, names, args.models_ignore, args.all)

    # pick architectures; legacy config names select them too
    arches = list(dict.fromkeys(args.arch))
    if args.configs_select:
        arches = [architecture(c.removesuffix('_cfg')) for c in args.configs_select]
    arches = [a for a in arches if a not in [architecture(c.removesuffix('_cfg')) for c in args.configs_ignore]]
    if not arches:
        raise ValueError('No architectures selected')

    # config factory overrides
    for model in chosen:
        configurations = dict(model.settings.get('configurations', {}))
        if args.local_config:
            configurations[architecture(platform.machine())] = args.local_config
        if args.deploy_config:
            configurations['arm64'] = args.deploy_config
        model.settings['configurations'] = configurations

    if args.command in ('open', 'messages'):
        if args.command == 'open' and len(chosen) != 1:
            raise ValueError('Select exactly one model/project to open')
        if args.command == 'open':
            return backend.open_model(chosen[0], args.matlab)

        tui.pipeline([('sources', 'Custom message generation')])
        tui.stage('sources', 'running')
        check_build_memory(args)
        result = backend.generate(chosen, arches, args.archives_dir, args.matlab, args.matlab_release,
                                  args.ros_distro, args.command, jobs=args.jobs, user_startup=args.user_startup,
                                  work_cache=getattr(args, 'publication_cache', None))
        tui.stage('sources', 'done')
        return result

    tui.pipeline([('sources', 'Model sources'), ('devstage', 'Development staging'),
                  ('devbuild', 'Development compilation'), ('release', 'ARM64 release staging'),
                  ('publish', 'GitHub publication')])

    tui.fact('Models', ', '.join(m.name for m in chosen))
    tui.fact('Architectures', ', '.join(arches))

    # get sources: cached, local MATLAB, or a GitHub release
    tui.stage('sources', 'running', 'Resolving build route')
    locally_generated = False
    if args.command == 'process-cached':
        artifacts = backend.cached(chosen, arches, args.archives_dir, args.matlab_release, args.ros_distro)
    else:
        source = 'release' if args.command == 'download' else args.source
        artifacts, generation_error = None, None
        if source in ('auto', 'local'):
            print(f'Local route: {args.matlab} ({args.matlab_release}), architectures {", ".join(arches)}')
            check_build_memory(args)
            try:
                tui.stage('sources', 'running', 'Local MATLAB generation')
                artifacts = backend.generate(chosen, arches, args.archives_dir, args.matlab, args.matlab_release,
                                             args.ros_distro, jobs=args.jobs, user_startup=args.user_startup,
                                             work_cache=getattr(args, 'publication_cache', None))
                locally_generated = True
            except backend.GenerationError as error:
                if source == 'local':
                    raise
                generation_error = error
                print(f'Local generation failed: {error}\nFalling back to GitHub release sources.', file=sys.stderr)
                tui.stage('sources', 'running', 'Local generation failed; downloading release')

        if artifacts is None:
            tui.stage('sources', 'running', 'Downloading and validating GitHub release')
            try:
                artifacts = backend.download(chosen, arches, args.archives_dir, args.matlab_release,
                                             args.ros_distro, args.release, args.repository)
            except (ValueError, OSError, RuntimeError) as error:
                if generation_error is None:
                    raise
                # the generation failure is the real problem; the fallback is secondary
                raise backend.GenerationError(f'{generation_error}\nThe GitHub release fallback also failed:\n'
                                              f'  {error}') from error
            if generation_error is not None:
                tui.fact('Local generation', f'failed, used the GitHub release instead: {generation_error}')
    tui.stage('sources', 'done', 'Local generation' if locally_generated else 'Cached/release sources')
    tui.fact('Source', 'Local MATLAB generation' if locally_generated else
             'Cached archives' if args.command == 'process-cached' else 'GitHub release')

    # stage and build host packages in the dev workspace
    host = architecture(platform.machine())
    if not args.no_process_local:
        host_artifacts = [a for a in artifacts if a['architecture'] == host]
        if host_artifacts:
            tui.stage('devstage', 'running', f'Installing {host} source packages')
            packages = install(host_artifacts, args.archives_dir, args.local_dir, args.local_dir.parent)
            tui.stage('devstage', 'done')
            tui.fact('Development install', f'{", ".join(packages)} → {args.local_dir}')

            if not args.no_build:
                check_build_memory(args)
                tui.fact('Compiler jobs', args.jobs)
                tui.stage('devbuild', 'running', ', '.join(packages))
                if args.local_dir.parent.name != 'src':
                    raise ValueError('--local-dir must be under a workspace src directory to build')
                workspace = args.local_dir.parent.parent
                backend.reset_stale_builds(workspace, args.local_dir, packages)
                try:
                    backend.run(['colcon', 'build', '--executor', 'sequential', '--packages-select', *packages],
                                workspace, env=backend.build_environment(args.jobs))
                except subprocess.CalledProcessError as error:
                    raise RuntimeError(f'colcon could not compile {", ".join(packages)} in {workspace} '
                                       f'(exit status {error.returncode}); the compiler errors are in the '
                                       f'colcon output') from error
                tui.stage('devbuild', 'done')
            else:
                tui.stage('devbuild', 'skipped', 'Source staging only')
        else:
            print(f'No {host} artifacts selected; skipping development build')
            tui.stage('devstage', 'skipped', f'No {host} artifacts')
            tui.stage('devbuild', 'skipped', f'No {host} artifacts')
    else:
        tui.stage('devstage', 'skipped', 'Development staging disabled')
        tui.stage('devbuild', 'skipped', 'Development staging disabled')

    # stage arm packages in the release workspace, deploying if asked
    if not args.no_process_deploy:
        arm_artifacts = [a for a in artifacts if a['architecture'] == 'arm64']
        if arm_artifacts:
            tui.stage('release', 'running', 'Staging ARM64 source packages')
            packages = install(arm_artifacts, args.archives_dir, args.deploy_dir, args.deploy_dir.parent)
            tui.fact('Release staging', f'{", ".join(packages)} → {args.deploy_dir}')
            if args.deploy:
                tui.stage('release', 'running', f'Deploying to {args.deploy_target}')
                try:
                    backend.run(['colcon', 'deploy', args.deploy_target, '--packages-select', *packages],
                                args.deploy_dir.parent.parent)
                except subprocess.CalledProcessError as error:
                    raise RuntimeError(f'colcon deploy of {", ".join(packages)} to {args.deploy_target} failed '
                                       f'(exit status {error.returncode}); see the colcon output') from error
            tui.stage('release', 'done', 'Deployed' if args.deploy else 'Source staging only')
        else:
            tui.stage('release', 'skipped', 'No ARM64 artifacts')
    else:
        tui.stage('release', 'skipped', 'Release staging disabled')

    # only local builds can be published
    if locally_generated:
        print('Local generation and requested installation/build steps succeeded.')
        offer_publication(args, artifacts)
    elif args.publish:
        raise ValueError('Publication skipped: this run used release sources, not a successful local generation')
    else:
        tui.stage('publish', 'skipped', 'Release/cached input')


def can_interact(args):
    return not args.non_interactive and sys.stdin.isatty() and sys.stdout.isatty()


def check_build_memory(args):
    memory = resources.memory_use()
    if (memory is None or memory.utilization < resources.HIGH_MEMORY
            or getattr(args, '_memory_warning_shown', False)):
        return

    args._memory_warning_shown = True
    message = resources.memory_warning(memory, args.jobs)
    print('WARNING: ' + message, file=sys.stderr)
    if not can_interact(args):
        print('Continuing with the configured count; use --jobs N to lower it.', file=sys.stderr)
        return

    # offer a lower count, but never pick one for the user
    choices = [f'Continue with {args.jobs} job' + ('s' if args.jobs != 1 else '')]
    if args.jobs > 1:
        choices.append('Choose a lower job count')
    choices.append('Cancel build')

    while True:
        choice = tui.choose(f'High memory use: {memory.utilization:.0%}, '
                            f'{memory.available / 1024**3:.1f} GiB available', choices)[0]
        if choice == 0:
            return
        if choice == len(choices) - 1:
            raise KeyboardInterrupt

        # keep asking until the count is valid; back returns to the choice above
        try:
            while True:
                try:
                    value = int(tui.prompt(f'Compiler jobs (1–{args.jobs - 1})', str(max(1, args.jobs // 2))))
                    if not 1 <= value < args.jobs:
                        raise ValueError
                    args.jobs = value
                    print(f'Using {args.jobs} compiler jobs for this build.')
                    return
                except ValueError:
                    tui.page('Invalid job count', f'Enter a number from 1 to {args.jobs - 1}.')
        except tui.Back:
            continue


def preview(records, args):
    lines = []
    for record in records:
        repo = args.repository or record.get('repository') or '(no GitHub repository configured)'
        tag = args.publish_release or record.get('publish_release') or 'models-latest'
        lines.append(f'{repo}@{tag}: {record["model"]}/{record["architecture"]}\n'
                     f'  Source: {record["source_commit"]}' +
                     (' [MODIFIED SOURCES]' if record.get('dirty') else ''))

    text = '\n'.join(lines)
    print(text)
    if tui.active():
        tui.page('Publication preview', text, back_cancels=True)


def offer_publication(args, artifacts):
    if args.publish is False or not artifacts:
        tui.stage('publish', 'skipped')
        return
    if args.publish is None:
        if not can_interact(args):
            tui.stage('publish', 'skipped', 'Not requested')
            return
        if tui.choose('Local build complete', ['Update GitHub model release', 'Skip upload'])[0] == 1:
            tui.stage('publish', 'skipped', 'Upload declined')
            return

    directory = getattr(args, 'publication_cache', args.archives_dir)
    directory.mkdir(parents=True, exist_ok=True)
    if directory.resolve() != args.archives_dir.resolve():
        # --no-archive must not erase the only copy when an upload fails.
        for record in artifacts:
            shutil.copyfile(args.archives_dir / record['asset'], directory / record['asset'])

    args.index = publishing.bundle(directory, artifacts)
    publish_index(args)


def publish_index(args):
    index_path = args.index.resolve()

    # command to print if this fails partway
    retry = ['ros2', 'run', 'riptide_simulink', 'model_manager.py', 'publish', '--index', str(index_path)]
    for flag, value in (('--repository', args.repository), ('--publish-release', args.publish_release)):
        if value:
            retry += [flag, value]
    if args.allow_dirty:
        retry += ['--allow-dirty']

    try:
        tui.stage('publish', 'running', 'Validating local artifacts')
        index = read_index(index_path)
        records = index['artifacts']
        preview(records, args)
        if args.repository:
            # Validate the captured selection before applying a destination override.
            expected = index.get('publication', {}).get('expected')
            if expected is not None and (len(expected) != len(records) or
                                         {tuple(k) for k in expected} != {publishing.key(r) for r in records}):
                raise ValueError('Incomplete selected build set in publication index')
            for record in records:
                record['repository'] = args.repository
            if 'publication' in index:
                index['publication']['expected'] = [publishing.key(r) for r in records]

        # modified sources need an explicit ok
        dirty = any(r.get('dirty') for r in records)
        if dirty and can_interact(args):
            if not tui.confirm('This build used MODIFIED SOURCES. Publish it with that warning?'):
                print('Upload skipped; local build preserved.')
                tui.stage('publish', 'skipped', 'Modified-source upload declined')
                return
            args.allow_dirty = True
            if '--allow-dirty' not in retry:
                retry += ['--allow-dirty']
        publishing.validate_local(index, index_path.parent, args.allow_dirty)

        # one publish per repository/tag
        grouped = {}
        for record in records:
            destination = (record['repository'],
                           args.publish_release or record.get('publish_release') or 'models-latest')
            grouped.setdefault(destination, []).append(record)

        # Authentication happens only after validation and the user's upload selection.
        tui.stage('publish', 'running', 'Checking GitHub authentication')
        token, owner = Auth().identity(can_interact(args))
        for (repo, tag), selected in grouped.items():
            tui.stage('publish', 'running', f'{repo}@{tag}')
            channel = selected[0].get('publish_release') or 'models-latest'
            publishing.publish(index_path.parent, API(repo, token), tag, records=selected,
                               owner=owner, allow_dirty=args.allow_dirty, latest=tag == channel)
            tui.fact('Published', f'{repo}@{tag}')
        tui.stage('publish', 'done')
    except (Exception, KeyboardInterrupt):
        print('Publication did not finish. Local build artifacts are preserved.\nRetry: '
              + shlex.join(retry), file=sys.stderr)
        raise


def main(argv=None):
    try:
        args = arguments(sys.argv[1:] if argv is None else argv)
        if args.command is None or args.interactive:
            if args.command is not None:
                raise ValueError('Use --interactive without a command to enter the guided manager')
            if args.non_interactive:
                raise ValueError('Specify a command when using --non-interactive')
            from .interactive import run
            run(args)
            return 0

        # Explicit commands are scriptable single-run operations, even in a terminal.
        # account check is the exception: signing in is what it was asked to do.
        args.sign_in = not args.non_interactive
        args.non_interactive = True
        if args.command in ('list', 'open', 'account', 'packages'):
            execute(args)
        else:
            with manager_lock():
                execute(args)
        tui.summary()
        return 0
    except KeyboardInterrupt:
        tui.summary('cancelled')
        print('\nCancelled; no fallback attempted.', file=sys.stderr)
        return 130
    except (ValueError, OSError, RuntimeError, subprocess.CalledProcessError) as error:
        tui.summary('failed')
        print(f'Error: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
