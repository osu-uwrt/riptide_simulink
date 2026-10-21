import pytest


@pytest.fixture(autouse=True)
def workspace_history(tmp_path, monkeypatch):
    # keep the TUI's remembered workspaces out of the real home directory
    monkeypatch.setenv('XDG_STATE_HOME', str(tmp_path / 'state'))
