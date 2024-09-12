# beman.execution26: Building Block For Asynchronous Programs

`beman.execution26` provides the basic vocabulary for asynchronous
programming as well as important algorithms implemented in terms
of this vocabulary. The key entities of the vocabulary are:

- `scheduler` used to control where operations are executed.
    - The key operation is `schedule(scheduler) -> sender`.
- `sender` used to represent work.
    - The key operation is `connect(sender, receiver) -> operation-state`.
- `receiver` used to receive completion notifications.
    - There are multiple operations on receivers:
        - `set_value(receiver, args...)` for successful completions.
        - `set_error(receiver, error)` to report errors.
        - `set_stopped(receiver)` to report cancellation.
- `operation-state` to represent a ready to run work graph .
    - The key operation is `start(state)`.

Using these operations some fundamental algorithms are implemented,
e.g.:

- `just(args...)` as the starting point of a work graph without a
    specified scheduler`.
- `let_value(sender, fun)` to produce a sender based on `sender`'s
    results.
- `on(scheduler, sender)` to execute `sender` on `scheduler`.
- `transfer(sender, scheduler)` to complete with with `sender`'s
    results on `scheduler`.
- `when_all(sender ...)` to complete when all `sender`s have
    completed.
- `bulk(...)` to executed execute work, potentially concurrently.

**Implements:** [`std::execution` (P2300)](http://wg21.link/p2300).

## Build

| Library | Linux | MacOS | Windows |
| ------- | ----- | ----- | ------- |
| build | ![Linux build status](https://github.com/beman-project/execution26/actions/workflows/linux.yml/badge.svg) | ![MacOS build status](https://github.com/beman-project/execution26/actions/workflows/macos.yml/badge.svg) | ![Window build status](https://github.com/beman-project/execution26/actions/workflows/windows.yml/badge.svg) |

You can just use `make` in the current directory. It will invoke `cmake`,
build the code, and run tests. To have more control over the build-process
you can use `cmake` with the `CMakeLists.txt` in the root directory.

## Examples

- `<stop_token>` example: [Compiler Explorer](https://godbolt.org/z/4r4x9q1r7)

## To Do

Help is very welcome: Please send pull requests!
The [To Do Page](docs/TODO.md) lists what sections and their current status.
