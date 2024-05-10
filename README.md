# [`std::execution`](http://wg21.link/p2300) Proposal Implementation

This repository is intended to become an implementation of the
[`std::execution`](http://wg21.link/p2300) proposal for the [Beman
Project](https://github.com/beman-project). There are likely
additional proposals affecting some of the components from this
proposal and are closely related. These may be integrated into this
repository.

## Build

| Library | Linux | MacOS | Windows |
| ------- | ----- | ----- | ------- |
| build | ![Linux build status](https://github.com/dietmarkuehl/beman-execution/actions/workflows/linux.yml/badge.svg) | ![MacOS build status](https://github.com/dietmarkuehl/beman-execution/actions/workflows/macos.yml/badge.svg) | ![Window build status](https://github.com/dietmarkuehl/beman-execution/actions/workflows/windows.yml/badge.svg) |

You can just use `make` in the current directory. It will invoke `cmake`,
build the code, and run tests. To have more control over the build-process
you can use `cmake` with the `CMakeLists.txt` in the root directory.

You can just use `make` in the current directory. It will invoke `cmake`,
build the code, and run tests. To have more control over the build-process
you can use `cmake` with the `CMakeLists.txt` in the root directory.

## To Do

Help is very welcome: Please send pull requests!
The [To Do Page](docs/TODO.md) lists what sections and their current status.
