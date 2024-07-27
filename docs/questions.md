# Questions

This page lists some questions which came up while implementing
[P2300](http://wg21.link/p2300). The current implementation made a
choice to avoid getting blocked on the respective question. Different
implementations may make different choices and the differences are
likely observable.

- [exec.set.value], [exec.set.error], and [exec.set.stopped] don't
    specify whether the operator should be `noexcept`. Choice: make
    the respective operation `noexcept`.
- [exec.general] use "Mandates: err != exception_ptr() is true."
    However, the condition can only be determined at run-time =>
    The "Mandates" should be "Precondition".
