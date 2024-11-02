// src/beman/execution26/tests/exec-sched.test.cpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/scheduler.hpp>
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

    template <bool Expect, typename Signal, typename Result, typename Env>
    auto test_get_completion_scheduler(Result&& result, Env&& env) ->void
    {
        static_assert(Expect == requires{
            test_std::get_completion_scheduler<Signal>(env);
        });
        if constexpr (Expect)
        {
            static_assert(::std::same_as<
                std::remove_cvref_t<Result>,
                std::remove_cvref_t<decltype(test_std::get_completion_scheduler<Signal>(env))>
                >
            );
            ASSERT(result == test_std::get_completion_scheduler<Signal>(env));
        }
    }

    template <bool Expect, typename Scheduler>
    auto test_scheduler() -> void
    {
        static_assert(Expect == test_std::scheduler<Scheduler>);
    }
}

TEST(exec_sched)
{
    static_assert(test_std::sender<sender<env<scheduler>>>);
    static_assert(std::same_as<env<scheduler>, decltype(test_std::get_env(sender<env<scheduler>>{}))>);

    test_get_completion_scheduler<false, test_std::set_value_t>(0, bad_env{});

    test_scheduler<false, int>();
    test_scheduler<false, no_scheduler_concept>();
    test_scheduler<false, not_queryable>();
    test_scheduler<false, no_schedule>();
    test_scheduler<false, not_equality_comparable>();
    test_scheduler<false, not_copy_constructible>();
    test_scheduler<false, bad_completion_scheduler>();
    test_scheduler<true, scheduler>();
}
