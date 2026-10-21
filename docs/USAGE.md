# Using the manager

Reference for the TUI and the command line. To set up a new model first, see [Adding a model](ADDING_MODELS.md).

- [The TUI](#the-tui)
- [Command line](#command-line)
- [Where models come from](#where-models-come-from)
- [MATLAB](#matlab)
- [Outputs](#outputs)
- [Caching](#caching)
- [Cleanup](#cleanup)
- [Downloading published builds](#downloading-published-builds)
- [Legacy commands](#legacy-commands)

## The TUI

```bash
ros2 run riptide_simulink model_manager.py
```

With no command, the manager stays open until you quit.

### Navigation

| Key | Action |
|---|---|
| Arrows, Page Up/Down | Move |
| Space | Select (multi-select lists) or toggle a setting |
| Enter | Accept or edit |
| `b`, Escape, `q` | Back one page (`q` at the home menu quits) |
| Tab, ←/→ | Switch views on a results page |

Listings and results are scrollable pages, and errors and cancellations return you to the manager. Terminals without cursor support get numbered prompts instead. Escape responds after 25 ms rather than ncurses' one second; an `ESCDELAY` you set yourself is respected.

### Building

The normal flow is **Build models → select models → Build**. By default it uses local MATLAB (falling back to the published release if generation fails), builds both architectures, compiles the host package, stages the release sources, and uses every CPU core.

**Advanced options** opens one scrollable page of settings grouped under Build and MATLAB, GitHub and publishing, Output and deployment, and Workspace. Only settings relevant to the current action are shown. Enter edits a setting and returns you to the same row; booleans toggle in place. The home screen's own **Advanced options** page lists less common actions the same way: cached builds, maintenance, MATLAB, GitHub account and session defaults.

Referenced and excluded models are collapsed under **Referenced/excluded models** in listings and selection pages. Highlight the section and press Enter, Space or → to expand it, ← to collapse it.

### Progress

While a build runs, a panel below the output tracks each stage (sources, development staging, development compilation, release staging, publication) as waiting, running, done, skipped, failed or cancelled:

- The header is a status bar: **blue** while running, **yellow** when the build needs you, **green** when complete, **red** on failure. It shows the total elapsed time.
- Each stage shows its duration, and the active MATLAB step (e.g. `complete_controller.slx arm64 Generating (2/2)`) shows how long it has been running, so a quiet step is visibly not stalled.
- MATLAB and colcon output is dimmed behind a `│` gutter so the manager's own messages stand out.
- Questions asked mid-build, such as the upload prompt or the high-memory warning, appear in a yellow **ACTION NEEDED** box.

### Results

A finished operation opens with a banner such as **✔ BUILD COMPLETE** or **✘ BUILD FAILED**, keeps the final status bar pinned at the bottom, and has three views (long lines wrap):

- **Summary** (default): any error, a timeline of stage durations, MATLAB generation time per model and architecture (cached or generated), and details such as models, source, custom-message counts, compiler jobs, peak RAM, tool-output and warning counts, and where things were installed or published. After a download it shows exactly which build you got. After a failure it lists the last lines from the stage that stopped.
- **Output**: after a failure or cancellation, only the stopped stage's output; after success, the manager's own messages.
- **Full log**: everything.

## Command line

Explicit commands run once without prompts:

```bash
ros2 run riptide_simulink model_manager.py list
ros2 run riptide_simulink model_manager.py complete_controller          # same as build
ros2 run riptide_simulink model_manager.py build complete_controller --source local
ros2 run riptide_simulink model_manager.py build complete_controller --source local --publish
ros2 run riptide_simulink model_manager.py build complete_controller --source release --release latest
ros2 run riptide_simulink model_manager.py build complete_controller --arch arm64 --no-build
ros2 run riptide_simulink model_manager.py download complete_controller
ros2 run riptide_simulink model_manager.py process-cached complete_controller
ros2 run riptide_simulink model_manager.py publish --index /path/to/publish-BUILD.json
ros2 run riptide_simulink model_manager.py messages complete_controller
ros2 run riptide_simulink model_manager.py open complete_controller
ros2 run riptide_simulink model_manager.py packages
ros2 run riptide_simulink model_manager.py account status
ros2 run riptide_simulink model_manager.py clean --include complete_controller --yes
ros2 run riptide_simulink model_manager.py clean --ignore another_model --dry-run
ros2 run riptide_simulink model_manager.py clean --all --yes
```

| Command | What it does |
|---|---|
| `list` | Models the manager can see |
| `build MODEL…` (or just `MODEL…`) | Get sources, stage them, compile the host package |
| `download MODEL…` | Same as `build --source release` |
| `process-cached MODEL…` | Re-stage the last cached archives without MATLAB or GitHub |
| `publish --index FILE` | Upload a saved local build; see [Publishing](PUBLISHING.md) |
| `messages MODEL…` | Regenerate MATLAB's custom-message support |
| `open MODEL` | Open the model or its project in the MATLAB desktop |
| `packages` | Generated packages the manager owns |
| `account status\|check\|logout` | GitHub sign-in |
| `clean`, `delete` | Remove generated outputs ([Cleanup](#cleanup)) |

Select models by name (`complete_controller`), workspace-relative path, or absolute path. A name shared by two models must be given as a path. Commands never silently select every model: pass names, `--models-select`, `--models-ignore`, or `--all`.

**Build routes.** `--source auto` (the default) tries MATLAB first and downloads the published release if generation, setup or licensing fails. Cancelling and colcon failures never trigger a download. `--source local` and `--source release` force one route.

**Mode flags.** `--non-interactive` forces command-line mode. `--interactive` opens the TUI with workspace or path overrides applied.

`--help` lists every flag.

## Where models come from

The manager searches `~/osu-uwrt/development/software/src` (override with `--workspace`) for `.slx` files inside folders named `models`. [Adding a model](ADDING_MODELS.md) has the full rules and the `simulink.yaml` reference.

Without a manifest, the nearest `.prj` above the model is opened, and the repository's `origin` remote supplies the GitHub repository. More than one candidate project is an error until `project` is set. A model that needs data or messages that aren't configured reports its MATLAB error and, with `--source auto`, falls back to the published release.

## MATLAB

**Requirements.** MATLAB R2023a with Simulink, ROS Toolbox, Simulink Coder and Embedded Coder. The release is checked at runtime. `--matlab` picks a different launcher, and `--matlab-release` requests another release, but only R2023a is validated. Individual models may need more toolboxes for their own blocks.

**Isolated startup.** Batch MATLAB skips your personal `startup.m`, using a temporary startup folder owned by the manager. Projects and manifest `setup` functions still run. To include your startup, enable **Run personal MATLAB startup** under Build and MATLAB, or pass `--user-startup`. **Open** in the MATLAB desktop always uses your normal startup.

**Compiler jobs.** `--jobs` defaults to every available CPU, for both MATLAB's custom-message build and colcon. Change it with `--jobs N` or **Maximum compiler jobs**. Models, architectures and packages are still processed one at a time.

**Memory check.** Before generation and compilation, the manager checks RAM use (Linux `MemAvailable`). At **80% or higher**, the TUI offers **Continue with N jobs**, **Choose a lower job count** or **Cancel build**. A lower count applies to that build only; the default is never reduced automatically. The warning appears at most once per operation. Commands print the warning and keep their job count, so use `--jobs N` when scripting.

**How generation treats your model.** The model and its configuration files are never saved. For each run the generator copies the model's configuration, selects the ROS 2 hardware board, processor and ERT target, generates, and restores everything. An existing ERT configuration is left intact. When converting from another target, R2023a's `switchTarget` resets unrelated settings, so the generator restores and verifies them: tunability, solver and timing, custom code, dynamic allocation and so on. If a setting can't be preserved, generation stops and names it; supply a configuration factory instead. The ROS packaging overrides select a C++ class and disable MAT-file logging, which is incompatible with it. `--local-config`/`--deploy-config` are command-line equivalents of the `configurations` manifest entries.

**Custom messages.** Packages listed under `messages` are compiled once into a cache keyed by their definitions and the MATLAB version, then re-registered with the R2023a ROS Toolbox for later builds. Setup functions run before message generation.

## Outputs

| Content | Default path | Override |
|---|---|---|
| Host ROS source (compiled) | `~/osu-uwrt/development/software/src/simulink_models` | `--local-dir` |
| ARM64 ROS source (for the robot) | `~/osu-uwrt/release/src/simulink_models` | `--deploy-dir` |
| Archives, local index, saved builds, message cache | `~/osu-uwrt/simulink_model_archives` | `--archives-dir` |
| Incremental Simulink build cache | `~/osu-uwrt/simulink_model_archives/builds` | follows `--archives-dir` |

- Only the development workspace is compiled. `--no-build` stages without compiling, and `--no-process-local`/`--no-process-deploy` skip a destination.
- Nothing is deployed by default. `--deploy --deploy-target orin` runs the colcon-deploy extension from the release workspace.
- Before compiling, a colcon `build/<package>` folder configured from a different source path is removed. The old manager's `controller_models` location left these behind, and CMake would otherwise abort.
- A package still present under the old `controller_models` folder counts as a collision. The manager reports its path and leaves it alone; resolve it deliberately, or validate in a separate workspace.
- Output folders get a `.gitignore` and per-package ownership receipts. Checksums, archive contents, package names and collisions are validated before anything is replaced.
- Replacement rolls back on errors and interruption. A machine crash can leave an ignored `.transaction-*` folder to inspect. Development and release staging are separate transactions.
- Manager operations are serialized per user, so builds and cleanups can't overlap.

## Caching

- **Incremental builds.** Each model and architecture keeps its own Simulink code-generation and cache folders, so later builds reuse unchanged work. When Simulink reports the code is up to date, generation is skipped and the archive from that cache's last successful build is reused. If no trusted archive exists (for example after an interrupted build), the top model is rebuilt. To force a full regeneration, delete the model's folder under `builds/` or use `clean --full-clean`.
- **Saved builds.** The archive directory keeps the newest 5 saved builds of each model and deletes a model's archives once nothing references them. Other models' files are never touched.
- **`--no-archive`.** Uses temporary archives for one build or download. The incremental build and message caches still persist.

## Cleanup

`clean` and `delete` select **generated ROS package names**, not model file names. Preview with `--dry-run`.

- `--include` and `--ignore` filter packages and can't be combined.
- Unfiltered unattended cleanup requires both `--all --yes`.
- `--no-delete-packages` keeps generated sources and archives and cleans only their build/install products.
- `--exclude-local`, `--exclude-deploy` and `--exclude-archive` keep those categories.
- `--full-clean` also clears the shared message and incremental build caches that carry the manager's ownership marker. It can't be combined with package filters.
- Per-package build/install cleanup supports isolated colcon layouts; merged installs are skipped. MATLAB caches left in source trees by the old manager aren't owned by the manager and are never deleted.

## Downloading published builds

`download` (or `--source release`) fetches a published build instead of running MATLAB:

```bash
ros2 run riptide_simulink model_manager.py download complete_controller
ros2 run riptide_simulink model_manager.py download complete_controller --release v1.2
```

- **Which release.** By default it uses the manifest's `github.release`. The default, `latest`, means the publish channel (`models-latest` unless `publish_release` says otherwise); `--release TAG` picks another.
- **What you get.** The archives are checksummed and test-extracted before anything is staged. The output then shows the release, build type (clean, tagged or dev), branch, full commit, build time and builder, architectures and tools, and compares the build with your checkout.
- **Release assets are ROS source packages,** not prebuilt executables. Releases without the shared `simulink-index.json` are rejected rather than guessed from file names.
- **Authentication.** Downloads use `GITHUB_TOKEN`/`GH_TOKEN` if set, otherwise your saved GitHub login, and never prompt. Private repositories therefore work once you've signed in, and a rejected login falls back to anonymous access.

Publishing is covered in [Publishing](PUBLISHING.md).

## Legacy commands

The old `model_manager.py` task names still work:

| Previous command/flag | Now |
|---|---|
| `generate_packages` | `build --source local` (alias kept) |
| `download_packages` | `download` (alias kept) |
| `process_cached` | `process-cached` (alias kept) |
| `refresh_custom_msg_support` | `messages MODEL` (alias kept) |
| `open_project` | `open MODEL` (alias kept) |
| `clean_workspace`, `delete_packages` | `clean`, `delete` (aliases kept) |
| `--models-select/--models-ignore` | Kept; prefer model names as arguments |
| `--configs-select/--configs-ignore` | Kept for `x86_cfg`/`arm_cfg`; prefer `--arch` |
| `--local-config/--deploy-config` | ConfigSet factory overrides |
| `--assume-yes` | `--yes` (alias kept) |
| `--from-release` | `--release` (alias kept) |
| `--build` | Kept; building is now the default, opt out with `--no-build` |

`--from-url`, the old destructive cache replacement, and the unused `--test` flag were dropped. Release assets need the shared index and GitHub repository metadata.
