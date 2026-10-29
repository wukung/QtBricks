# Contributing to QtBricks

## Build & test

```bash
cmake -B build
cmake --build build
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure
```

`QT_QPA_PLATFORM=offscreen` lets the widget tests run without a display
(the CI workflow sets it too).

## Workflow

- Branch from `main` with a `fix/`-, `test/`-, `ci/`- or `docs/`-prefixed name.
- Keep each pull request focused; run the build and full test suite before
  opening it.
- Unit tests live in `UT/` (Qt Test, one `tst_*` binary per area) and are
  registered in `UT/CMakeLists.txt`.
