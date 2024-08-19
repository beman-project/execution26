// src/Beman/Execution26/tests/exec-sched.pass.cpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/detail/scheduler.hpp>
#include <test/execution.hpp>

// ----------------------------------------------------------------------------

namespace
{
    struct bad_env
    {
        auto query(test_std::get_completion_scheduler_t<test_std::set_value_t> const&) const noexcept
        {
            return 0;
        }
    };

    template <typename Scheduler>
    struct env
    {
        auto query(test_std::get_completion_scheduler_t<test_std::set_value_t> const&) const noexcept
        {
            return Scheduler{};
        }
    };

    template <typename Env>
    struct sender
    {
        using sender_concept = test_std::sender_t;
        auto get_env() const noexcept { return Env{}; }
    };

    struct no_scheduler_concept
    {
        auto schedule() -> sender<env<no_scheduler_concept>> { return {}; }
        auto operator== (no_scheduler_concept const&) const -> bool = default;
    };

    struct not_queryable
    {
        using scheduler_concept = test_std::scheduler_t;
        ~not_queryable() = delete;
        auto schedule() -> sender<env<not_queryable>> { return {}; }
        auto operator== (not_queryable const&) const -> bool = default;
    };

    struct no_schedule
    {
        using scheduler_concept = test_std::scheduler_t;
        auto operator== (no_schedule const&) const -> bool = default;
    };

    struct not_equality_comparable
    {
        using scheduler_concept = test_std::scheduler_t;
        auto schedule() -> sender<env<not_equality_comparable>> { return {}; }
    };

    struct not_copy_constructible
    {
        using scheduler_concept = test_std::scheduler_t;
        not_copy_constructible(not_copy_constructible const&) = delete;
        auto schedule() -> sender<env<not_copy_constructible>> { return {}; }
        auto operator== (not_copy_constructible const&) const -> bool = default;
    };

    struct scheduler
    {
        using scheduler_concept = test_std::scheduler_t;
        auto schedule() -> sender<env<scheduler>> { return {}; }
        auto operator== (scheduler const&) const -> bool = default;
    };

    struct bad_completion_scheduler
    {
        using scheduler_concept = test_std::scheduler_t;
        auto schedule() -> sender<env<scheduler>> { return {}; }
        auto operator== (bad_completion_scheduler const&) const -> bool = default;
    };
}

auto main() -> int
{
    static_assert(test_std::sender<sender<env<scheduler>>>);
    static_assert(std::same_as<env<scheduler>, decltype(test_std::get_env(sender<env<scheduler>>{}))>);
    static_assert(std::same_as<int, decltype(test_std::get_completion_scheduler<test_std::set_value_t>(bad_env{}))>);

    static_assert(not test_std::scheduler<int>);
    static_assert(not test_std::scheduler<no_scheduler_concept>);
    static_assert(not test_std::scheduler<not_queryable>);
    static_assert(not test_std::scheduler<no_schedule>);
    static_assert(not test_std::scheduler<not_equality_comparable>);
    static_assert(not test_std::scheduler<not_copy_constructible>);
    static_assert(not test_std::scheduler<bad_completion_scheduler>);
    static_assert(test_std::scheduler<scheduler>);
}