// src/Beman/Execution26/tests/exec-get-compl-sched.pass.cpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/detail/get_completion_scheduler.hpp>
#include <Beman/Execution26/detail/forwarding_query.hpp>
#include <Beman/Execution26/detail/set_error.hpp>
#include <Beman/Execution26/detail/set_stopped.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <concepts>
#include <test/execution.hpp>

// ----------------------------------------------------------------------------

namespace
{
    struct tag {};

    template <typename Tag>
    struct scheduler
    {
        int value{};
        auto operator== (scheduler const&) const -> bool = default;
    };

    struct env
    {
        int value{};
        auto query(test_std::get_completion_scheduler_t<test_std::set_value_t> const&) const noexcept
        {
            return scheduler<test_std::set_value_t>{this->value + 1};
        }
        auto query(test_std::get_completion_scheduler_t<test_std::set_error_t> const&) const noexcept
        {
            return scheduler<test_std::set_error_t>{this->value + 2};
        }
        auto query(test_std::get_completion_scheduler_t<test_std::set_stopped_t> const&) const noexcept
        {
            return scheduler<test_std::set_stopped_t>{this->value + 3};
        }
        template <typename Tag>
        auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
        {
            return scheduler<Tag>{this->value};
        }
    };

    template <bool Expect, typename Tag, typename Env>
    auto test_tag(Env&& env) -> void
    {
        static_assert(Expect == requires{ test_std::get_completion_scheduler<Tag>(env); });
    }
}

auto main() -> int
{
    static_assert(std::same_as<test_std::get_completion_scheduler_t<test_std::set_error_t> const,
                  decltype(test_std::get_completion_scheduler<test_std::set_error_t>)>);
    static_assert(std::same_as<test_std::get_completion_scheduler_t<test_std::set_stopped_t> const,
                  decltype(test_std::get_completion_scheduler<test_std::set_stopped_t>)>);
    static_assert(std::same_as<test_std::get_completion_scheduler_t<test_std::set_value_t> const,
                  decltype(test_std::get_completion_scheduler<test_std::set_value_t>)>);
    static_assert(test_std::forwarding_query(test_std::get_completion_scheduler<test_std::set_error_t>));
    static_assert(test_std::forwarding_query(test_std::get_completion_scheduler<test_std::set_stopped_t>));
    static_assert(test_std::forwarding_query(test_std::get_completion_scheduler<test_std::set_value_t>));

    env e{17};
    test_tag<true, test_std::set_error_t>(e);
    test_tag<true, test_std::set_stopped_t>(e);
    test_tag<true, test_std::set_value_t>(e);
    test_tag<false, tag>(e);

    static_assert(::std::same_as<
                    decltype(test_std::get_completion_scheduler<test_std::set_error_t>(e)),
                    scheduler<test_std::set_error_t>>);
    static_assert(::std::same_as<
                    decltype(test_std::get_completion_scheduler<test_std::set_stopped_t>(e)),
                    scheduler<test_std::set_stopped_t>>);
    static_assert(::std::same_as<
                    decltype(test_std::get_completion_scheduler<test_std::set_value_t>(e)),
                    scheduler<test_std::set_value_t>>);
    assert(test_std::get_completion_scheduler<test_std::set_error_t>(e)
           == scheduler<test_std::set_error_t>{19});
    assert(test_std::get_completion_scheduler<test_std::set_stopped_t>(e)
           == scheduler<test_std::set_stopped_t>{20});
    assert(test_std::get_completion_scheduler<test_std::set_value_t>(e)
           == scheduler<test_std::set_value_t>{18});
}