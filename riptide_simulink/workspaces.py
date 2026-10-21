"""Remembered model search workspaces and path completion for the TUI."""
import json
import os
from pathlib import Path

from .artifacts import atomic_json

LIMIT = 10


def state_file():
    base = os.environ.get('XDG_STATE_HOME') or Path.home() / '.local/state'
    return Path(base) / 'riptide_simulink/workspaces.json'


def recent():
    """Saved workspaces, newest first; the first is the last one used."""
    try:
        entries = json.loads(state_file().read_text())['recent']
        return [Path(e) for e in entries if isinstance(e, str) and e]
    except (OSError, ValueError, KeyError, TypeError):
        return []


def _save(paths):
    try:
        atomic_json(state_file(), {'recent': [str(p) for p in paths[:LIMIT]]})
    except OSError:
        pass  # history is a convenience; never fail an action over it


def remember(path):
    path = Path(path).expanduser().resolve()
    _save([path] + [p for p in recent() if p != path])
    return path


def forget(path):
    path = Path(path).expanduser().resolve()
    _save([p for p in recent() if p != path])


def last():
    saved = recent()
    return saved[0] if saved and saved[0].is_dir() else None


def label(path):
    """Display form, with the home directory shortened to ~."""
    text, home = str(path), str(Path.home())
    if text == home or text.startswith(home + os.sep):
        return '~' + text[len(home):]
    return text


def matches(text):
    """Directories that complete the typed text, shown as typed (~ kept) with a trailing /."""
    if not text:
        return []
    expanded = os.path.expanduser(text)
    folder, prefix = os.path.split(expanded)
    shown = text[:len(text) - len(prefix)]
    try:
        names = sorted(e.name for e in os.scandir(folder or '.')
                       if e.is_dir() and e.name.startswith(prefix)
                       and (prefix.startswith('.') or not e.name.startswith('.')))
    except OSError:
        return []
    return [shown + name + '/' for name in names]


def complete(text):
    """Tab: fill in a single match, or extend to the matches' common prefix."""
    found = matches(text)
    if not found:
        return text
    common = os.path.commonprefix(found)
    return common if len(common) > len(text) else text
