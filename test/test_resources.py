"""Parallel-build defaults and opt-in reductions under actual RAM pressure."""
from unittest.mock import patch

import pytest

from riptide_simulink import backend, cli, resources, tui


HIGH = resources.MemoryUse(10 * 1024**3, 1024**3)
LOW = resources.MemoryUse(10 * 1024**3, 8 * 1024**3)


def test_cpu_default_respects_available_affinity_and_fallback():
    with patch.object(resources.os, 'sched_getaffinity', return_value={2, 3, 4}):
        assert resources.available_cpus() == 3
        assert cli.arguments(['build', 'plant']).jobs == 3
        assert backend.build_environment()['CMAKE_BUILD_PARALLEL_LEVEL'] == '3'
        assert backend.build_environment()['MAKEFLAGS'] == '-j3 -l3'

    with patch.object(resources.os, 'sched_getaffinity', side_effect=OSError), \
            patch.object(resources.os, 'cpu_count', return_value=None):
        assert resources.available_cpus() == 1


@pytest.mark.parametrize('value', [0, -1, '0', 'bad', True, 2.5])
def test_invalid_job_count(value):
    with pytest.raises(ValueError):
        resources.job_count(value)


def test_auto_and_explicit_job_count():
    with patch.object(resources, 'available_cpus', return_value=12):
        assert resources.job_count('auto') == resources.job_count() == 12
        assert resources.job_count('4') == resources.job_count(4) == 4


def test_memory_uses_available_not_free_and_handles_missing_data(tmp_path):
    path = tmp_path / 'meminfo'
    path.write_text('MemTotal: 1000 kB\nMemFree: 1 kB\nMemAvailable: 400 kB\n')
    memory = resources.memory_use(path)
    assert memory.total == 1024000 and memory.utilization == pytest.approx(.6)
    assert resources.memory_use(tmp_path / 'missing') is None
    for data in ('MemTotal: 0 kB\nMemAvailable: 0 kB', 'not meminfo',
                 'MemTotal: 100 kB\nMemAvailable: 200 kB'):
        path.write_text(data)
        assert resources.memory_use(path) is None


@pytest.mark.parametrize('memory', [LOW, None])
def test_ordinary_build_has_no_extra_questions(memory):
    args = cli.arguments(['build', 'plant', '--jobs', '8'])
    with patch.object(resources, 'memory_use', return_value=memory), \
            patch.object(cli, 'can_interact', return_value=True), patch.object(tui, 'choose') as choose:
        cli.check_build_memory(args)
        choose.assert_not_called()

    assert args.jobs == 8


def test_high_memory_continue_keeps_full_count_and_does_not_repeat():
    args = cli.arguments(['build', 'plant', '--jobs', '8'])
    with patch.object(resources, 'memory_use', return_value=HIGH), \
            patch.object(cli, 'can_interact', return_value=True), \
            patch.object(tui, 'choose', return_value=[0]) as choose:
        cli.check_build_memory(args)
        cli.check_build_memory(args)

    assert choose.call_count == 1 and args.jobs == 8
    assert '90%' in choose.call_args.args[0]
    assert choose.call_args.args[1][1] == 'Choose a lower job count'


def test_user_can_lower_count_for_this_build():
    args = cli.arguments(['build', 'plant', '--jobs', '8'])
    with patch.object(resources, 'memory_use', return_value=HIGH), \
            patch.object(cli, 'can_interact', return_value=True), \
            patch.object(tui, 'choose', return_value=[1]), patch.object(tui, 'prompt', return_value='3'):
        cli.check_build_memory(args)

    assert args.jobs == 3
    assert backend.build_environment(args.jobs)['CMAKE_BUILD_PARALLEL_LEVEL'] == '3'


def test_invalid_reductions_and_back_navigation():
    args = cli.arguments(['build', 'plant', '--jobs', '8'])
    with patch.object(resources, 'memory_use', return_value=HIGH), \
            patch.object(cli, 'can_interact', return_value=True), patch.object(tui, 'choose', return_value=[1]), \
            patch.object(tui, 'prompt', side_effect=['0', '8', 'text', '2']), patch.object(tui, 'page') as page:
        cli.check_build_memory(args)

    assert args.jobs == 2 and page.call_count == 3
    args = cli.arguments(['build', 'plant', '--jobs', '8'])
    with patch.object(resources, 'memory_use', return_value=HIGH), \
            patch.object(cli, 'can_interact', return_value=True), \
            patch.object(tui, 'choose', side_effect=[[1], [0]]) as choose, \
            patch.object(tui, 'prompt', side_effect=tui.Back()):
        cli.check_build_memory(args)

    assert choose.call_count == 2 and args.jobs == 8


def test_cli_warns_without_prompting_or_changing_explicit_count(capsys):
    args = cli.arguments(['build', 'plant', '--jobs', '5', '--non-interactive'])
    with patch.object(resources, 'memory_use', return_value=HIGH), patch.object(tui, 'choose') as choose:
        cli.check_build_memory(args)
    assert args.jobs == 5
    assert '--jobs N' in capsys.readouterr().err
    choose.assert_not_called()


def test_pressure_that_appears_after_generation_still_warns():
    args = cli.arguments(['build', 'plant', '--jobs', '8'])
    with patch.object(resources, 'memory_use', side_effect=[LOW, HIGH]), \
            patch.object(cli, 'can_interact', return_value=True), \
            patch.object(tui, 'choose', return_value=[0]) as choose:
        cli.check_build_memory(args)
        cli.check_build_memory(args)

    assert choose.call_count == 1


def test_memory_warning_can_cancel_without_generation_or_fallback(tmp_path):
    models = tmp_path / 'models'
    models.mkdir()
    (models / 'plant.slx').touch()
    args = cli.arguments(['build', 'plant', '--workspace', str(tmp_path), '--jobs', '8'])
    with patch.object(resources, 'memory_use', return_value=HIGH), \
            patch.object(cli, 'can_interact', return_value=True), patch.object(tui, 'choose', return_value=[2]), \
            patch.object(backend, 'generate') as generate, patch.object(backend, 'download') as download:
        with pytest.raises(KeyboardInterrupt):
            cli.execute(args)
        generate.assert_not_called()
        download.assert_not_called()


def test_single_job_warning_does_not_offer_invalid_reduction():
    args = cli.arguments(['build', 'plant', '--jobs', '1'])
    with patch.object(resources, 'memory_use', return_value=HIGH), \
            patch.object(cli, 'can_interact', return_value=True), patch.object(tui, 'choose', return_value=[0]) as choose:
        cli.check_build_memory(args)

    assert choose.call_args.args[1] == ['Continue with 1 job', 'Cancel build']
