"""Persistent standard-library terminal UI, with numbered-terminal fallback."""
from collections import deque
from contextlib import redirect_stderr, redirect_stdout
import curses
import json
import os
import re
import sys
import subprocess
import textwrap
import time

from . import workspaces
from .resources import memory_use

_current = None
PROGRESS = 'RIPTIDE_PROGRESS '
HEARTBEAT = 30  # seconds between plain-text status lines, like colcon
WARNING = re.compile(r'(?i)\bwarning:')


class Back(Exception):
    """Return to the preceding page without terminating the application."""


class Board:
    """Stage states plus overall, per-stage and current-step timings."""

    def __init__(self):
        self.stages = {}
        self.started = None
        self.ended = None
        self.beat = 0
        self.count = 0  # lines logged so far; locates each stage's output
        self.facts = {}  # label -> value for the results summary
        self.runs = {}  # (model, arch) -> MATLAB generation timing
        self.peak = None  # highest RAM utilization seen
        self.sampled = 0
        self.attention = ''  # something the user must do outside the TUI, e.g. a sign-in code


_console = Board()  # CLI runs have no Session but still report stages
_palette = {}  # curses color attributes by stage status, when the terminal has colors
_bars = {}  # black text on each status color
ANSI = {'failed': '1;31', 'cancelled': '1;33'}
GUTTER = '│ '  # marks child-tool output so manager messages stand out
# bar color and label for each build state; None is still running
STATES = {None: ('running', 'RUNNING'), 'Waiting': ('cancelled', 'WAITING FOR INPUT'),
          'Complete': ('done', '✔ COMPLETE'), 'Failed': ('failed', '✘ FAILED'),
          'Cancelled': ('cancelled', '■ CANCELLED')}


def active():
    return _current is not None


def duration(seconds):
    seconds = max(0, seconds)
    if seconds < 60:
        return f'{seconds:.1f}s'
    minutes, seconds = divmod(int(seconds), 60)
    if minutes < 60:
        return f'{minutes}min {seconds}s'
    hours, minutes = divmod(minutes, 60)
    return f'{hours}h {minutes}min {seconds}s'


def _board():
    return _current if _current else _console


def _panel():
    """The curses panel shows timings itself; other modes print timing lines."""
    return _current is not None and _current.screen is not None


def _elapsed(board, now=None):
    if board.started is None:
        return 0
    end = board.ended if board.ended is not None else (now or time.monotonic())
    return end - board.started


def _init_colors():
    try:
        if not curses.has_colors():
            return
        curses.start_color()
        try:
            curses.use_default_colors()
            background = -1
        except curses.error:
            background = curses.COLOR_BLACK
        colors = (('failed', curses.COLOR_RED), ('cancelled', curses.COLOR_YELLOW),
                  ('done', curses.COLOR_GREEN), ('running', curses.COLOR_BLUE),
                  ('workspace', curses.COLOR_CYAN))
        for number, (status, color) in enumerate(colors, 1):
            curses.init_pair(number, color, background)
            _palette[status] = curses.color_pair(number)
            # reverse video would use the terminal background (purple on Ubuntu) as text
            curses.init_pair(number + len(colors), curses.COLOR_BLACK, color)
            _bars[status] = curses.color_pair(number + len(colors))
    except curses.error:
        pass  # Monochrome terminals still get bold failures.


def style(status):
    """Red failures and yellow cancellations stand out; bold without colors."""
    return _palette.get(status, 0) | (curses.A_BOLD if status in ANSI else 0)


def bar(status):
    """Solid colored bar for build state; plain reverse video without colors."""
    if status in _bars:
        return _bars[status]
    return curses.A_REVERSE | curses.A_BOLD


def _ansi(text, status):
    if (status not in ANSI or _current or os.environ.get('NO_COLOR') or not sys.stdout.isatty()
            or os.environ.get('TERM', '') in ('', 'dumb', 'unknown')):
        return text
    return f'\x1b[{ANSI[status]}m{text}\x1b[0m'


def panel_rows(board, height, result=None):
    """The stage panel as (text, attribute) rows: header, stages, and a spacer."""
    items = list(board.stages.values())
    if not items:
        return []
    # short terminals only get the interesting stage
    if height < 13:
        items = [next((s for s in items if s['status'] in ('running', 'failed', 'cancelled')), items[-1])]
    now = time.monotonic()
    done = sum(s['status'] in ('done', 'skipped') for s in board.stages.values())
    if result is None and board.attention:
        result = 'Waiting'
    color, label = STATES[result]
    final = result in ('Complete', 'Failed', 'Cancelled')

    rows = [(f' {label}  {done}/{len(board.stages)} stages finished | '
             f'{duration(_elapsed(board, now))} {"total" if final else "elapsed"}', bar(color))]
    if board.attention and not final:
        rows.append((f'>>> {board.attention}', curses.A_BOLD | _palette.get('cancelled', 0)))
    for item in items:
        text = f'[{item["status"]:9}] {item["label"]}'
        if timing(item, now):
            text += f' [{timing(item, now)}]'
        if item['detail']:
            text += ': ' + item['detail']
        if item['status'] == 'running' and item['detail'] and item.get('step', 0) > item.get('started', 0):
            text += f' [step {duration(now - item["step"])}]'
        rows.append((text, curses.A_REVERSE if item['status'] == 'running' else style(item['status'])))
    return rows + [('', 0)]


def _footer(screen, rows):
    height, _ = screen.getmaxyx()
    for row, (text, attribute) in enumerate(rows, height - len(rows)):
        _put(screen, row, text, attribute, fill=True)


def timing(item, now=None):
    now = now or time.monotonic()
    if item.get('elapsed') is not None:
        return duration(item['elapsed'])
    if item['status'] == 'running' and item.get('started') is not None:
        return duration(now - item['started'])
    return ''


def _announce(board, item, previous, now):
    if _panel():
        return
    label, status, detail = item['label'], item['status'], item['detail']
    # colcon-like lines: started, step detail, finished/failed, skipped
    if status == 'running' and previous != 'running':
        text = f'=== {label}: started' + (f' ({detail})' if detail else '')
    elif status == 'running':
        if not detail:
            return
        text = f'[{duration(_elapsed(board, now))}] {label}: {detail}'
    elif status in ('done', 'failed', 'cancelled'):
        text = f'=== {label}: ' + {'done': 'finished'}.get(status, status)
        if item.get('elapsed') is not None:
            text += f' [{duration(item["elapsed"])}]'
        if detail:
            text += f' ({detail})'
        text = _ansi(text, status)
    elif status == 'skipped':
        text = f'=== {label}: skipped' + (f' ({detail})' if detail else '')
    else:
        return
    board.beat = now
    print(text, flush=True)


def pipeline(stages):
    board = _board()
    board.stages = {key: dict(label=label, status='waiting', detail='') for key, label in stages}
    board.started = board.beat = time.monotonic()
    board.ended, board.facts, board.runs, board.peak, board.attention = None, {}, {}, None, ''


def attention(text):
    """Pin an instruction for the user (or clear it with ''); the panel turns to waiting."""
    board = _board()
    board.attention = text
    if text and not _panel():
        print(_ansi(f'>>> {text}', 'cancelled'), flush=True)
    if _panel() and _current.running:
        _current._progress()


def operation(title, callback):
    """Run callback with live output and a results page in the TUI; directly otherwise."""
    if _current:
        return _current.operation(title, callback)
    return callback()


def fact(label, value):
    """Add a line to the results summary."""
    _board().facts[label] = value


def stage(key, status, detail=''):
    board = _board()
    now = time.monotonic()
    if board.started is None:
        board.started = board.beat = now
    item = board.stages.setdefault(key, dict(label='GitHub publication' if key == 'publish' else key,
                                             status='waiting', detail=''))

    # stage timer starts/stops on running transitions; step timer restarts on new detail
    previous = item['status']
    changed = status != previous or detail != item['detail']
    if status == 'running' and previous != 'running':
        item.update(started=now, step=now, elapsed=None, line=board.count)
    elif status == 'running' and detail != item['detail']:
        item['step'] = now
    elif status != 'running' and previous == 'running':
        item['elapsed'] = now - item['started']
    item.update(status=status, detail=detail)

    if changed:
        _announce(board, item, previous, now)
    if _panel() and _current.running:
        _current._progress()


def progress_event(line):
    """Turn a generator progress line into the sources stage detail; False if not one."""
    if not line.startswith(PROGRESS):
        return False
    try:
        event = json.loads(line[len(PROGRESS):])
        model = os.path.basename(event.get('model', ''))
        arch = event.get('architecture', '')
        phase = str(event.get('phase', ''))
        detail = ' '.join(str(v) for v in (model, arch, phase) if v)
    except (ValueError, TypeError, AttributeError):
        return False

    # time each model/architecture for the summary
    board = _board()
    if arch and phase.startswith('Generating'):
        board.runs[model, arch] = dict(started=time.monotonic(), elapsed=None, cached=False)
    elif arch and phase == 'Generation complete' and (model, arch) in board.runs:
        run = board.runs[model, arch]
        run.update(elapsed=time.monotonic() - run['started'], cached=bool(event.get('cached')))

    item = board.stages.get('sources')
    if item is not None:
        if item['status'] == 'running':
            stage('sources', 'running', detail)
        else:
            item['detail'] = detail
    return True


class Relay:
    """Forward child output; generator progress events update stages instead of printing."""

    def __init__(self):
        self.pending = ''
        self.midline = False

    def feed(self, text):
        text, self.pending = self.pending + text, ''
        output = []

        # whole lines are checked for events; a trailing partial line that could be one is held back
        while text:
            line, newline, rest = text.partition('\n')
            if self.midline or newline and not progress_event(line):
                output.append(line + newline)
            elif not newline:
                if line.startswith(PROGRESS) or PROGRESS.startswith(line):
                    self.pending = line  # wait for the rest of a possible event
                else:
                    output.append(line)
            self.midline = not newline and not self.pending
            text = rest

        if output:
            _child_print(''.join(output))

    def close(self):
        if self.pending and not progress_event(self.pending):
            _child_print(self.pending)
        self.pending, self.midline = '', False


def _child_print(text):
    """Tag tool output so successful results can show only manager messages."""
    if _current:
        _current.child = True
    try:
        print(text, end='', flush=True)
    finally:
        if _current:
            _current.child = False


def status_line(board, now=None):
    now = now or time.monotonic()
    finished = sum(s['status'] in ('done', 'skipped') for s in board.stages.values())
    parts = [duration(_elapsed(board, now)), f'{finished}/{len(board.stages)} stages finished']
    for item in board.stages.values():
        if item['status'] == 'running':
            parts.append(f'{item["label"]} - {timing(item, now)}')
            if item['detail'] and item.get('step', 0) > item.get('started', 0):
                parts.append(f'{item["detail"]} - {duration(now - item["step"])}')
    return ''.join(f'[{part}]' for part in parts)


def tick():
    """Plain-text heartbeat so long silent steps visibly have not stalled."""
    board = _board()
    now = time.monotonic()
    if board.started is not None and board.ended is None and now - board.sampled >= 2:
        board.sampled = now
        memory = memory_use()
        if memory:
            board.peak = max(board.peak or 0, memory.utilization)

    if _panel() or not any(s['status'] == 'running' for s in board.stages.values()):
        return
    if now - board.beat >= HEARTBEAT:
        board.beat = now
        print(status_line(board, now), flush=True)


def close_stages(status, board=None):
    """Mark interrupted stages and stop their timers."""
    board = board or _board()
    now = time.monotonic()
    for item in board.stages.values():
        if item['status'] == 'running':
            item.update(status=status, elapsed=now - item['started'])
    return now


def summary_lines(board, now=None):
    now = now or time.monotonic()
    lines = []
    for item in board.stages.values():
        text = f'[{item["status"]}] {item["label"]}'
        if timing(item, now):
            text += f' [{timing(item, now)}]'
        lines.append(text + (f': {item["detail"]}' if item['detail'] else ''))
    if board.started is not None:
        lines.append(f'Total elapsed: {duration(_elapsed(board, now))}')
    return lines


def run_text(key, run):
    model, arch = key
    if run['elapsed'] is None:
        return f'{os.path.splitext(model)[0]} {arch}: unfinished'
    return f'{os.path.splitext(model)[0]} {arch}: {"cached" if run["cached"] else "generated"} [{duration(run["elapsed"])}]'


def _track(item, board, width):
    """Timeline segments for one stage, scaled to the whole run."""
    total = max(_elapsed(board), 1e-9)
    if item.get('started') is None:
        return []
    start = int((item['started'] - board.started) / total * width)
    length = max(1, round((timing_seconds(item)) / total * width))
    return [(start, '█' * min(length, width - start), _palette.get(item['status'], 0))]


def timing_seconds(item):
    if item.get('elapsed') is not None:
        return item['elapsed']
    return time.monotonic() - item['started']


def summary_rows(board, width, alert='', stopped=None, tail=(), tool_lines=0, warnings=0):
    """Glanceable results: error, stage timeline, generation times and facts, side by side when wide."""
    heading = curses.A_BOLD
    track = max(10, min(40, (width // 2 if width >= 120 else width) - 40))

    timeline = [[(0, 'Time by stage', heading)]]
    for item in board.stages.values():
        row = [(0, item['label'][:26], 0)]
        if item['status'] in ('skipped', 'waiting'):
            text = 'skipped' + (f': {item["detail"]}' if item['detail'] else '') if item['status'] == 'skipped' else 'not run'
            row.append((27, text, curses.A_DIM))
        else:
            row.append((27, '·' * track, curses.A_DIM))
            row += [(27 + start, text, attribute) for start, text, attribute in _track(item, board, track)]
            row.append((28 + track, timing(item).rjust(10), style(item['status'])))
        timeline.append(row)

    if board.runs:
        timeline += [[], [(0, 'MATLAB generation', heading)]]
        for (model, arch), run in board.runs.items():
            state = 'unfinished' if run['elapsed'] is None else 'cached' if run['cached'] else 'generated'
            timeline.append([(0, os.path.splitext(model)[0][:26], 0), (27, arch, 0),
                             (36, state, _palette.get('done', 0) if state == 'cached' else 0),
                             (48, duration(run['elapsed']) if run['elapsed'] is not None else '', 0)])

    facts = dict(board.facts)
    if board.peak is not None:
        facts['Peak RAM use'] = f'{board.peak:.0%}'
    if tool_lines:
        facts['Tool output'] = f'{tool_lines} lines' + (f', {warnings} with warnings' if warnings else '')
    home = os.path.expanduser('~')
    column = max([21] + [len(label) + 2 for label in facts])
    details = [[(0, 'Details', heading)]] + [[(0, label, curses.A_DIM), (column, str(value).replace(home, '~'), 0)]
                                             for label, value in facts.items()]

    # side by side when there is room
    rows = []
    if alert:
        attribute = style('cancelled' if stopped and stopped['status'] == 'cancelled' else 'failed')
        # keep the message's own lines; indented details stay indented when they wrap
        for paragraph in alert.splitlines():
            indent = paragraph[:len(paragraph) - len(paragraph.lstrip())]
            rows += [[(0, line, attribute)] for line in textwrap.wrap(
                paragraph.strip(), max(20, width - 1), initial_indent=indent, subsequent_indent=indent,
                break_on_hyphens=False)]
        rows.append([])
    if width >= 120 and len(facts):
        offset = 40 + track
        for i in range(max(len(timeline), len(details))):
            left = timeline[i] if i < len(timeline) else []
            right = details[i] if i < len(details) else []
            rows.append(left + [(offset + column, text, attribute) for column, text, attribute in right])
    else:
        rows += timeline + ([[]] + details if facts else [])

    if tail:
        rows += [[], [(0, f'Last output from {stopped["label"]}', heading)]]
        rows += [[(0, line, curses.A_DIM if line.startswith(GUTTER) else 0)] for line in tail]
    return rows


def summary(status=None):
    """CLI equivalent of colcon's closing summary; the TUI shows it on the results page."""
    if _current or not _console.stages:
        return
    now = close_stages(status) if status else time.monotonic()

    # stage labels grouped by final status
    counts = {}
    for item in _console.stages.values():
        counts.setdefault(item['status'], []).append(item['label'])
    text = (f'=== Summary: {len(counts.get("done", []))} stages finished'
            + (f', {len(counts["skipped"])} skipped' if counts.get('skipped') else '')
            + f' [{duration(_elapsed(_console, now))}]')
    for state in ('failed', 'cancelled'):
        if counts.get(state):
            text += '\n' + _ansi(f'    {len(counts[state])} {state}: ' + ', '.join(counts[state]), state)
    for key, run in _console.runs.items():
        text += f'\n    {run_text(key, run)}'
    for label, value in _console.facts.items():
        text += f'\n    {label}: {value}'

    print(text, flush=True)
    _console.stages, _console.started = {}, None


def check_cancel():
    tick()
    if _current:
        _current.check_cancel()


def wait(seconds):
    """Keep progress/cancellation responsive while waiting for device authorization."""
    if not _current:
        tick()
        return time.sleep(seconds)

    # short sleeps so q/Esc still cancels
    end = time.monotonic() + seconds
    while time.monotonic() < end:
        check_cancel()
        time.sleep(min(0.1, max(0, end - time.monotonic())))


def confirm(question):
    if _current:
        try:
            return choose(question, ['Cancel', 'Confirm'])[0] == 1
        except Back:
            return False
    if not sys.stdin.isatty():
        raise ValueError('Confirmation requires a terminal; use explicit selections and --yes')
    return input(question + ' [y/N] ').strip().lower() in ('y', 'yes')


def choose(title, labels, multiple=False):
    if not labels:
        raise ValueError('No models/options available')
    if _current:
        return _current.choose(title, labels, multiple)
    if not sys.stdin.isatty() or not sys.stdout.isatty():
        raise ValueError('Interactive selection requires a terminal; pass model names or --all')

    # curses menu, or numbered prompts if the terminal can't do it
    try:
        if os.environ.get('TERM', '') in ('', 'dumb', 'unknown'):
            raise curses.error('Terminal has no cursor-addressing capabilities')
        return curses.wrapper(_menu, title, labels, multiple)
    except curses.error:
        return _plain_choose(title, labels, multiple, KeyboardInterrupt)


def sections(title, groups, selected=0):
    """Choose an option on one page; section headings are never selectable."""
    labels = [label for _, options in groups for label in options]
    if not labels:
        raise ValueError('No options available')
    if _current:
        if _current.screen is not None:
            return _section_menu(_current.screen, title, groups, selected)
        return _plain_choose(title, labels, False, Back, groups)[0]
    if not sys.stdin.isatty() or not sys.stdout.isatty():
        raise ValueError('Interactive selection requires a terminal')

    try:
        if os.environ.get('TERM', '') in ('', 'dumb', 'unknown'):
            raise curses.error('Terminal has no cursor-addressing capabilities')
        return curses.wrapper(_section_menu, title, groups, selected)
    except curses.error:
        return _plain_choose(title, labels, False, Back, groups)[0]


def home(title, workspace, labels):
    """Home page: workspace bar above the actions. Returns -1 for the workspace, else the action."""
    if _current and _current.screen is not None:
        return _home_menu(_current.screen, title, workspace, labels)

    # numbered fallback: 0 is the workspace so actions keep their numbers
    print(f'{title}\n  0. Workspace: {workspace}')
    for i, label in enumerate(labels, 1):
        print(f'  {i}. {label}')
    while True:
        answer = input('Selection (0 changes the workspace; q to quit): ').strip()
        if answer.lower() in ('q', 'b'):
            raise KeyboardInterrupt
        if answer.isdigit() and int(answer) <= len(labels):
            return int(answer) - 1
        print('Choose a listed number, or q to quit.')


def _home_menu(screen, title, workspace, labels):
    screen.keypad(True)
    screen.timeout(-1)
    cursor = 0  # -1 is the workspace bar
    while True:
        screen.erase()
        _, width = screen.getmaxyx()
        _put(screen, 0, title, curses.A_BOLD)
        _put(screen, 1, 'Arrows: move | Enter: select | w: workspace | q/Esc: quit')
        marker = '> ' if cursor == -1 else '  '
        _put(screen, 3, f'{marker}Workspace  {workspace}', bar('workspace') | curses.A_BOLD * (cursor == -1),
             fill=True)
        _put(screen, 4, '─' * (width - 1), _palette.get('workspace', 0))
        for i, label in enumerate(labels):
            _put(screen, i + 5, label, curses.A_REVERSE if i == cursor else curses.A_NORMAL)
        screen.refresh()

        key = screen.getch()
        if key in (ord('q'), ord('b'), 27):
            raise KeyboardInterrupt
        if key == ord('w'):
            return -1
        if key in (curses.KEY_DOWN, ord('j')):
            cursor = cursor + 1 if cursor < len(labels) - 1 else -1
        elif key in (curses.KEY_UP, ord('k')):
            cursor = cursor - 1 if cursor > -1 else len(labels) - 1
        elif key in (10, 13, curses.KEY_ENTER):
            return cursor


def workspace(current):
    """Pick a workspace directory: typed with Tab completion, or from presets."""
    if _current:
        return _current.workspace(current)
    raise ValueError('Choosing a workspace requires the interactive manager')


def _plain_choose(title, labels, multiple, cancel, groups=None):
    print(title)
    if groups is None:
        for i, label in enumerate(labels, 1):
            print(f'{i}. {label}')
    else:
        number = 1
        for heading, options in groups:
            if heading:
                print('\n' + heading)
            for label in options:
                print(f'  {number}. {label}')
                number += 1

    while True:
        answer = input('Selection (numbers separated by spaces; b/q to go back): ').strip()
        if answer.lower() in ('q', 'b'):
            raise cancel
        try:
            indices = list(dict.fromkeys(int(x) - 1 for x in answer.split()))
            if (not indices or any(i < 0 or i >= len(labels) for i in indices)
                    or (not multiple and len(indices) != 1)):
                raise ValueError
            return indices
        except ValueError:
            print('Choose a listed number, or b to go back.')


def _wrap(lines, width):
    """Wrap long lines to the screen, keeping tool output's gutter on continuations."""
    wrapped = []
    for line in lines:
        if not isinstance(line, str) or len(line) < width:
            wrapped.append(line)
            continue
        prefix = GUTTER if line.startswith(GUTTER) else '  '
        body = line[len(GUTTER):] if prefix == GUTTER else line
        first = width - 1 - (len(GUTTER) if prefix == GUTTER else 0)
        chunks = [body[:first]] + [body[i:i + width - 1 - len(prefix)]
                                   for i in range(first, len(body), width - 1 - len(prefix))]
        wrapped += [(GUTTER if prefix == GUTTER else '') + chunks[0]] + [prefix + chunk for chunk in chunks[1:]]
    return wrapped


def _put(screen, row, text, attribute=0, column=0, fill=False):
    height, width = screen.getmaxyx()
    if 0 <= row < height:
        text = str(text).replace('\t', '    ')
        if fill:
            text = text.ljust(width - 1 - column)  # bars span the whole row
        try:
            screen.addnstr(row, column, text, max(0, width - 1 - column), attribute)
        except curses.error:
            pass  # Resizing can change the dimensions between getmaxyx and addnstr.


def _frame(screen, bottom):
    """Yellow ACTION NEEDED box around a mid-build menu or prompt."""
    _, width = screen.getmaxyx()
    try:
        box = screen.derwin(bottom, width, 0, 0)
        box.attrset(curses.A_BOLD | _palette.get('cancelled', 0))
        box.box()
        box.addstr(0, 2, ' ACTION NEEDED ', bar('cancelled'))
    except curses.error:
        pass  # Too small to frame; the yellow panel bar still shows it.


def _menu(screen, title, labels, multiple, cancel=KeyboardInterrupt, footer=()):
    screen.keypad(True)
    screen.timeout(-1)
    cursor, selected = 0, set()
    # a footer means a build is waiting on this answer, so it gets framed
    top, left = (1, 2) if footer else (0, 0)
    while True:
        # draw, scrolled so the cursor stays visible
        screen.erase()
        height, _ = screen.getmaxyx()
        _put(screen, top, title, curses.A_BOLD, left)
        _put(screen, top + 1, 'Arrows: move | Space: select | Enter: accept | b/q/Esc: back', 0, left)
        count = max(1, height - 4 - len(footer) - 2 * top)
        start = max(0, cursor - count + 1)
        for i in range(start, min(len(labels), start + count)):
            prefix = ('[x] ' if i in selected else '[ ] ') if multiple else ''
            _put(screen, i - start + top + 3, prefix + labels[i],
                 curses.A_REVERSE if i == cursor else curses.A_NORMAL, left)
        if footer:
            _frame(screen, height - len(footer))
        _footer(screen, footer)
        screen.refresh()

        key = screen.getch()
        if key in (ord('q'), ord('b'), 27):
            raise cancel
        if key in (curses.KEY_DOWN, ord('j')):
            cursor = (cursor + 1) % len(labels)
        elif key in (curses.KEY_UP, ord('k')):
            cursor = (cursor - 1) % len(labels)
        elif key == ord(' ') and multiple:
            selected.symmetric_difference_update({cursor})
        elif key in (10, 13, curses.KEY_ENTER):
            return sorted(selected) if selected else [cursor]


def _section_menu(screen, title, groups, selected=0):
    # flat row list; positions maps option index -> row
    rows, positions = [], []
    for heading, options in groups:
        if heading:
            rows.append((heading, True))
        for label in options:
            positions.append(len(rows))
            rows.append(('  ' + label, False))
    cursor = max(0, min(selected, len(positions) - 1))

    screen.keypad(True)
    screen.timeout(-1)
    while True:
        screen.erase()
        height, _ = screen.getmaxyx()
        _put(screen, 0, title, curses.A_BOLD)
        _put(screen, 1, 'Arrows: move | Enter: edit/select | b/q/Esc: back')
        count = max(1, height - 4)
        start = max(0, positions[cursor] - count + 1)
        for i in range(start, min(len(rows), start + count)):
            label, heading = rows[i]
            attribute = (curses.A_REVERSE if i == positions[cursor] else
                         curses.A_BOLD if heading else curses.A_NORMAL)
            _put(screen, i - start + 3, label, attribute)
        _put(screen, height - 1,
             f'Option {cursor + 1}/{len(positions)} | PgUp/PgDn: scroll | Home/End: first/last')
        screen.refresh()

        key = screen.getch()
        if key in (ord('q'), ord('b'), 27):
            raise Back
        if key in (curses.KEY_DOWN, ord('j')):
            cursor = (cursor + 1) % len(positions)
        elif key in (curses.KEY_UP, ord('k')):
            cursor = (cursor - 1) % len(positions)
        elif key == curses.KEY_NPAGE:
            cursor = min(len(positions) - 1, cursor + count)
        elif key == curses.KEY_PPAGE:
            cursor = max(0, cursor - count)
        elif key == curses.KEY_HOME:
            cursor = 0
        elif key == curses.KEY_END:
            cursor = len(positions) - 1
        elif key in (10, 13, curses.KEY_ENTER):
            return cursor


def models(title, primary, excluded, multiple=False, browse=False):
    """Model rows plus an initially collapsed referenced/excluded section.

    Selection indices refer to primary + excluded, independent of expansion.
    """
    if not excluded:
        if browse:
            return page(title, '\n'.join(primary) or 'No models found. Change the workspace in Settings.')
        return choose(title, primary, multiple)

    screen = _current.screen if _current else None
    try:
        if screen is not None:
            return _model_menu(screen, title, primary, excluded, multiple, browse)

        # plain fallback: the toggle row is one more numbered option
        expanded, selected = False, set()
        while True:
            labels = list(primary) + [f'{"v" if expanded else ">"} Referenced/excluded models ({len(excluded)})']
            if expanded:
                labels += ['  ' + label for label in excluded]
            try:
                chosen = _plain_choose(title, labels, multiple and not browse, Back)
            except Back:
                if expanded:
                    expanded = False
                    continue
                raise

            if len(primary) in chosen:
                selected.update(i if i < len(primary) else i - 1 for i in chosen if i != len(primary))
                expanded = not expanded
                continue
            if browse:
                return
            selected.update(i if i < len(primary) else i - 1 for i in chosen)
            return sorted(selected)
    except Back:
        if not browse:
            raise


def _model_menu(screen, title, primary, excluded, multiple=False, browse=False):
    screen.keypad(True)
    screen.timeout(-1)
    expanded, cursor, selected = False, 0, set()
    while True:
        # rows are (selection index, label); the toggle row has no index
        rows = [(i, label) for i, label in enumerate(primary)]
        hidden_selected = sum(i >= len(primary) for i in selected)
        suffix = f'; {hidden_selected} selected' if hidden_selected else ''
        rows.append((None, f'{"v" if expanded else ">"} Referenced/excluded models ({len(excluded)}{suffix})'))
        if expanded:
            rows.extend((len(primary) + i, '  ' + label) for i, label in enumerate(excluded))

        screen.erase()
        height, _ = screen.getmaxyx()
        _put(screen, 0, title, curses.A_BOLD)
        _put(screen, 1, 'Arrows: move | Enter: accept/toggle | Space: select/toggle | b/q/Esc: back')
        count = max(1, height - 4)
        start = max(0, cursor - count + 1)
        for i in range(start, min(len(rows), start + count)):
            identity, label = rows[i]
            prefix = (('[x] ' if identity in selected else '[ ] ')
                      if multiple and not browse and identity is not None else '')
            _put(screen, i - start + 3, prefix + label,
                 curses.A_REVERSE if i == cursor else curses.A_NORMAL)
        screen.refresh()

        key = screen.getch()
        identity = rows[cursor][0]
        if key in (ord('q'), ord('b'), 27):
            if expanded:
                expanded, cursor = False, min(cursor, len(primary))
                continue
            raise Back
        if key in (curses.KEY_DOWN, ord('j')):
            cursor = (cursor + 1) % len(rows)
        elif key in (curses.KEY_UP, ord('k')):
            cursor = (cursor - 1) % len(rows)
        elif key == curses.KEY_LEFT and (identity is None or cursor > len(primary)):
            expanded, cursor = False, len(primary)
        elif key == curses.KEY_RIGHT and identity is None:
            expanded = True
        elif key in (10, 13, curses.KEY_ENTER, ord(' ')) and identity is None:
            expanded = not expanded
        elif key == ord(' ') and multiple and not browse:
            selected.symmetric_difference_update({identity})
        elif key in (10, 13, curses.KEY_ENTER):
            return None if browse else (sorted(selected) if selected else [identity])


class Session(Board):
    def __init__(self, screen=None):
        super().__init__()
        self.screen = screen
        self.lines = deque(maxlen=3000)
        self.partial = ''
        self.running = False
        self.title = ''
        self.child = False
        self.kinds = deque(maxlen=3000)  # parallel to lines: True for child-tool output
        self.partial_child = False
        self.tool_lines = 0
        self.warnings = 0
        self.terminal = sys.stdout

    def _footer(self):
        """Keep the stage panel pinned below menus and prompts shown mid-build."""
        if self.screen is None or not self.running:
            return []
        return panel_rows(self, self.screen.getmaxyx()[0], 'Waiting')

    def choose(self, title, labels, multiple=False):
        if self.screen is None:
            return _plain_choose(title, labels, multiple, Back)
        return _menu(self.screen, title, labels, multiple, Back, self._footer())

    def write(self, value):
        # Child tools may emit ANSI escapes/progress carriage returns.
        clean = re.sub(r'\x1b\[[0-?]*[ -/]*[@-~]', '', value).replace('\r', '\n')

        # keep whole lines, tagged by source; the rest waits in partial
        parts = (self.partial + clean).split('\n')
        for line in parts[:-1]:
            if not progress_event(line):
                self.lines.append(line)
                self.kinds.append(self.child)
                self.count += 1
                if self.child:
                    self.tool_lines += 1
                    self.warnings += bool(WARNING.search(line))
        self.partial = parts[-1][-4000:]
        self.partial_child = self.child

        if self.screen is None:
            self.terminal.write(value)
            self.terminal.flush()
        elif self.running:
            self._progress()
        return len(value)

    def flush(self):
        if self.screen is None:
            self.terminal.flush()

    def isatty(self):
        return True

    def _progress(self):
        self.screen.erase()
        height, _ = self.screen.getmaxyx()
        _put(self.screen, 0, self.title, curses.A_BOLD)
        _put(self.screen, 1, 'Working... q/Esc or Ctrl-C: cancel and return to results')
        lines = list(zip(self.lines, self.kinds)) + ([(self.partial, self.partial_child)] if self.partial else [])
        footer = panel_rows(self, height)
        available = max(0, height - 3 - len(footer))
        for row, (line, child) in enumerate(lines[-available:] if available else [], 3):
            _put(self.screen, row, GUTTER + line if child else line, curses.A_DIM if child else 0)
        _footer(self.screen, footer)
        self.screen.refresh()

    def check_cancel(self):
        if self.screen is not None and self.running:
            self.screen.timeout(0)
            key = self.screen.getch()
            if key in (ord('q'), 27):
                raise KeyboardInterrupt
            self._progress()

    def page(self, title, content, back_cancels=False, footer=None, alert='', full=None,
             status='', at_end=False, summary=None):
        """Scrollable page; Tab/L cycles summary, output and full log views, `footer` stays pinned."""
        views = [('Output', str(content).splitlines() or ['(none)'])]
        if full is not None:
            views.append(('Full log', str(full).splitlines() or ['(none)']))
        if summary is not None:
            views.insert(0, ('Summary', summary))

        # plain terminal: print it all and wait for enter
        if self.screen is None:
            print(f'\n{title}' + (f'\n{alert}' if alert else '') + '\n' + '\n'.join(str(content).splitlines()))
            if footer:
                print('\n'.join(text for text, _ in footer))
            answer = input('Enter to continue; b/q to go back: ' if back_cancels else 'Press Enter to go back: ')
            if back_cancels and answer.lower() in ('b', 'q'):
                raise Back
            return

        footer = self._footer() if footer is None else footer
        view = 0
        offset = sys.maxsize if at_end and views[0][0] != 'Summary' else 0
        self.screen.timeout(-1)
        while True:
            height, width = self.screen.getmaxyx()
            name, lines = views[view][0], _wrap(views[view][1], width)
            rows = max(1, height - 4 - len(footer))
            offset = min(offset, max(0, len(lines) - rows))
            self.screen.erase()
            _put(self.screen, 0, title, bar(status) if status else curses.A_BOLD, fill=bool(status))

            # view tabs, then key help
            left = 0
            if len(views) > 1:
                for i, (label, _) in enumerate(views):
                    _put(self.screen, 1, f' {label} ', curses.A_REVERSE if i == view else 0, left)
                    left += len(label) + 3
                left += 1
            _put(self.screen, 1, ('Tab/←/→: view | ' if len(views) > 1 else '') + '↑/↓/PgUp/PgDn: scroll | ' +
                 ('Enter: continue | b/q/Esc: back' if back_cancels else 'Enter/b/q/Esc: back'), 0, left)
            if alert and name != 'Summary':
                _put(self.screen, 2, alert.splitlines()[0], style(status or 'failed'))

            for row, line in enumerate(lines[offset:offset + rows], 3):
                if isinstance(line, str):
                    _put(self.screen, row, line, curses.A_DIM if line.startswith(GUTTER) else 0)
                else:
                    for start, text, attribute in line:
                        _put(self.screen, row, text, attribute, start)
            _footer(self.screen, footer)
            self.screen.refresh()

            key = self.screen.getch()
            if key in (ord('b'), ord('q'), 27) and back_cancels:
                raise Back
            if key in (ord('b'), ord('q'), 27, 10, 13, curses.KEY_ENTER):
                return
            # long lines wrap, so left/right switch views like tabs
            if key in (9, ord('l'), ord('L'), curses.KEY_RIGHT, curses.KEY_LEFT) and len(views) > 1:
                view = (view + (-1 if key == curses.KEY_LEFT else 1)) % len(views)
                offset = sys.maxsize if at_end and views[view][0] != 'Summary' else 0
            elif key == curses.KEY_DOWN:
                offset += 1
            elif key == curses.KEY_UP:
                offset = max(0, offset - 1)
            elif key == curses.KEY_NPAGE:
                offset += rows
            elif key == curses.KEY_PPAGE:
                offset = max(0, offset - rows)
            elif key == curses.KEY_END:
                offset = len(lines)
            elif key == curses.KEY_HOME:
                offset = 0

    def prompt(self, title, default=''):
        if self.screen is None:
            value = input(f'{title} [{default}] (b: back; -: clear): ')
            if value == 'b':
                raise Back
            return '' if value == '-' else (value or str(default))

        value = str(default)
        self.screen.timeout(-1)
        while True:
            self.screen.erase()
            height, width = self.screen.getmaxyx()
            footer = self._footer()
            top, left = (1, 2) if footer else (0, 0)
            _put(self.screen, top, title, curses.A_BOLD, left)
            _put(self.screen, top + 1, 'Enter: accept | Esc: back | Ctrl-U: clear', 0, left)
            _put(self.screen, top + 3, '> ' + value[-max(1, width - 5 - 2 * left):] + '▏', 0, left)
            if footer:
                _frame(self.screen, height - len(footer))
            _footer(self.screen, footer)
            self.screen.refresh()

            key = self.screen.get_wch()
            if key in ('\n', '\r'):
                return value.strip()
            if key == '\x1b':
                raise Back
            if key == '\x15':
                value = ''
            elif key in ('\b', '\x7f', curses.KEY_BACKSPACE):
                value = value[:-1]
            elif isinstance(key, str) and key.isprintable():
                value += key

    def workspace(self, current):
        if self.screen is None:
            presets = [workspaces.label(p) for p in workspaces.recent()]
            for i, preset in enumerate(presets, 1):
                print(f'  {i}. {preset}')
            while True:
                value = input(f'Workspace [{current}] (number or path; b: back): ').strip()
                if value == 'b':
                    raise Back
                if value.isdigit() and 0 < int(value) <= len(presets):
                    value = presets[int(value) - 1]
                path = os.path.expanduser(value or current)
                if os.path.isdir(path):
                    return path
                print(f'Not a directory: {value}')

        value, edited, cursor, error = current, False, -1, ''
        self.screen.timeout(-1)
        while True:
            # live lists: presets filtered once edited, then directory matches
            presets = [workspaces.label(p) for p in workspaces.recent()]
            if edited:
                presets = [p for p in presets if value.strip() in p]
            height, width = self.screen.getmaxyx()
            footer = self._footer()
            top, left = (1, 2) if footer else (0, 0)

            # recent gets at most half the room, matches the rest
            room = max(2, height - 9 - len(footer) - 2 * top)
            presets = presets[:max(1, room // 2)]
            found = workspaces.matches(value)[:max(1, room - len(presets))]
            rows = [(p, True) for p in presets] + [(m, False) for m in found]
            cursor = min(cursor, len(rows) - 1)

            self.screen.erase()
            _put(self.screen, top, 'Model search workspace', curses.A_BOLD, left)
            _put(self.screen, top + 1, 'Tab: complete | Up/Down: pick | Enter: accept | Del: forget preset | '
                 'Esc: back', 0, left)
            _put(self.screen, top + 3, '> ' + value[-max(1, width - 5 - 2 * left):] + '▏',
                 curses.A_BOLD if cursor == -1 else 0, left)
            _put(self.screen, top + 4, error, style('failed'), left)

            row = top + 6
            for heading, entries, offset in (('Recent workspaces', presets, 0),
                                             ('Matching directories', found, len(presets))):
                _put(self.screen, row, heading, curses.A_BOLD, left)
                row += 1
                for i, entry in enumerate(entries, offset):
                    _put(self.screen, row, '  ' + entry, curses.A_REVERSE if i == cursor else 0, left)
                    row += 1
                if not entries:
                    _put(self.screen, row, '  (none)', curses.A_DIM, left)
                    row += 1
                row += 1
            if footer:
                _frame(self.screen, height - len(footer))
            _footer(self.screen, footer)
            self.screen.refresh()

            key = self.screen.get_wch()
            if key == '\x1b':
                raise Back
            if key == curses.KEY_DOWN:
                cursor = cursor + 1 if cursor < len(rows) - 1 else -1
            elif key == curses.KEY_UP:
                cursor = cursor - 1 if cursor > -1 else len(rows) - 1
            elif key == '\t':
                value = rows[cursor][0] if cursor >= 0 else workspaces.complete(value)
                edited, cursor, error = True, -1, ''
            elif key == curses.KEY_DC and cursor >= 0 and rows[cursor][1]:
                workspaces.forget(rows[cursor][0])
            elif key in ('\n', '\r', curses.KEY_ENTER):
                chosen = rows[cursor][0] if cursor >= 0 else value.strip()
                path = os.path.expanduser(chosen)
                if os.path.isdir(path):
                    return path
                error = f'Not a directory: {chosen}'
            else:
                # editing always returns focus to the input
                if key == '\x15':
                    value = ''
                elif key in ('\b', '\x7f', curses.KEY_BACKSPACE):
                    value = value[:-1]
                elif isinstance(key, str) and key.isprintable():
                    value += key
                else:
                    continue
                edited, cursor, error = True, -1, ''

    def operation(self, title, callback):
        self.lines.clear()
        self.kinds.clear()
        self.partial = ''
        self.title = title
        self.stages, self.started, self.ended, self.count = {}, None, None, 0
        self.facts, self.runs, self.peak, self.tool_lines, self.warnings = {}, {}, None, 0, 0
        self.running = True

        result, alert = 'Complete', ''
        try:
            callback()
        except (KeyboardInterrupt, Back):
            result, alert = 'Cancelled', 'Cancelled. Returned to the manager; no generation fallback triggered.'
            self._finish_active('cancelled')
            print(alert)
        except (ValueError, OSError, RuntimeError, subprocess.CalledProcessError) as error:
            result, alert = 'Failed', f'Error: {error}'
            self._finish_active('failed')
            print(alert)
        finally:
            self.running = False
            self.ended = time.monotonic()

        # pick what the results page shows first
        log = [GUTTER + line if child else line for line, child in zip(self.lines, self.kinds)]
        log += [GUTTER + self.partial if self.partial_child else self.partial] if self.partial else []
        content, full = log, None
        stopped = next((s for s in self.stages.values()
                        if s['status'] in ('failed', 'cancelled') and 'line' in s), None)
        if stopped:
            # Only the failing stage's output; earlier stages stay behind L.
            start = max(0, stopped['line'] - (self.count - len(self.lines)))
            content = [f'Output from {stopped["label"]} only (Tab: full log)', ''] + log[start:]
            full = log
        elif result == 'Complete' and self.stages:
            content = [line for line, child in zip(self.lines, self.kinds) if not child]
            content += ['', 'Tool output hidden after success (Tab: full log)']
            full = log

        # the summary shows the stages, so only the status bar stays pinned
        summary = None
        if self.screen is not None:
            height, width = self.screen.getmaxyx()
            footer = panel_rows(self, height, result)[:1] + [('', 0)]
            if self.stages:
                tail = [line for line in log[start:] if line != alert][-8:] if stopped else ()
                summary = summary_rows(self, width, alert, stopped, tail, self.tool_lines, self.warnings)
        else:
            footer = [(line, 0) for line in summary_lines(self)]
        color, label = STATES[result]
        icon, word = label.split()
        self.page(f'{icon} {title.upper()} {word}', '\n'.join(content) or result, footer=footer, alert=alert,
                  full=None if full is None else '\n'.join(full), status=color, at_end=result != 'Complete',
                  summary=summary)

    def _finish_active(self, status):
        close_stages(status, self)


def prompt(title, default=''):
    if _current:
        return _current.prompt(title, default)
    return input(title + ': ').strip()


def page(title, content, back_cancels=False):
    if _current:
        return _current.page(title, content, back_cancels=back_cancels)
    print(content)


def run(callback):
    """Own the terminal once; operations and result pages stay in the same session."""
    if not sys.stdin.isatty() or not sys.stdout.isatty():
        raise ValueError('Interactive mode requires a terminal; specify a CLI command')
    started = False

    def start(screen=None):
        nonlocal started
        global _current
        started = True
        if screen is not None:
            _init_colors()
            try:
                curses.curs_set(0)  # otherwise it blinks wherever the last row was drawn
            except curses.error:
                pass  # Some terminals can't hide it.
        session = Session(screen)
        _current = session
        try:
            with redirect_stdout(session), redirect_stderr(session):
                return callback(session)
        finally:
            _current = None

    if os.environ.get('TERM', '') not in ('', 'dumb', 'unknown'):
        previous_delay = curses.get_escdelay()
        try:
            if 'ESCDELAY' not in os.environ:
                # ncurses defaults to a full second before treating a lone Escape
                # as a key rather than the start of a function-key sequence.
                curses.set_escdelay(25)
            return curses.wrapper(start)
        except curses.error:
            if started:
                raise  # Never repeat an operation after a terminal failure.
        finally:
            curses.set_escdelay(previous_delay)
    return start()
