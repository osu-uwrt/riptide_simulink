# Adding a model

This guide takes a Simulink model from "it runs in MATLAB" to "the manager builds it, stages it and can publish it". The controller in `riptide_control/riptide_controllers/models/` is the reference setup; follow it when in doubt.

## What the manager can build

The manager uses ROS Toolbox to turn a **top-level Simulink model** into a standalone ROS 2 C++ package. That means the model:

- talks to the rest of the robot through ROS 2 blocks (Subscribe, Publish, parameter blocks and so on),
- is a normal model, not a library, and builds with Embedded Coder (ERT),
- gets one ROS 2 package per model; the package and its executable are both named after the model.

It is not a converter for unsupported blocks. A model that only produces plain C++ for a hand-written node to call doesn't fit this workflow.

## 1. Put the model in a `models/` folder

The manager only looks for models inside folders named exactly `models`. A `.slx` anywhere else (for example `matlab/` or `simulink/`) is never listed.

```
riptide_control/riptide_controllers/
└── models/                              ← the "models root"
    ├── simulink.yaml                    ← optional manifest (step 3)
    ├── Riptide_control_models.prj       ← MATLAB project, if you use one
    ├── x86_cfg.m, arm_cfg.m             ← optional configuration factories
    ├── complete_system/
    │   └── complete_controller.slx      ← top-level model: listed and selected by default
    └── referenced_models/
        └── Velocity_Profiles.slx        ← referenced model: hidden by default
```

The rules:

- Models can be at any depth below `models/`. The nearest enclosing folder named `models` is the models root, and that is where the manager looks for `simulink.yaml`.
- Put model references and other helper models in `referenced_models/`. They are hidden from the default selection (but can still be selected by name). Other folders can be hidden with `exclude` in the manifest.
- Build and cache folders are skipped: `build`, `install`, `log`, `slprj`, `*_ert_rtw`, `codegen`, `cache`, `generated_models`, `simulink_models`, `controller_models` and the like. Don't put source models in them.
- Symbolic links are not followed.
- The search starts at `~/osu-uwrt/development/software/src` (change it with `--workspace`).

## 2. Name and prepare the model

- **Name.** The model's file name becomes the ROS package and executable name, so use lowercase `snake_case` (`depth_controller.slx`, not `DepthController.slx`). It must not clash with an existing ROS package in the workspace; the manager refuses to stage over a package it doesn't own. If two models share a name, select them by workspace-relative path instead.
- **ROS 2 interfaces.** Use ROS Toolbox ROS 2 blocks for all inputs and outputs. If the model uses custom messages (for example from `riptide_msgs`), list their packages under `messages` in the manifest (step 3).
- **Configuration.** You don't need to switch the model to the ROS 2 target yourself. For each build the generator temporarily selects the ROS 2 hardware board, the right processor (x86-64 or ARM64) and ERT, and it never saves your model. An existing ERT configuration is kept as is. If the model's settings differ per architecture, write configuration factory functions (like `x86_cfg.m`/`arm_cfg.m`) and name them under `configurations`.
- **Model data.** Generation runs in a fresh batch MATLAB that skips your personal `startup.m`. Everything the model needs (workspace variables, data dictionaries, libraries, custom code paths) must come from the MATLAB project or a `setup` function, using paths relative to the repository, never your home directory.
- **Commit it.** Builds record the commit they came from. Only builds of clean, committed sources can be published without an explicit override.

## 3. Add `simulink.yaml` if you need it

Without a manifest the manager opens the nearest `.prj` found by walking up from the model, and uses the repository's `origin` remote as the GitHub repository. That is enough for a simple model.

Add `models/simulink.yaml` when you need any of: custom messages, a setup function, per-architecture configurations, a specific project (more than one `.prj` is an error), a different GitHub repository, or to hide models. Start from [the template](../examples/simulink.yaml); the controller's real one is [riptide_control.simulink.yaml](../examples/riptide_control.simulink.yaml):

```yaml
version: 1
defaults:                                   # apply to every model under this models/ folder
  project: Riptide_control_models.prj
  messages:
    - ../../../riptide_core/riptide_msgs    # ROS interface package directories
  configurations:
    x86_64: x86_cfg                         # functions returning a ConfigSet
    arm64: arm_cfg
  github:
    repository: osu-uwrt/riptide_control
    release: latest                         # what downloads use
    publish_release: models-latest          # where uploads go
exclude: [referenced_models/**]
models:
  complete_system/complete_controller.slx: {}   # per-model overrides go here
```

| Key | Meaning |
|---|---|
| `version` | Always `1`. |
| `defaults` | Settings applied to every model in this models root. |
| `models` | Per-model settings, keyed by path relative to the models root (a unique file name also works). They override `defaults`. |
| `exclude` | Glob patterns, relative to the models root, for models hidden from default selection. A model entry can set `exclude: false` to show itself anyway. |
| `project` | MATLAB project file to open, relative to the manifest. |
| `setup` | Name of a MATLAB function (on the project or model path) run before generation. It must be a function name, not an expression. |
| `messages` | Custom ROS interface packages the model uses, as directories containing `package.xml`, relative to the manifest. Include every custom dependency. |
| `configurations` | Optional `x86_64`/`arm64` mapping to ConfigSet factory function names. |
| `github.repository` | `owner/repo` for downloads and publishing. Defaults to the `origin` remote. |
| `github.release` | Release tag for downloads. `latest` means the publish channel below. |
| `github.publish_release` | Release tag for uploads. Defaults to `models-latest`. |

All paths are relative to the manifest file. `--manifest PATH` uses a different manifest for one run.

## 4. Check the manager sees it

```bash
ros2 run riptide_simulink model_manager.py list
```

Your model should appear as a workspace-relative path, e.g. `riptide_control/riptide_controllers/models/complete_system/complete_controller.slx`. Hidden models are listed with `[excluded by default]`; in the TUI they are under the collapsed **Referenced/excluded models** section.

If it's missing, check that the file sits under a folder named exactly `models`, isn't inside one of the skipped folders, and isn't reached through a symlink.

## 5. Build it

In the TUI: **Build models**, select your model, **Build**. Or:

```bash
ros2 run riptide_simulink model_manager.py build depth_controller --source local
```

The first build generates code for both architectures and can take several minutes; later builds reuse unchanged work. A successful build:

1. stages the host package in `~/osu-uwrt/development/software/src/simulink_models/<model>` and compiles it,
2. stages the ARM64 sources in `~/osu-uwrt/release/src/simulink_models/<model>` for the robot,
3. keeps the archives in `~/osu-uwrt/simulink_model_archives`.

Then run it:

```bash
source ~/osu-uwrt/development/software/install/setup.bash
ros2 run depth_controller depth_controller
```

If generation fails, the results page shows the MATLAB error; the **Output** view (Tab) has the full MATLAB log. Common causes are missing model data (fix the project or `setup`), a custom message package missing from `messages`, or an ambiguous project.

## 6. Share it

After a good build, choose **Update GitHub model release** (or pass `--publish`). Teammates can then `download` your model without MATLAB. The repository needs the organization's GitHub App installed once; see [Publishing](PUBLISHING.md).

## 7. Supervise it (optional)

To configure or reload the model node from an overseer, use the [C++ model wrapper](CPP_WRAPPER.md).
