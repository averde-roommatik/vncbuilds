# UltraVNC compatibility patches

Same convention as [`patches-tigervnc/`](../patches-tigervnc): a `.patch` here works
around a mismatch between an UltraVNC source tree (usually the frozen `stable` branch)
and whatever toolchain/vcpkg package versions the CI runner currently provides. The
build applies every patch here with a dry-run check first and silently skips any that no
longer apply, so this directory can be added to over time without breaking builds once
upstream or vcpkg catches up.

`ViewerBounds.h` and `ViewerJpegSource.h` are not patches: upstream commit `6c44cfc`
includes them but never committed them, so the workflow copies these reconstructions into
`vncviewer/` when upstream still lacks them. Delete each (and the workflow step) once
upstream ships the real one.

Otherwise added reactively as real CI failures surface.
