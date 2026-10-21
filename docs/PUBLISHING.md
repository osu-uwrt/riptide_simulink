# Publish locally generated models

Local MATLAB R2023a generation is the primary workflow. After a successful build,
the manager offers to update the repository’s rolling model release.
Uploading uses GitHub’s HTTPS API directly; neither `gh` nor a personal access token
is required. Local MATLAB uses the computer’s existing license.

## Ordinary use

```bash
ros2 run riptide_simulink model_manager.py
```

Choose **Build models**, select the models, and choose **Build**. Optional settings
appear beneath headings on a single **Advanced options** page. Choose **Update GitHub model release** when
the build succeeds. The preview identifies the repository, release, model,
architecture, and captured source revision. The first upload opens GitHub’s device
sign-in page and displays a short code. Enter the code and authorize the app.
If a browser cannot open, use the displayed URL on another computer. Escape or
`q` cancels an operation and returns to its results; Back returns to earlier pages.
Browser sign-in starts only after selecting publication and passing local artifact
validation. Listing models, ordinary builds, and public downloads do not sign in.

A shared organization App is configured once by an administrator. Each user
usually authorizes it once; expired refresh tokens, revoked access, or unavailable
credential storage may require another browser sign-in. GitHub App permissions
are the intersection of the user’s access and the app’s installation permissions.
An SSH key authenticates Git operations and cannot authorize release API uploads.

Credentials are saved only in Ubuntu Secret Service via `python3-keyring`
(`sudo apt install python3-keyring`; rosdep has no rule for it).
The manager refreshes expiring credentials automatically. If Secret Service is
unavailable or locked, credentials remain in memory for the current manager
session; no plaintext credential file is created. Signing out removes this
machine’s stored credentials. To revoke the App’s access as well, visit
<https://github.com/settings/apps/authorizations>.

Explicit commands run once without prompts, including when called from a terminal:

```bash
# Use saved authentication; do not open a browser or prompt.
ros2 run riptide_simulink model_manager.py build complete_controller --source local --publish
ros2 run riptide_simulink model_manager.py build complete_controller --no-publish
ros2 run riptide_simulink model_manager.py publish --index /path/to/publish-BUILD.json
ros2 run riptide_simulink model_manager.py publish --index /path/to/publish-BUILD.json --publish-release models-test
ros2 run riptide_simulink model_manager.py account status
ros2 run riptide_simulink model_manager.py account check osu-uwrt/riptide_control
ros2 run riptide_simulink model_manager.py account logout
```

`account check` (or **Advanced options → GitHub account → Sign in and check repository
access** in the TUI) signs in if needed and reports the login, the user's write access,
and whether the App is installed on the repository. It writes nothing. During any
sign-in the TUI pins the device code in the build panel and turns the status bar yellow.

If authentication is needed for a command, open **Advanced options** and choose
**Publish a cached local build** beneath the **Cached builds** heading in the TUI
to sign in and retry. `GITHUB_TOKEN` or `GH_TOKEN` remains supported for
unattended use, and takes precedence over saved App credentials. No token is
accepted as a command-line argument. Environment credentials remain active after
sign-out until unset. `--interactive` explicitly opens the manager with
optional workspace/path overrides; `--non-interactive` explicitly selects CLI mode.

## First live test

1. Register and install the App (below). Its Client ID is already built in.
2. Run the access check against the model repository and fix anything it reports.
3. Publish an existing clean cached build to a throwaway tag instead of
   `models-latest`: in the TUI, set **Publication release tag** to `models-test`
   under **GitHub and publishing**, then use **Publish a cached local build**. From a
   terminal: `model_manager.py publish --index ~/osu-uwrt/simulink_model_archives/publish-ID.json --publish-release models-test`.
4. Afterwards, delete the `models-test` release and its tag on GitHub.

Each publication to the model channel (`models-latest`, or the manifest's
`publish_release`) marks it as the repository's latest release. A one-off tag passed
with `--publish-release` does not, so it stays out of the way of ordinary releases.
Downloads with `release: latest` look up the channel tag first and only fall back to
GitHub's latest release when the channel doesn't exist yet. A release someone makes
by hand therefore can't break model downloads, even if it becomes "Latest". If a first publication to a new tag
fails, the draft it created is reused by the next attempt.

A fine-grained personal token with **Contents: read and write** on the repository,
exported as `GH_TOKEN`, exercises the upload path without the App (organizations
may require approval for such tokens). It does not test browser sign-in.

## One-time organization administrator setup

The team's App is **registered once by an organization owner**, then installed on
each model repository. Users never create their own App and never handle any App
secret. The current App's Client ID, `Iv23liVPcmYLaOKXjqtu`, is built into
`DEFAULT_CLIENT_ID` in `riptide_simulink/auth.py`.

### What the Client ID is, and what must stay private

The **Client ID** is the App's public name in the OAuth sign-in flow. During sign-in
the manager sends it to GitHub to say "this request is for the Riptide publisher App".
GitHub then shows the user a code and the App's name, and the *user* approves it
in their own browser session. The resulting token is tied to that user and is
limited by their own repository permissions. Knowing the Client ID lets someone
start a sign-in that a user would still have to approve, so it is safe to commit.
That is why device flow is designed for tools that cannot keep secrets.

The App settings page also offers two real secrets. **Neither is needed by this
manager, so don't create them, and delete them if they already exist:**

- **Client secret**: used by web-server OAuth flows. Device flow and token
  refresh don't need it.
- **Private key** (`.pem`): lets a server authenticate *as the App itself* across
  every installation, without any user. Anyone holding it can act with the App's
  full installation permissions. Delete it under **Private keys** and remove the
  downloaded file.

### Registering the App

1. Open **github.com/organizations/osu-uwrt/settings/apps** (organization
   **Settings → Developer settings → GitHub Apps**) and choose **New GitHub App**.
   Register it under the organization, not under **your personal account's**
   Developer settings: a personal App cannot be installed on organization
   repositories unless made public.
2. Fill in:
   - **Name**: for example *Riptide Simulink Publisher*. It is shown to users when they sign in.
   - **Homepage URL**: the tooling repository's URL.
   - **Callback URL**: leave empty. Leave **Request user authorization (OAuth) during installation** off.
   - **Expire user authorization tokens**: on.
   - **Enable Device Flow**: on. Without it, sign-in fails with `device_flow_disabled`.
   - **Webhook → Active**: off.
   - **Repository permissions → Contents**: *Read and write*. Metadata becomes
     read-only automatically. Leave everything else as *No access*.
   - **Where can this GitHub App be installed?**: *Only on this account*.
3. Create the App, then copy the **Client ID** (starts with `Iv`; it's not the
   numeric App ID) into `DEFAULT_CLIENT_ID`, or have users set
   `RIPTIDE_SIMULINK_GITHUB_CLIENT_ID`. Rebuild the package.

### Installing it on repositories

Registering an App does not install it anywhere.

1. On the App's settings page, choose **Install App** in the left sidebar, then
   **Install** next to *osu-uwrt*.
2. Choose **Only select repositories**, pick each model repository (for example
   `riptide_control`), and confirm.
3. Check that it appears under the organization's **Settings → Third-party Access
   → GitHub Apps** (*Installed GitHub Apps*). To add repositories later, choose
   **Configure** there.

Only organization owners can install directly. For anyone else, **Install**
sends a request that an owner must approve.

### Checking the setup

Run **Advanced options → GitHub account → Sign in and check repository access**, or
`model_manager.py account check osu-uwrt/riptide_control`. It must report write access
and "App installed on repository: yes".

Installation is not a permission grant: a signed-in user still needs write
access to the repository, and GitHub limits the token to the intersection of the
user's access and the App's permissions. Repeated sign-in cannot fix missing
access or a missing installation.

Rolling releases must be mutable. If the repository enables immutable releases,
choose a publication arrangement that permits updating the rolling channel. The
manager refuses an immutable release instead of deleting or recreating it.

References: [device authorization](https://docs.github.com/en/apps/creating-github-apps/authenticating-with-a-github-app/generating-a-user-access-token-for-a-github-app#using-the-device-flow-to-generate-a-user-access-token),
[token refresh](https://docs.github.com/en/apps/creating-github-apps/authenticating-with-a-github-app/refreshing-user-access-tokens),
[installing your own App](https://docs.github.com/en/apps/using-github-apps/installing-your-own-github-app).

## Release selection and provenance

The default publication tag is `models-latest`. Override it with
`--publish-release TAG` or the independent manifest setting:

```yaml
defaults:
  github:
    repository: osu-uwrt/example_controller
    release: latest                 # Download selection
    publish_release: models-latest  # Upload destination
```

An existing rolling tag identifies the channel, not the source of every build.
The release text has a section per model build, newest first, showing:
- whether it was a clean, tagged or dev build. A dev build had uncommitted or
  untracked changes, and untracked files count as in Titan firmware's build tags.
- the commit (linked) and branch
- the build time and `user@host`
- its architectures, MATLAB/ROS versions and packages
Each archive’s index record contains its exact commit, branch, exact tag (if any),
builder `user@host`, modification status, generation ID/time, selected architectures,
tool versions, packages, and SHA-256.
A new rolling tag is anchored to the remote default branch. Publishing never
pushes local commits or advances the rolling tag. `--repository` explicitly
overrides the detected upload destination when needed.

Publication requires captured clean, committed sources by default. Committing
changes after generation does not make that earlier build clean. The TUI can
explicitly confirm publication of modified sources; scripts must provide
`--allow-dirty`. Modified builds are marked in the preview, index, and release
body. Downloaded artifacts are rejected as local publication inputs.

The local `publish-UUID.json` file snapshots the exact selected build and is used
by **Publish a cached local build**, which lists saved builds newest first by a
Titan-style tag: models, `branch/rev-dirty (tag)`, build time (UTC), builder and
architectures. Builds saved before branch/builder capture show only the revision. The newest 5
saved builds of each model are kept. Generated metadata records the complete
architecture selection, preventing accidental publication of half a dual-
architecture build. A deliberately generated single-architecture build is valid.
Updating selected models preserves all unrelated release-index entries. Duplicate
model entries and conflicting package ownership are rejected. Separate repository
updates complete independently; a failure in one repository does not undo another
repository’s successful publication.

## Failure recovery and concurrent users

Archives are validated before upload. New assets receive unique names and upload
before index promotion. After a successful publish, the release keeps the current and
previous build of the pushed models and deletes their older archives and index backups;
other models' archives and hand-attached assets are never touched. Cleanup is best
effort: a failure is reported and retried on the next publish. The
publisher checks GitHub’s reported upload size and checksum when supplied.
The index is switched last, followed by publishing the release (marked latest only
for `models-latest`). Recoverable
promotion errors restore the previous index. There can be a short interval during
index renaming where a downloader must retry; GitHub does not provide an atomic
multi-asset transaction. Publication failure never invokes generation fallback.

An exclusive `riptide-publish-lock.json` release asset serializes cooperating
publishers. A busy message names the owner and start time. After an abrupt
crash, an administrator must verify that no publisher is still running, inspect
index/backups if recovery was interrupted, then delete **only this lock asset**
through GitHub’s release UI. Locks are not automatically expired or stolen.

Upload failures preserve local archives and print the exact retry command.
`--no-archive` is overridden for a selected upload: a durable retry copy is retained
in the configured archive directory. Cancellation also preserves that retry copy.
A publication requested by a CLI command returns failure if it cannot finish;
the message separately reports the successful local build. The TUI displays the
failure and remains open. Retrying a multi-repository bundle may upload fresh
versioned copies to repositories that already succeeded; unrelated models remain.
