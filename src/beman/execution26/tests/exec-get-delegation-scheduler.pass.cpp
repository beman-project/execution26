// src/beman/execution26/tests/exec-get-delegation-scheduler.pass.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/get_delegation_scheduler.hpp>

#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/get_completion_scheduler.hpp>
#include <beman/execution26/detail/scheduler.hpp>
#include <beman/execution26/detail/sender.hpp>

#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct scheduler;

    template <bool Noexcept, typename Scheduler>
    struct env
    {
        int value{};

        auto query(test_std::get_delegation_scheduler_t const&) const noexcept(Noexcept)
        {
            return Scheduler{this->value};
        }

        template <typename Tag>
        auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
        {
            return scheduler{this->value};
        }
    };

    struct scheduler
    {
        using scheduler_concept = test_std::scheduler_t;

        struct sender
        {
            using sender_concept = test_std::sender_t;
            int value{};
            auto get_env() const noexcept -> env<true, scheduler>
            {
                return {this->value};
            }
        };

        int value{};

        auto schedule() noexcept -> sender { return {this->value}; }
        auto operator== (scheduler const&) const -> bool = default;
    }; 

    template <bool Expect, typename Scheduler = void>
    auto test_get_delegation_scheduler(auto&& env) -> void
    {
        static_assert(Expect == requires{
            test_std::get_delegation_scheduler(env);
        });
        if constexpr (Expect)
        {
            assert(17 == test_std::get_delegation_scheduler(env).value);
            //assert(Scheduler{env.value} == test_std::get_delegation_scheduler(env));
        }
    }
}

auto main() -> int
{
    static_assert(test_std::sender<scheduler::sender>);
    static_assert(test_std::scheduler<scheduler>);
    static_assert(std::same_as<
        test_std::get_delegation_scheduler_t const,
        decltype(test_std::get_delegation_scheduler)
    >);
    static_assert(test_std::forwarding_query((test_std::get_delegation_scheduler)));

    test_get_delegation_scheduler<false>(test_std::empty_env{});
    test_get_delegation_scheduler<false>(env<false, scheduler>{});
    test_get_delegation_scheduler<false>(env<true, scheduler::sender>{});
    test_get_delegation_scheduler<true>(env<true, scheduler>{17});
}