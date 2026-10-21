"""Remembered workspaces, path completion, and the home page workspace bar."""
from unittest.mock import Mock, patch

import pytest

from riptide_simulink import cli, interactive, tui, workspaces


def test_remember_moves_to_front_dedupes_and_caps(tmp_path):
    folders = [tmp_path / str(i) for i in range(12)]
    for folder in folders:
        folder.mkdir()
        workspaces.remember(folder)
    workspaces.remember(folders[5])

    saved = workspaces.recent()
    assert saved[0] == folders[5] and len(saved) == workspaces.LIMIT
    assert saved.count(folders[5]) == 1 and folders[0] not in saved

    workspaces.forget(folders[5])
    assert folders[5] not in workspaces.recent()


def test_corrupt_history_and_deleted_last_workspace(tmp_path):
    workspaces.state_file().parent.mkdir(parents=True)
    workspaces.state_file().write_text('{not json')
    assert workspaces.recent() == [] and workspaces.last() is None

    gone = tmp_path / 'gone'
    gone.mkdir()
    workspaces.remember(gone)
    gone.rmdir()
    assert workspaces.last() is None


def test_matches_list_directories_only(tmp_path):
    for name in ('release', 'release_old', 'development', '.hidden'):
        (tmp_path / name).mkdir()
    (tmp_path / 'release.txt').touch()

    assert workspaces.matches(f'{tmp_path}/rel') == [f'{tmp_path}/release/', f'{tmp_path}/release_old/']
    assert workspaces.matches(f'{tmp_path}/') == [f'{tmp_path}/development/', f'{tmp_path}/release/',
                                                  f'{tmp_path}/release_old/']
    assert workspaces.matches(f'{tmp_path}/.h') == [f'{tmp_path}/.hidden/']
    assert workspaces.matches(f'{tmp_path}/missing/x') == []


def test_tab_completes_single_match_or_common_prefix(tmp_path):
    for name in ('release', 'release_old', 'development'):
        (tmp_path / name).mkdir()

    assert workspaces.complete(f'{tmp_path}/dev') == f'{tmp_path}/development/'
    assert workspaces.complete(f'{tmp_path}/r') == f'{tmp_path}/release'
    assert workspaces.complete(f'{tmp_path}/zzz') == f'{tmp_path}/zzz'


def test_matches_keep_tilde(tmp_path, monkeypatch):
    monkeypatch.setenv('HOME', str(tmp_path))
    (tmp_path / 'osu-uwrt').mkdir()
    assert workspaces.matches('~/osu') == ['~/osu-uwrt/']
    assert workspaces.label(tmp_path / 'osu-uwrt') == '~/osu-uwrt'


def screen(keys):
    fake = Mock()
    fake.getmaxyx.return_value = (30, 100)
    fake.getch.side_effect = keys
    fake.get_wch.side_effect = keys
    return fake


def test_home_starts_on_first_action_and_bar_is_above():
    assert tui._home_menu(screen([10]), 'Home', '~/ws', ['Build', 'Quit']) == 0
    assert tui._home_menu(screen([tui.curses.KEY_UP, 10]), 'Home', '~/ws', ['Build', 'Quit']) == -1
    assert tui._home_menu(screen([ord('w')]), 'Home', '~/ws', ['Build', 'Quit']) == -1
    with pytest.raises(KeyboardInterrupt):
        tui._home_menu(screen([ord('q')]), 'Home', '~/ws', ['Build', 'Quit'])


def test_editor_tab_completes_then_accepts(tmp_path):
    (tmp_path / 'release').mkdir()
    session = tui.Session(screen(['\x15'] + list(f'{tmp_path}/rel') + ['\t', '\n']))
    assert session.workspace('~') == f'{tmp_path}/release/'


def test_editor_picks_and_forgets_presets(tmp_path):
    first, second = tmp_path / 'first', tmp_path / 'second'
    first.mkdir()
    second.mkdir()
    workspaces.remember(first)
    workspaces.remember(second)

    down = tui.curses.KEY_DOWN
    assert tui.Session(screen([down, down, '\n'])).workspace(str(second)) == str(first)

    with pytest.raises(tui.Back):
        tui.Session(screen([down, tui.curses.KEY_DC, '\x1b'])).workspace(str(second))
    assert workspaces.recent() == [first]


def test_editor_rejects_missing_directory(tmp_path):
    fake = screen(['\x15'] + list(f'{tmp_path}/missing') + ['\n', '\x1b'])
    with pytest.raises(tui.Back):
        tui.Session(fake).workspace('~')
    drawn = [c.args[2] for c in fake.addnstr.call_args_list]
    assert any(text.startswith('Not a directory') for text in drawn)


def run_home(args, choices, chosen=None):
    with patch.object(tui, 'run', side_effect=lambda callback: callback(Mock())), \
            patch.object(tui, 'home', side_effect=choices) as home, \
            patch.object(tui, 'workspace', return_value=chosen):
        interactive.run(args)
    return home


def test_tui_starts_in_last_workspace_unless_given(tmp_path):
    saved, given = tmp_path / 'saved', tmp_path / 'given'
    saved.mkdir()
    given.mkdir()
    workspaces.remember(saved)

    args = cli.arguments([])
    run_home(args, [KeyboardInterrupt()])
    assert args.workspace == saved

    args = cli.arguments(['--workspace', str(given)])
    run_home(args, [KeyboardInterrupt()])
    assert args.workspace == given and workspaces.recent()[0] == given


def test_choosing_workspace_updates_session_and_history(tmp_path):
    start, chosen = tmp_path / 'start', tmp_path / 'chosen'
    start.mkdir()
    chosen.mkdir()
    args = cli.arguments(['--workspace', str(start)])
    home = run_home(args, [-1, KeyboardInterrupt()], str(chosen))

    assert args.workspace == chosen and workspaces.recent() == [chosen, start]
    assert home.call_args.args[1] == str(chosen)
