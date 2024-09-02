// src/beman/execution26/tests/exec-run-loop-general.pass.cpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/run_loop.hpp>
#include <beman/execution26/detail/scheduler.hpp>
#include <test/execution.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace
{
    auto use(auto&&...) {}
}

auto main() -> int
{
    static_assert(noexcept(test_std::run_loop()));
    static_assert(not std::move_constructible<test_std::run_loop>);
    test_std::run_loop rl;
    use(rl);

    static_assert(requires{ { rl.get_scheduler() } -> test_std::scheduler; });
    static_assert(requires{ rl.run(); });
    static_assert(requires{ rl.finish(); });
}