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
- [exec.fwd.env] states "Mandates: ... is a core constant expression if
    `q` is a core constant expression": I don't know how to possibly
    implement that.
- [exec.snd.expos] uses query-or-default; it seems only query-with-default
    is defined.
- [exec.snd.concepts] "The type tag_of_t<Sndr> is +defined+ as follows:"
- [exec.sched] uses `auto(get_completion_scheduler<set_value_t>(...))`
    which is OK for clang but doesn't seem to compile for g++ os MSVC.