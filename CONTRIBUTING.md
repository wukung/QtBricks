# Contributing to QtBricks

## Build & test

```bash
cmake -B build
cmake --build build
cd build
QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

Plain `ctest` (no `--test-dir`, which needs CMake 3.20+) matches the
CMake >= 3.16 minimum. `QT_QPA_PLATFORM=offscreen` lets the widget tests
run without a display.

## Workflow

- Branch from `main` with a `fix/`-, `test/`-, `ci/`- or `docs/`-prefixed name.
- Keep each pull request focused; run the build and full test suite before
  opening it.
- Unit tests live in `UT/` (Qt Test, one `tst_*` binary per area) and are
  registered in `UT/CMakeLists.txt`.
