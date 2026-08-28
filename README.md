# VNC Viewer — Unofficial Windows Builds

Automated, unofficial Windows builds of two open-source VNC viewers, built directly from
upstream source via GitHub Actions and published here as public [Releases](../../releases)
— no login, no token, no SourceForge required.

> **This project is not affiliated with, endorsed by, or officially maintained by the
> TigerVNC or UltraVNC projects.** It simply automates compiling their public source code
> for Windows and republishing the result.

## Downloads

Two products, both published as plain `.exe`/`.zip` assets:

| Product | Channel | Tag pattern | Built from | Stability |
|---|---|---|---|---|
| **TigerVNC** (`tiger-vncviewer`) | Stable | `tiger-vX.Y.Z` | The corresponding official [TigerVNC](https://github.com/TigerVNC/tigervnc) tag | Matches an official TigerVNC release exactly |
| **TigerVNC** (`tiger-vncviewer`) | Rolling | `tiger-vX.Y.Z-rc.g<sha>` | TigerVNC's current `master` branch (`<sha>` = short commit hash) | Latest upstream code, not an official release |
| **UltraVNC** (`ultra-vncviewer`) | — | `ultra-vX.Y.Z.W` | UltraVNC's current [`main`](https://github.com/ultravnc/UltraVNC) branch | See "UltraVNC versioning" below — there's no reliable stable/rolling split to make |

Only the **last 10** TigerVNC rolling releases are kept — older ones are pruned
automatically. Stable releases are kept indefinitely.

Always-current direct links (no need to browse releases manually):

- Latest TigerVNC **stable**: `https://github.com/averde-roommatik/vncbuilds/releases/latest/download/tiger-vncviewer.exe`
- Latest UltraVNC build: see the [Releases page](../../releases) for the most recent `ultra-v*` tag
- TigerVNC rolling builds: see the [Releases page](../../releases) for the most recent `tiger-v*-rc.g<sha>` tag

Both viewer-only builds ship as an installer (`*.exe`) and a portable, no-install `*.zip`.
**UltraVNC's build packages the viewer only** — no server, no repeater, no drivers,
unlike UltraVNC's own official installer which bundles the full suite.

## How it works

A scheduled GitHub Actions workflow (`.github/workflows/build.yml`) runs every 12 hours,
and can also be triggered manually from the Actions tab (`workflow_dispatch`, with
`product`, `target`, and `force` inputs to scope a manual run).

1. Checks upstream for the current stable ref and rolling-branch commit — no cloning or
   forking of either upstream repo ahead of time.
2. Skips work entirely if a Release already exists here for that tag/commit (idempotent —
   safe to run on a schedule).
3. Otherwise, checks out upstream at that exact ref and builds it on `windows-latest`.
4. Publishes the resulting installer + portable zip as a GitHub Release, tagged as
   described above.

No TigerVNC or UltraVNC source code is copied or vendored into this repository — every
build compiles fresh from the upstream commit it's tagged against, so you can always
verify exactly what went into a given release.

### TigerVNC build

Built with the same MSYS2 + mingw-w64 toolchain and dependency set TigerVNC uses in its
own CI, with `CMAKE_BUILD_TYPE=Release`:
- `mingw-w64-x86_64-toolchain`, `-cmake`, `-fltk1.3`, `-libjpeg-turbo`, `-gnutls`,
  `-pixman`, `-nettle`, `-gmp`
- `cmake -DBUILD_VIEWER=ON -DENABLE_NLS=ON -DENABLE_H264=ON -DENABLE_AUDIO=ON -DENABLE_GNUTLS=ON -DENABLE_NETTLE=ON`

The installer is TigerVNC's own `release/tigervnc.iss`, patched at build time to inject
the resolved list of runtime DLLs.

#### Compatibility patches

The `stable` channel builds older, frozen TigerVNC tags against whatever toolchain and
library versions MSYS2 currently provides — which can drift out of sync over time (e.g. a
newer `nettle` major version breaking an old tag's code that predates a compatibility fix
already present upstream). When that happens, a small, targeted patch is kept in
[`patches-tigervnc/`](patches-tigervnc) and applied automatically, but only if it's still
needed — the build checks first, and silently skips any patch that no longer applies
(typically because a newer stable tag already includes the equivalent fix upstream). This
keeps `stable` buildable across time without vendoring any TigerVNC source.

### UltraVNC build

Built with Visual Studio (MSBuild) against UltraVNC's own `vncviewer.sln`, using its
`vcpkg.json` manifest (`zlib`, `zstd`, `libjpeg-turbo`, `liblzma`, `libsodium`,
`minizip-ng`) in static-linking mode — no runtime DLLs beyond what's already on a stock
Windows install.

Because the installer we ship packages **only the viewer**, we don't reuse UltraVNC's own
`UltraVNC_installer_x64.iss`/`_x86.iss` (those build the full server+viewer+driver suite).
Instead this repo vendors a minimal Inno Setup script,
[`installer/ultravnc-viewer.iss`](installer/ultravnc-viewer.iss), that installs nothing
but the unmodified `vncviewer.exe` this build produces.

Same compatibility-patch convention as TigerVNC applies here too, in
[`patches-ultravnc/`](patches-ultravnc) — empty until a real build failure calls for one.

#### UltraVNC versioning

UltraVNC publishes no git tags and no GitHub Releases, and there's no branch that
reliably maps to an actual shipped version either: uvnc.com's real latest release is
`1.8.2.4`, which is *older* than what `main` itself currently reports, and the only
other branch in the repo (`1.9.0.0`) is an in-progress future-version candidate that
was never released — not something safe to treat as "the stable version." `main` is
the only ref this pipeline builds. Its version string comes from reading
[`common/versionServerViewer.inc`](https://github.com/ultravnc/UltraVNC/blob/main/common/versionServerViewer.inc)
(`APS_MAJORNUMBER`/`MINORNUMBER`/`BUILDNUMBER`/`MODIFICATIONNUMBER`) at whatever commit
was built, so the tag always reflects exactly what that build reports — it is not
claimed to be an official UltraVNC release.

## Verifying a build

Every release tag maps to a specific upstream commit. To inspect or rebuild the exact
source used for a given rolling release:

```bash
git clone https://github.com/TigerVNC/tigervnc.git   # or https://github.com/ultravnc/UltraVNC.git
git -C tigervnc checkout <sha>                        # the <sha> from the release tag
```

For stable releases, check out the matching branch/tag named in the release notes instead.

## License

- **This repository's own files** (workflow definitions, scripts, this README) are
  licensed under the [MIT License](LICENSE).
- **TigerVNC binaries** are unmodified apart from the compilation step, and remain
  licensed under the **GNU General Public License v2** by their original authors. See
  [TigerVNC's `LICENCE.TXT`](https://github.com/TigerVNC/tigervnc/blob/master/LICENCE.TXT)
  for the full text.
- **UltraVNC binaries** are unmodified apart from the compilation step, and remain
  licensed under **GPL-3.0-or-later** by the UltraVNC Team. See
  [UltraVNC's `LICENSE`](https://github.com/ultravnc/UltraVNC/blob/main/LICENSE) for the
  full text.
- See "Verifying a build" above to obtain the exact matching source for either project.

## No warranty

These are automated builds provided as-is, with no guarantee of stability, security
review, or fitness for any purpose. For production or security-sensitive use, consider
building from source yourself or using an official distribution channel.
