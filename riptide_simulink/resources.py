"""Available CPUs and actual RAM pressure, without external dependencies."""
from dataclasses import dataclass
import os
from pathlib import Path

HIGH_MEMORY = 0.80


def available_cpus():
    try:
        return max(1, len(os.sched_getaffinity(0)))
    except (AttributeError, OSError):
        return max(1, os.cpu_count() or 1)


def job_count(value=None):
    if value is None or value == 'auto':
        return available_cpus()
    if isinstance(value, bool) or not isinstance(value, (int, str)):
        raise ValueError('jobs must be a positive integer or auto')
    jobs = int(value)
    if jobs < 1:
        raise ValueError('jobs must be positive')
    return jobs


@dataclass(frozen=True)
class MemoryUse:
    total: int
    available: int

    @property
    def utilization(self):
        return 1 - self.available / self.total


def memory_use(path=Path('/proc/meminfo')):
    # MemAvailable, not MemFree, so reclaimable cache doesn't count as used
    try:
        values = {}
        for line in Path(path).read_text().splitlines():
            key, value = line.split(':', 1)
            if key in ('MemTotal', 'MemAvailable'):
                values[key] = int(value.split()[0]) * 1024

        total, available = values['MemTotal'], values['MemAvailable']
        if total > 0 and 0 <= available <= total:
            return MemoryUse(total, available)
    except (OSError, ValueError, KeyError):
        pass
    return None


def memory_warning(memory, jobs):
    return (f'High memory use: {memory.utilization:.0%} RAM used, '
            f'{memory.available / 1024**3:.1f} GiB available. '
            f'The build is configured for {jobs} jobs.')
