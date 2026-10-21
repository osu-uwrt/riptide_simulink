"""Serialize manager mutations and MATLAB sessions for the current user."""
from contextlib import contextmanager
import fcntl
import os
from pathlib import Path
import tempfile


@contextmanager
def manager_lock():
    path = Path(tempfile.gettempdir()) / f'riptide-simulink-{os.getuid()}.lock'
    # one lock file per user; flock releases it if we die
    fd = os.open(path, os.O_CREAT | os.O_RDWR | os.O_NOFOLLOW, 0o600)
    try:
        try:
            fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError as error:
            raise RuntimeError('Another model manager is active; wait for it to finish before starting '
                               'another build or cleanup') from error
        yield
    finally:
        os.close(fd)
