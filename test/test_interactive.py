"""Navigation, noninteractive boundaries and child-process cancellation."""
import io
import os
import subprocess
import sys
from unittest.mock import patch, Mock

import pytest

from riptide_simulink import backend, cli, interactive, tui


def action(name):
    return next(i for i, pair in enumerate(interactive.ACTIONS) if pair[1] == name)


class UI:
    def __init__(self):
        self.results = []

    def operation(self, title, callback):
        callback()
        self.results.append(title)


def test_listing_packages_and_settings_return_to_home(tmp_path):
    models = tmp_path / 'models'
    models.mkdir()
    (models / 'plant.slx').touch()
    args = cli.arguments(['--workspace', str(tmp_path)])
    ui = UI()
    choices = [[action('list')], [action('packages')], [action('advanced')], [action('quit')]]
    with patch.object(tui, 'run', side_effect=lambda callback: callback(ui)), \
            patch.object(tui, 'choose', side_effect=choices), \
            patch.object(tui, 'sections', side_effect=[7, len(interactive.TEXT), 8]), \
            patch.object(tui, 'page') as page:
        interactive.run(args)
        assert page.call_count == 2
        assert page.call_args_list[0].args[0] == 'Available models'
        assert 'plant.slx' in page.call_args_list[0].args[1]

    assert ui.results == []


def test_default_build_requires_only_model_selection_and_build():
    from types import SimpleNamespace
    args = cli.arguments(['build'])
    models = [SimpleNamespace(identity='models/plant.slx', excluded=False)]
    with patch.object(tui, 'models', return_value=[0]) as selected, \
            patch.object(tui, 'choose', return_value=[0]) as choose:
        interactive.configure(args, models)

    assert selected.call_count == choose.call_count == 1
    assert choose.call_args.args[1] == ['Build', 'Advanced options', 'Back']
    assert args.arch == ['x86_64', 'arm64'] and args.source == 'auto'
    assert not args.no_build and not args.deploy and not args.user_startup


def test_back_from_action_returns_home_and_root_back_exits(tmp_path):
    args = cli.arguments(['--workspace', str(tmp_path)])
    with patch.object(tui, 'run', side_effect=lambda callback: callback(UI())), \
            patch.object(tui, 'choose', side_effect=[[action('build')], tui.Back(), tui.Back()]) as choose:
        # No models: the build page reports an error and returns home.
        with patch.object(tui, 'page'):
            interactive.run(args)
        assert choose.call_count >= 2


def test_curses_results_back_and_prompt_editing():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 80)
    screen.getch.side_effect = [tui.curses.KEY_DOWN, tui.curses.KEY_NPAGE, ord('b')]
    session = tui.Session(screen)
    session.page('Models', '\n'.join(str(i) for i in range(50)))
    assert screen.refresh.call_count == 3
    screen.get_wch.side_effect = ['\x15', 'n', 'e', 'w', '\n']
    assert session.prompt('Value', 'old') == 'new'


@pytest.mark.parametrize('failure', [ValueError('bad input'), subprocess.CalledProcessError(1, ['false']),
                                     KeyboardInterrupt()])
def test_operation_failures_show_results_without_exiting(failure):
    screen = Mock()
    screen.getmaxyx.return_value = (20, 80)
    session = tui.Session(screen)
    with patch.object(session, 'page') as page, patch('sys.stdout', session):
        session.operation('Build', Mock(side_effect=failure))
        assert page.call_count == 1
        assert ('CANCELLED' if isinstance(failure, KeyboardInterrupt) else 'FAILED') in page.call_args.args[0]

    assert not session.running


def test_plain_session_lists_models_then_stays_open(tmp_path):
    models = tmp_path / 'models'
    models.mkdir()
    (models / 'plant.slx').touch()
    args = cli.arguments(['--workspace', str(tmp_path)])
    answers = [str(action('list') + 1), '', str(action('advanced') + 1), '7', '2', '', 'b', 'b', str(action('quit') + 1)]
    terminal = io.StringIO()
    with patch('sys.stdin.isatty', return_value=True), patch('sys.stdout', terminal), \
            patch.object(terminal, 'isatty', return_value=True), patch.dict(os.environ, {'TERM': 'dumb'}), \
            patch('builtins.input', side_effect=answers), patch.object(interactive, 'Auth') as account:
        account.return_value.status.return_value = 'No saved login'
        interactive.run(args)

    assert terminal.getvalue().count('Simulink model manager') == 3
    assert 'Available models' in terminal.getvalue() and 'plant.slx' in terminal.getvalue()
    assert tui._current is None


def test_explicit_commands_never_prompt_even_with_tty(tmp_path):
    with patch('sys.stdin.isatty', return_value=True), patch('sys.stdout.isatty', return_value=True), \
            patch.object(tui, 'choose') as choose, patch.object(cli, 'Auth') as auth:
        assert cli.main(['build', '--workspace', str(tmp_path)]) == 1
        choose.assert_not_called()
        auth.assert_not_called()

    with patch.object(interactive, 'run') as run:
        assert cli.main([]) == 0
        run.assert_called_once()


def test_child_output_captured_and_child_cancelled(tmp_path):
    output = io.StringIO()
    with patch.object(tui, 'active', return_value=True), patch.object(tui, 'check_cancel'), \
            patch('sys.stdout', output):
        backend.run([sys.executable, '-c', 'print("child output")'])

    assert 'child output' in output.getvalue()
    pid_file = tmp_path / 'pid'
    calls = 0

    def cancel():
        nonlocal calls
        calls += 1
        if pid_file.exists():
            raise KeyboardInterrupt
        if calls > 100:
            raise AssertionError('Child did not start')

    code = ('import os,time; from pathlib import Path; '
            'Path(os.environ["TEST_PID_FILE"]).write_text(str(os.getpid())); time.sleep(30)')
    env = dict(os.environ, TEST_PID_FILE=str(pid_file))
    with patch.object(tui, 'active', return_value=True), patch.object(tui, 'check_cancel', side_effect=cancel), \
            pytest.raises(KeyboardInterrupt):
        backend.run([sys.executable, '-c', code], env=env)

    with pytest.raises(ProcessLookupError):
        os.kill(int(pid_file.read_text()), 0)


def test_back_from_build_returns_to_model_selection():
    from types import SimpleNamespace
    args = cli.arguments(['build'])
    models = [SimpleNamespace(identity=f'models/{name}.slx', excluded=False) for name in ('one', 'two')]
    with patch.object(tui, 'models', side_effect=[[0], [1]]) as selected, \
            patch.object(tui, 'choose', side_effect=[tui.Back(), [0]]):
        interactive.configure(args, models)

    assert args.models == ['models/two.slx'] and selected.call_count == 2


def test_cli_cleanup_requires_yes_without_prompt(tmp_path):
    args = ['clean', '--local-dir', str(tmp_path / 'dev/src/simulink_models'), '--deploy-dir',
            str(tmp_path / 'rel/src/simulink_models'), '--archives-dir', str(tmp_path / 'cache')]
    with patch.object(tui, 'confirm') as confirm, patch('sys.stdin.isatty', return_value=True):
        assert cli.main(args) == 1
        confirm.assert_not_called()


def test_back_on_publication_preview_never_authenticates(tmp_path):
    args = cli.arguments(['publish', '--index', str(tmp_path / 'build.json')])
    screen = Mock()
    screen.getmaxyx.return_value = (20, 80)
    screen.getch.return_value = ord('b')
    session = tui.Session(screen)
    with patch.object(cli, 'read_index', return_value={'artifacts': []}), \
            patch.object(cli, 'Auth') as login, patch.object(tui, '_current', session):
        with pytest.raises(tui.Back):
            cli.publish_index(args)
        login.assert_not_called()


def test_referenced_section_is_collapsed_and_never_selected_implicitly():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 100)
    screen.getch.return_value = 10
    assert tui._model_menu(screen, 'Models', ['main.slx'], ['referenced/child.slx']) == [0]
    rendered = [str(call.args[2]) for call in screen.addnstr.call_args_list]
    assert any('> Referenced/excluded models (1)' in line for line in rendered)
    assert not any('child.slx' in line for line in rendered)


def test_expanding_and_collapsing_preserves_model_selection():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 100)
    screen.getch.side_effect = [ord(' '), tui.curses.KEY_DOWN, 10,
        tui.curses.KEY_DOWN, ord(' '), tui.curses.KEY_LEFT, tui.curses.KEY_UP, 10]
    assert tui._model_menu(screen, 'Models', ['main.slx'], ['child.slx'], multiple=True) == [0, 1]
    rendered = [str(call.args[2]) for call in screen.addnstr.call_args_list]
    assert any('child.slx' in line for line in rendered)
    assert any('> Referenced/excluded models (1; 1 selected)' in line for line in rendered)


def test_only_excluded_models_require_expansion():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 100)
    screen.getch.side_effect = [10, tui.curses.KEY_DOWN, 10]
    assert tui._model_menu(screen, 'Models', [], ['child.slx']) == [0]


def test_plain_referenced_section_can_expand_and_return():
    terminal = io.StringIO()
    session = tui.Session()
    with patch('sys.stdout', terminal), patch.object(tui, '_current', session), \
            patch('builtins.input', side_effect=['2', '3']):
        assert tui.models('Models', ['main.slx'], ['child.slx']) == [1]

    # Browsing can collapse again without leaving the listing or selecting a model.
    with patch.object(tui, '_plain_choose', side_effect=[[1], [1], tui.Back()]) as choose:
        assert tui.models('Models', ['main.slx'], ['child.slx'], browse=True) is None
    assert 'child.slx' not in '\n'.join(choose.call_args_list[0].args[1])
    assert any('child.slx' in row for row in choose.call_args_list[1].args[1])
    assert 'child.slx' not in '\n'.join(choose.call_args_list[2].args[1])


def test_advanced_editors_return_to_same_page_and_retain_selection():
    args = cli.arguments([])
    with patch.object(tui, 'sections', side_effect=[6, 0, tui.Back()]) as sections, \
            patch.object(tui, 'choose', side_effect=tui.Back()) as choose, \
            patch.object(tui, 'prompt', side_effect=tui.Back()):
        interactive.settings(args)

    assert [c.args[0] for c in sections.call_args_list] == ['Advanced options'] * 3
    assert [c.args[2] for c in sections.call_args_list] == [0, 6, 0]
    assert choose.call_args.args[0] == 'Build source'
    assert args.matlab == 'matlab' and args.source == 'auto'
    assert args.workspace == cli.ROOT / 'development/software/src'


def test_escape_from_release_tag_returns_to_single_advanced_page():
    args = cli.arguments(['download', 'plant'])
    fields = [f for entries in interactive.GROUPS.values() for f in entries
              if f in interactive.relevant_fields(args)]
    selected = fields.index('release')
    with patch.object(tui, 'sections', side_effect=[selected, tui.Back()]) as sections, \
            patch.object(tui, 'prompt', side_effect=tui.Back()):
        interactive.settings(args)

    assert [c.args[0] for c in sections.call_args_list] == ['Advanced options'] * 2
    assert sections.call_args.args[2] == selected
    assert args.release is None


def test_escape_collapses_excluded_section_before_leaving_list():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 100)
    screen.getch.side_effect = [tui.curses.KEY_DOWN, 10, 27, 27]
    with pytest.raises(tui.Back):
        tui._model_menu(screen, 'Models', ['main.slx'], ['child.slx'], browse=True)
    assert screen.getch.call_count == 4
    rendered = [str(c.args[2]) for c in screen.addnstr.call_args_list]
    assert sum('> Referenced/excluded' in line for line in rendered) == 3


def test_session_uses_short_escape_delay_and_restores_it(monkeypatch):
    monkeypatch.delenv('ESCDELAY', raising=False)
    monkeypatch.setenv('TERM', 'xterm')
    old = tui.curses.get_escdelay()
    observed = []
    with patch('sys.stdin.isatty', return_value=True), patch('sys.stdout.isatty', return_value=True), \
            patch.object(tui.curses, 'wrapper', side_effect=lambda callback: callback(Mock())):
        tui.run(lambda _: observed.append(tui.curses.get_escdelay()))

    assert observed == [25]
    assert tui.curses.get_escdelay() == old


def test_progress_panel_stays_below_output_and_reads_architecture_events():
    screen = Mock()
    screen.getmaxyx.return_value = (25, 100)
    session = tui.Session(screen)
    session.running = True
    with patch.object(tui, '_current', session):
        tui.pipeline([('sources', 'Model sources'), ('devstage', 'Development staging'),
                      ('devbuild', 'Compilation'), ('release', 'Release staging'), ('publish', 'Publication')])
        tui.stage('sources', 'running')
        session.write('\n'.join('output line ' + str(i) for i in range(100)) + '\n')
        session.write('RIPTIDE_PROGRESS {"model":"/models/plant.slx",')
        session.write('"architecture":"arm64","phase":"Generating (2/2)"}\n')

    assert session.stages['sources']['detail'] == 'plant.slx arm64 Generating (2/2)'
    assert not any('RIPTIDE_PROGRESS' in line for line in session.lines)
    calls = screen.addnstr.call_args_list
    assert any(c.args[0] == 18 and c.args[2].startswith(' RUNNING  0/5') for c in calls)
    assert all(c.args[0] < 18 for c in calls if str(c.args[2]).startswith('output line'))
    assert any('arm64 Generating (2/2)' in str(c.args[2]) for c in calls)


def test_progress_records_failure_and_keeps_summary_in_results():
    session = tui.Session()

    def fail():
        tui.pipeline([('sources', 'Model sources'), ('build', 'Compilation')])
        tui.stage('sources', 'done')
        tui.stage('build', 'running')
        raise RuntimeError('compiler rejected source')

    with patch.object(tui, '_current', session), patch('sys.stdout', session), patch.object(session, 'page') as page:
        session.operation('Build', fail)
    assert session.stages['sources']['status'] == 'done'
    assert session.stages['build']['status'] == 'failed'
    assert any(text.startswith('[failed] Compilation') for text, _ in page.call_args.kwargs['footer'])
    assert page.call_args.kwargs['alert'] == 'Error: compiler rejected source'


def test_advanced_options_hide_irrelevant_groups():
    args = cli.arguments(['build', 'plant'])
    with patch.object(tui, 'sections', side_effect=tui.Back()) as sections:
        interactive.settings(args)
    groups = sections.call_args.args[1]
    assert [heading for heading, _ in groups] == ['Build and MATLAB', 'GitHub and publishing',
                                                'Output and deployment', 'Workspace', '']
    assert groups[0][1][0] == 'Build source: auto'
    assert groups[-1][1] == ['Back']


def test_boolean_setting_changes_in_place():
    args = cli.arguments(['build', 'plant'])
    with patch.object(tui, 'sections', side_effect=[2, tui.Back()]) as sections, \
            patch.object(tui, 'choose') as choose:
        interactive.settings(args)

    assert args.no_build
    assert sections.call_args.args[1][0][1][2] == 'Skip development compilation: yes'
    assert sections.call_args.args[2] == 2
    choose.assert_not_called()


def test_section_headings_are_visible_but_navigation_skips_them():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 100)
    screen.getch.side_effect = [tui.curses.KEY_DOWN, 10]
    groups = [('Build', ['Source']), ('GitHub', ['Release']), ('', ['Back'])]
    assert tui._section_menu(screen, 'Advanced options', groups) == 1
    rows = screen.addnstr.call_args_list
    assert any(c.args[2] == 'Build' and c.args[4] == tui.curses.A_BOLD for c in rows)
    assert any(c.args[2] == 'GitHub' and c.args[4] == tui.curses.A_BOLD for c in rows)
    assert any(c.args[2] == '  Release' and c.args[4] == tui.curses.A_REVERSE for c in rows)


def test_section_page_scrolls_to_last_option_and_restores_cursor():
    screen = Mock()
    screen.getmaxyx.return_value = (8, 100)
    groups = [('Build', ['Source', 'Architecture']), ('Outputs', ['Dev', 'Release']), ('', ['Back'])]
    screen.getch.side_effect = [tui.curses.KEY_END, tui.curses.KEY_UP, 10]
    assert tui._section_menu(screen, 'Advanced options', groups) == 3
    screen.getch.return_value = 10
    screen.getch.side_effect = None
    assert tui._section_menu(screen, 'Advanced options', groups, selected=3) == 3
    screen.getch.side_effect = [tui.curses.KEY_HOME, tui.curses.KEY_NPAGE, tui.curses.KEY_PPAGE, 10]
    assert tui._section_menu(screen, 'Advanced options', groups, selected=3) == 0
    screen.getch.side_effect = [27]
    with pytest.raises(tui.Back):
        tui._section_menu(screen, 'Advanced options', groups)


def test_plain_sections_number_only_options_and_return_one_level():
    terminal = io.StringIO()
    groups = [('Build', ['Source']), ('GitHub', ['Release']), ('', ['Back'])]
    with patch.object(tui, '_current', tui.Session()), patch('sys.stdout', terminal), \
            patch('builtins.input', side_effect=['0', '2', 'b']):
        assert tui.sections('Advanced options', groups) == 1
        with pytest.raises(tui.Back):
            tui.sections('Advanced options', groups, selected=1)

    assert '\nBuild\n  1. Source\n\nGitHub\n  2. Release\n  3. Back' in terminal.getvalue()


def test_advanced_actions_share_one_page_and_retain_selection():
    args = cli.arguments([])
    perform = Mock()
    with patch.object(tui, 'sections', side_effect=[4, tui.Back()]) as sections, \
            patch.object(tui, 'choose') as choose:
        interactive.advanced_home(args, perform)

    perform.assert_called_once_with('messages')
    assert sections.call_args.args[2] == 4
    assert sections.call_args.args[1][1] == ('Maintenance',
        ['Clean outputs', 'Delete generated packages', 'Refresh messages'])
    choose.assert_not_called()


@pytest.mark.parametrize('command', ['publish', 'clean'])
def test_back_from_action_returns_to_its_input_selection(tmp_path, command):
    (tmp_path / 'publish-one.json').write_text('{}')
    args = cli.arguments([command, '--archives-dir', str(tmp_path)])
    with patch.object(tui, 'choose', side_effect=[[0], tui.Back(), tui.Back()]) as choose, \
            patch.object(interactive.cleanup, 'plan', return_value=(['plant'], [], [], [])):
        with pytest.raises(tui.Back):
            interactive.configure(args, [])

    titles = [call.args[0] for call in choose.call_args_list]
    assert titles[0] == titles[2]
    assert titles[1].startswith('Publish:' if command == 'publish' else 'Clean:')


def test_back_from_scope_package_selection_returns_to_scope_menu():
    args = cli.arguments(['clean'])
    with patch.object(tui, 'choose', side_effect=[[0], tui.Back(), tui.Back()]) as choose, \
            patch.object(interactive.cleanup, 'plan', return_value=(['plant'], [], [], [])):
        with pytest.raises(tui.Back):
            interactive.edit(args, 'scope')

    assert [call.args[0] for call in choose.call_args_list] == ['Package selection', 'Packages', 'Package selection']


def test_duration_matches_colcon_style():
    assert [tui.duration(s) for s in (0.04, 59.94, 60, 3725)] == ['0.0s', '59.9s', '1min 0s', '1h 2min 5s']


def test_cli_stages_print_timers_heartbeat_and_summary(capsys):
    clock = iter([0, 0, 5, 5, 10, 40, 41, 70, 72, 75, 75])
    with patch.object(tui, '_console', tui.Board()), patch.object(tui.time, 'monotonic', side_effect=lambda: next(clock)):
        tui.pipeline([('sources', 'Model sources'), ('devbuild', 'Compilation')])
        tui.stage('sources', 'running', 'Local MATLAB generation')
        tui.progress_event('RIPTIDE_PROGRESS {"model":"/m/plant.slx","architecture":"x86_64","phase":"Generating (1/2)"}')
        tui.tick()  # 10s since the last line: quiet
        tui.tick()  # 35s: heartbeat
        tui.stage('sources', 'running', 'plant.slx x86_64 Generating (1/2)')  # unchanged: quiet
        tui.stage('sources', 'done')
        tui.stage('devbuild', 'running')
        tui.summary('failed')

    lines = capsys.readouterr().out.splitlines()
    assert lines == [
        '=== Model sources: started (Local MATLAB generation)',
        '[5.0s] Model sources: plant.slx x86_64 Generating (1/2)',
        '[40.0s][0/2 stages finished][Model sources - 40.0s][plant.slx x86_64 Generating (1/2) - 35.0s]',
        '=== Model sources: finished [1min 10s]',
        '=== Compilation: started',
        '=== Summary: 1 stages finished [1min 15s]',
        '    1 failed: Compilation',
        '    plant x86_64: unfinished']
    assert not tui._console.stages


def test_relay_hides_split_progress_events_and_keeps_partial_output(capsys):
    with patch.object(tui, '_console', tui.Board()):
        tui.pipeline([('sources', 'Model sources')])
        tui.stage('sources', 'running')
        capsys.readouterr()
        relay = tui.Relay()
        relay.feed('compiling 50%')
        relay.feed('\rdone\nRIPTIDE_PRO')
        relay.feed('GRESS {"model":"/m/plant.slx","phase":"Preparing messages"}\nRIPTIDE')
        relay.feed('_ROBOT not an event\nRIPTIDE_PROGRESS {bad json}\ntail')
        relay.close()
        assert tui._console.stages['sources']['detail'] == 'plant.slx Preparing messages'

    out = capsys.readouterr().out
    assert 'compiling 50%\rdone\n' in out and 'RIPTIDE_ROBOT not an event\n' in out
    assert 'RIPTIDE_PROGRESS {bad json}\n' in out and out.endswith('tail')
    assert '"phase"' not in out


def test_panel_shows_overall_stage_and_step_timers():
    screen = Mock()
    screen.getmaxyx.return_value = (25, 120)
    session = tui.Session(screen)
    session.running = True
    now = [0]
    with patch.object(tui, '_current', session), patch.object(tui.time, 'monotonic', side_effect=lambda: now[0]):
        tui.pipeline([('sources', 'Model sources')])
        tui.stage('sources', 'running')
        now[0] = 30
        tui.stage('sources', 'running', 'plant.slx arm64 Generating (2/2)')
        now[0] = 95
        session._progress()

    rendered = [str(c.args[2]) for c in screen.addnstr.call_args_list[-3:]]
    assert any('1min 35s elapsed' in line for line in rendered)
    assert any('Model sources [1min 35s]: plant.slx arm64 Generating (2/2) [step 1min 5s]' in line for line in rendered)


def run_operation(session, callback):
    with patch.object(tui, '_current', session), patch('sys.stdout', session), patch.object(session, 'page') as page:
        session.operation('Build', callback)
    return page.call_args


def test_failure_results_show_only_failed_stage_output_with_pinned_panel():
    screen = Mock()
    screen.getmaxyx.return_value = (30, 100)
    session = tui.Session(screen)

    def fail():
        tui.pipeline([('sources', 'Model sources'), ('release', 'ARM64 release staging')])
        tui.stage('sources', 'running')
        tui.Relay().feed('matlab noise\n' * 50)
        tui.stage('sources', 'done')
        tui.stage('release', 'running')
        print('Staging release packages')
        raise ValueError('Package collision: complete_controller')

    call = run_operation(session, fail)
    content = call.args[1]
    assert 'matlab noise' not in content and 'Staging release packages' in content
    assert 'matlab noise' in call.kwargs['full']
    assert call.kwargs['alert'] == 'Error: Package collision: complete_controller'
    assert call.kwargs['status'] == 'failed' and call.kwargs['at_end']
    assert call.args[0] == '✘ BUILD FAILED'
    header = call.kwargs['footer'][0][0]
    assert header.startswith(' ✘ FAILED  1/2 stages finished') and 'total' in header

    # summary: error first, red timeline row, then the failed stage's last lines
    summary = [' '.join(text for _, text, _ in row) for row in call.kwargs['summary']]
    assert summary[0] == 'Error: Package collision: complete_controller'
    assert any(line.startswith('ARM64 release staging') for line in summary)
    assert summary[-2:] == ['Last output from ARM64 release staging', 'Staging release packages']


def test_success_results_hide_tool_output():
    session = tui.Session(Mock(**{'getmaxyx.return_value': (30, 100)}))

    def succeed():
        tui.pipeline([('sources', 'Model sources')])
        tui.stage('sources', 'running')
        tui.Relay().feed('### Writing source file complete_controller.cpp\n')
        print('Installed complete_controller into /ws/src/simulink_models')
        tui.stage('sources', 'done')

    call = run_operation(session, succeed)
    assert 'Installed complete_controller' in call.args[1] and '### Writing' not in call.args[1]
    assert tui.GUTTER + '### Writing' in call.kwargs['full'] and not call.kwargs['at_end']
    assert call.args[0] == '✔ BUILD COMPLETE' and call.kwargs['status'] == 'done'


def test_menus_during_build_keep_the_stage_panel():
    screen = Mock()
    screen.getmaxyx.return_value = (30, 100)
    screen.getch.return_value = 10
    session = tui.Session(screen)
    session.running = True
    with patch.object(tui, '_current', session):
        tui.pipeline([('sources', 'Model sources')])
        tui.stage('sources', 'running')
        screen.addnstr.reset_mock()
        assert session.choose('Local build complete', ['Update', 'Skip']) == [0]

    rows = {c.args[0]: str(c.args[2]) for c in screen.addnstr.call_args_list}
    assert rows[27].startswith(' WAITING FOR INPUT') and rows[28].startswith('[running  ] Model sources')
    assert rows[1] == 'Local build complete' and screen.derwin.call_args.args[:2] == (27, 100)
    assert 'ACTION NEEDED' in screen.derwin.return_value.addstr.call_args.args[2]


def test_tool_output_is_dimmed_behind_a_gutter():
    screen = Mock()
    screen.getmaxyx.return_value = (30, 100)
    session = tui.Session(screen)
    session.running = True
    with patch.object(tui, '_current', session), patch('sys.stdout', session):
        tui.pipeline([('sources', 'Model sources')])
        tui.stage('sources', 'running')
        tui.Relay().feed('### Generating code\n')
        print('Installed complete_controller')

    calls = {str(c.args[2]).strip(): c.args[4] for c in screen.addnstr.call_args_list}
    assert calls[tui.GUTTER + '### Generating code'] == tui.curses.A_DIM
    assert calls['Installed complete_controller'] == 0


def test_cli_failure_lines_are_red_only_on_color_terminals(capsys, monkeypatch):
    monkeypatch.setenv('TERM', 'xterm')
    monkeypatch.delenv('NO_COLOR', raising=False)
    with patch.object(tui, '_console', tui.Board()), patch('sys.stdout.isatty', return_value=True):
        tui.pipeline([('devbuild', 'Compilation')])
        tui.stage('devbuild', 'running')
        tui.stage('devbuild', 'failed')

    assert '\x1b[1;31m=== Compilation: failed' in capsys.readouterr().out
    monkeypatch.setenv('NO_COLOR', '1')
    with patch.object(tui, '_console', tui.Board()), patch('sys.stdout.isatty', return_value=True):
        tui.pipeline([('devbuild', 'Compilation')])
        tui.stage('devbuild', 'running')
        tui.stage('devbuild', 'failed')

    assert '\x1b' not in capsys.readouterr().out


def test_success_summary_shows_generation_times_and_facts():
    session = tui.Session(Mock(**{'getmaxyx.return_value': (40, 100)}))
    event = 'RIPTIDE_PROGRESS {"model":"/m/plant.slx","architecture":"%s","phase":"%s"%s}\n'

    def succeed():
        tui.pipeline([('sources', 'Model sources'), ('publish', 'GitHub publication')])
        tui.stage('sources', 'running')
        tui.fact('Development install', f'plant → {os.path.expanduser("~")}/ws/src')
        relay = tui.Relay()
        relay.feed(event % ('x86_64', 'Generating (1/2)', ''))
        relay.feed('Warning: corrupted graph\n')
        relay.feed(event % ('x86_64', 'Generation complete', ',"cached":true'))
        with patch.object(tui, 'memory_use', return_value=Mock(utilization=0.81)):
            tui.check_cancel()
        tui.stage('sources', 'done')
        tui.stage('publish', 'skipped', 'Upload declined')

    call = run_operation(session, succeed)
    summary = [' '.join(text for _, text, _ in row) for row in call.kwargs['summary']]
    assert any(line.startswith('plant x86_64 cached') for line in summary)
    assert 'GitHub publication skipped: Upload declined' in summary
    assert 'Development install plant → ~/ws/src' in summary
    assert 'Peak RAM use 81%' in summary and 'Tool output 1 lines, 1 with warnings' in summary
    assert call.kwargs['footer'][0][0].startswith(' ✔ COMPLETE')


def test_results_views_wrap_long_lines_and_switch_with_arrows():
    screen = Mock()
    screen.getmaxyx.return_value = (20, 40)
    screen.getch.side_effect = [tui.curses.KEY_RIGHT, tui.curses.KEY_RIGHT, tui.curses.KEY_LEFT, 10]
    session = tui.Session(screen)
    long_line = 'Installed complete_controller into /home/ethan/ws/src/simulink_models'
    session.page('Build', long_line, full=tui.GUTTER + 'x' * 60, summary=[[(0, 'Time by stage', 0)]])

    drawn = [str(c.args[2]) for c in screen.addnstr.call_args_list]
    assert long_line[:39] in drawn and '  ' + long_line[39:] in drawn
    assert tui.GUTTER + 'x' * 37 in drawn and tui.GUTTER + 'x' * 23 in drawn
    assert sum(text == 'Time by stage' for text in drawn) == 1  # Right, Right, Left never wraps back


def test_saved_builds_are_listed_by_build_tag(tmp_path):
    from riptide_simulink import artifacts
    from test_publishing import record
    index = tmp_path / 'publish-0123.json'
    artifacts.atomic_json(index, dict(schema=1, artifacts=[
        dict(record(tmp_path), source_branch='master', built_by='ethan@desk')]))
    broken = tmp_path / 'publish-broken.json'
    broken.write_text('{}')

    labels = interactive.saved_builds([index, broken])
    assert labels[0].split() == ['plant', 'master/aaaaaaa', '2026-10-19', '12:00', 'UTC', 'ethan@desk', 'x86_64']
    assert labels[1] == 'publish-broken.json'


def test_publish_without_saved_builds_explains(tmp_path):
    args = cli.arguments(['publish', '--archives-dir', str(tmp_path)])
    with pytest.raises(ValueError, match='No saved local builds'):
        interactive.configure(args, [])
