# riptide_simulink

Tooling for Simulink models in the riptide stack. One ROS 2 Humble package that:

- **finds** Simulink models in the workspace,
- **generates** a ROS 2 C++ source package from each model with local MATLAB R2023a, for x86_64 and arm64,
- **stages** the packages into the development workspace (compiled) and the release workspace (sources for the robot),
- **shares** builds through GitHub releases, so teammates without MATLAB can download them,
- **supervises** the running model nodes from C++ overseers.

## Install

Clone into the development workspace, then install dependencies and build:

```bash
cd ~/osu-uwrt/development/software/src
git clone git@github.com:osu-uwrt/riptide_simulink.git

sudo apt install python3-keyring    # remembers GitHub logins; rosdep has no rule for it
source /opt/ros/humble/setup.bash
cd ~/osu-uwrt/development/software
rosdep install --from-paths src/riptide_simulink --ignore-src -r -y
colcon build --packages-select riptide_simulink
source install/setup.bash
```

You also need `git` and colcon on your `PATH`. Generating models needs MATLAB R2023a with Simulink, ROS Toolbox, Simulink Coder and Embedded Coder; without MATLAB you can still download builds that someone else published.

## Quick tour

Start the manager with no arguments to open the TUI:

```bash
ros2 run riptide_simulink model_manager.py
```

The usual flow is **Build models → select models → Build**. Arrows move, Space selects, Enter accepts, `b`/Escape goes back, and `q` at the home menu quits. The bar at the top of the home page shows the workspace being searched for models; select it to switch, with Tab completion and remembered presets. When a build finishes, choose **Update GitHub model release** to share it.

Every action also works as a one-shot command:

```bash
ros2 run riptide_simulink model_manager.py list                          # models the manager can see
ros2 run riptide_simulink model_manager.py build complete_controller     # generate, stage and compile
ros2 run riptide_simulink model_manager.py download complete_controller  # use the published build instead
ros2 run riptide_simulink model_manager.py --help
```

Generated packages land in `src/simulink_models/` of the development and release workspaces. Run a model's node with `ros2 run <model> <model>`, e.g. `ros2 run complete_controller complete_controller`.

## Documentation

| Guide | Covers |
|---|---|
| [Adding a model](docs/ADDING_MODELS.md) | Onboarding a new Simulink model: the `models/` folder, `simulink.yaml`, first build |
| [Using the manager](docs/USAGE.md) | TUI, CLI commands, build routes, MATLAB settings, outputs, caching and cleanup |
| [Publishing](docs/PUBLISHING.md) | Uploading builds to GitHub releases, sign-in, and the one-time GitHub App setup |
| [C++ model wrapper](docs/CPP_WRAPPER.md) | Supervising generated model nodes from an overseer |

Worked examples live in [examples/](examples/): a [manifest template](examples/simulink.yaml), the [riptide_control manifest](examples/riptide_control.simulink.yaml), and an [overseer](examples/overseer.cpp).

## Testing

```bash
cd ~/osu-uwrt/development/software/src/riptide_simulink
PYTEST_DISABLE_PLUGIN_AUTOLOAD=1 python3 -m pytest -q test/test_manager.py test/test_publishing.py \
  test/test_interactive.py test/test_resources.py test/test_workspaces.py

cd ~/osu-uwrt/development/software
source /opt/ros/humble/setup.bash
colcon build --packages-select riptide_simulink
colcon test --packages-select riptide_simulink
colcon test-result --verbose
```

`PYTEST_DISABLE_PLUGIN_AUTOLOAD` keeps unrelated globally installed pytest plugins out of the run; a clean environment doesn't need it. The MATLAB configuration regression is run by hand: add `matlab/` and `test/` to the R2023a path, then run `test_configuration`.

ARM64 sources have been validated by cross-compiling against the official ARM64 Humble image, not by running on a robot. MATLAB R2023a prints a harmless `LibDictionaryDependencyInfo` destructor warning when closing models.
