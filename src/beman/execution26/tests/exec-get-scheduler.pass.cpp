// src/beman/execution26/tests/exec-get-scheduler.pass.cpp            -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/get_scheduler.hpp>
#include <beman/execution26/detail/forwarding_query.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct scheduler
    {
        int value{};
        auto operator== (scheduler const&) const -> bool = default;
    };

    struct env
    {
        int value{};
        auto query(test_std::get_scheduler_t const&) const noexcept
        {
            return scheduler{this->value};
        }
    };
}

auto main() -> int
{
    static_assert(std::same_as<test_std::get_scheduler_t const,
                               decltype(test_std::get_scheduler)>);
    static_assert(test_std::forwarding_query(test_std::get_scheduler));
    env e{17};
    auto sched{test_std::get_scheduler(e)};
    static_assert(::std::same_as<scheduler, decltype(sched)>);
    assert(sched == scheduler{17});
}