# TigerVNC Viewer — Unofficial Windows Builds

Automated, unofficial Windows builds of the [TigerVNC](https://github.com/TigerVNC/tigervnc) viewer (`vncviewer`), built directly from upstream source via GitHub Actions and published here as public [Releases](../../releases) — no login, no token, no SourceForge required.

> **This project is not affiliated with, endorsed by, or officially maintained by the TigerVNC project.** It simply automates compiling their public source code for Windows and republishing the result.

## Downloads

Two release channels are published, both as plain `.exe` assets:

| Channel | Tag pattern | Built from | Stability |
|---|---|---|---|
| **Stable** | `vX.Y.Z` | The corresponding official TigerVNC tag | Matches an official TigerVNC release exactly |
| **Rolling** | `vX.Y.Z-rc.g<sha>` | The current `master` branch (`<sha>` = short commit hash) | Latest upstream code, not an official release |

Always-current direct links (no need to browse releases manually):

- Latest **stable**: `https://github.com/averde-roommatik/vncbuilds/releases/latest/download/vncviewer.exe`
- Latest **rolling**: see the [Releases page](../../releases) for the most recent `-rc.g<sha>` tag

## How it works

A scheduled GitHub Actions workflow (`.github/workflows/build.yml`) runs every 12 hours, and can also be triggered manually from the Actions tab (`workflow_dispatch`, with an optional "force" input to rebuild even if the release already exists):

1. Checks the latest tag and the current `master` commit of [`TigerVNC/tigervnc`](https://github.com/TigerVNC/tigervnc) via `git ls-remote` — no cloning or forking of the upstream repo.
2. Skips work entirely if a Release already exists here for that tag/commit (idempotent — safe to run on a schedule).
3. Otherwise, checks out upstream at that exact ref and builds it on `windows-latest` using the same MSYS2 + mingw-w64 toolchain and dependency set TigerVNC uses in its own CI, with `CMAKE_BUILD_TYPE=Release`:
   - `mingw-w64-x86_64-toolchain`, `-cmake`, `-fltk1.3`, `-libjpeg-turbo`, `-gnutls`, `-pixman`, `-nettle`, `-gmp`
   - `cmake -DBUILD_VIEWER=ON -DENABLE_NLS=ON -DENABLE_H264=ON -DENABLE_AUDIO=ON -DENABLE_GNUTLS=ON -DENABLE_NETTLE=ON`
4. Publishes the resulting `vncviewer` installer as a GitHub Release, tagged as described above.

No source code is copied or vendored into this repository — every build compiles fresh from the upstream commit it's tagged against, so you can always verify exactly what went into a given release.

## Verifying a build

Every release tag maps to a specific upstream commit. To inspect or rebuild the exact source used for release `vX.Y.Z-rc.g<sha>`:

```bash
git clone https://github.com/TigerVNC/tigervnc.git
git -C tigervnc checkout <sha>
```

For stable releases, check out the matching official tag instead.

## License

- **This repository's own files** (workflow definitions, scripts, this README) are licensed under the [MIT License](LICENSE).
- **The distributed binaries** are TigerVNC, unmodified apart from the compilation step, and remain licensed under the **GNU General Public License v2** by their original authors. See [TigerVNC's `LICENCE.TXT`](https://github.com/TigerVNC/tigervnc/blob/master/LICENCE.TXT) for the full text, and the "Verifying a build" section above to obtain the exact matching source.

## No warranty

These are automated builds provided as-is, with no guarantee of stability, security review, or fitness for any purpose. For production or security-sensitive use, consider building from source yourself or using an official distribution channel.