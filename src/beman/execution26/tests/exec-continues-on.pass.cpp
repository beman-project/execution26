// src/beman/execution26/tests/exec-continues-on.pass.cpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/continues_on.hpp>
#include <beman/execution26/detail/get_domain_late.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct non_scheduler {};
    struct non_sender {};

    struct custom_domain
        : test_std::default_domain
    {
    };

    struct scheduler
    {
        struct env
        {
            auto query(test_std::get_completion_scheduler_t<test_std::set_value_t> const&) const noexcept -> scheduler{ return {}; }
        };
        struct sender
        {
            template <typename Receiver>
            struct state
            {
                using operation_state_concept = test_std::operation_state_t;
                std::remove_cvref_t<Receiver> receiver;
                auto start() & noexcept -> void {
                    test_std::set_value(::std::move(this->receiver));
                }
            };
            using sender_concept = test_std::sender_t;
            using completion_signatures = test_std::completion_signatures<test_std::set_value_t()>;
            auto get_env() const noexcept -> env { return {}; }
            template <test_std::receiver Receiver>
            auto connect(Receiver&& receiver) -> state<Receiver> {
                return { std::forward<Receiver>(receiver) };
            }
        };
        using scheduler_concept = test_std::scheduler_t;
        auto schedule() -> sender { return {}; }
        auto operator== (scheduler const&) const -> bool = default;

        auto query(test_std::get_domain_t const&) const noexcept -> custom_domain { return {}; }
    };
    struct sender
    {
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<test_std::set_value_t()>;

        template <typename Receiver>
        struct state
        {
            using operation_state_concept = test_std::operation_state_t;
            std::remove_cvref_t<Receiver> receiver;
            auto start() & noexcept -> void {
                test_std::set_value(::std::move(this->receiver));
            }
        };
        template <test_std::receiver Receiver>
        auto connect(Receiver&& receiver) -> state<Receiver> {
            return { std::forward<Receiver>(receiver) };
        }
    };

    template <bool Expect, typename Scheduler, typename Sender>
    auto test_constraints(Scheduler&& scheduler, Sender&& sender)
    {
        static_assert(Expect == requires{ test_std::continues_on(sender, scheduler); });
        static_assert(Expect == requires{
            { test_std::continues_on(::std::forward<Sender>(sender), ::std::forward<Scheduler>(scheduler)) } -> test_std::sender;
        });
        static_assert(Expect == requires{ ::std::forward<Sender>(sender) | test_std::continues_on(scheduler); });

        if constexpr (Expect)
        {
            auto domain{test_std::get_domain(scheduler)};
            test::check_type<custom_domain&>(domain);

            auto s{test_std::continues_on(::std::forward<Sender>(sender), ::std::forward<Scheduler>(scheduler))};
            auto late{test_detail::get_domain_late(s, test_std::empty_env{})};
            test::check_type<custom_domain&>(late);
        }
    }

    template <typename Scheduler, typename Sender>
    auto test_use(Scheduler&& scheduler, Sender&& sender)
    {
        auto s{test_std::continues_on(::std::forward<Sender>(sender), ::std::forward<Scheduler>(scheduler))};

        static_assert(test_std::sender<decltype(s)>);
        test_std::sync_wait(std::move(s));
    }
}

TEST(exec_continues_on)
{
    static_assert(std::same_as<test_std::continues_on_t const, decltype(test_std::continues_on)>);
    static_assert(not test_std::scheduler<non_scheduler>);
    static_assert(test_std::scheduler<scheduler>);
    static_assert(not test_std::sender<non_sender>);
    static_assert(test_std::sender<sender>);

    test_constraints<false>(non_scheduler{}, non_sender{});
    test_constraints<false>(non_scheduler{}, sender{});
    test_constraints<false>(scheduler{}, non_sender{});
    test_constraints<true>(scheduler{}, sender{});
    
    test_use(scheduler{}, sender{});
}
