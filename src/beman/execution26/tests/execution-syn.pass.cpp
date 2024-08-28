// src/beman/execution26/tests/execution-syn.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/schedule_result_t.hpp>
#include <beman/execution26/detail/env_of_t.hpp>
#include <beman/execution26/detail/decayed_tuple.hpp>
#include <beman/execution26/detail/variant_or_empty.hpp>
#include <beman/execution26/detail/completion_signatures_of_t.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct scheduler
    {
        struct env
        {
            template <test_detail::completion_tag Tag>
            auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
                -> scheduler { return {}; }
        };
        struct sender
        {
            using sender_concept = test_std::sender_t;
            auto get_env() const noexcept -> env { return {}; }
        };
        using scheduler_concept = test_std::scheduler_t;

        auto schedule() const noexcept -> sender { return {}; }
        auto operator== (scheduler const&) const -> bool = default;
    };

    auto test_schedule_result_t() -> void
    {
        static_assert(test_std::sender<scheduler::sender>);
        static_assert(test_std::scheduler<scheduler>);
        static_assert(std::same_as<scheduler::sender, test_std::schedule_result_t<scheduler>>);
    }

    auto test_env_of_t() -> void
    {
        struct env {};
        struct object {};
        struct object_with_env
        {
            auto get_env() const noexcept -> env { return {}; }
        };

        static_assert(std::same_as<test_std::empty_env, test_std::env_of_t<object>>);
        static_assert(std::same_as<env, test_std::env_of_t<object_with_env>>);
    }

    auto test_decayed_tuple() -> void
    {
        static_assert(std::same_as<std::tuple<int, bool, char>,
            test_detail::decayed_tuple<int, bool, char>>);
        static_assert(std::same_as<std::tuple<int, bool, char>,
            test_detail::decayed_tuple<int const, bool volatile, char const volatile>>);
        static_assert(std::same_as<std::tuple<int, bool, char*>,
            test_detail::decayed_tuple<int&, bool&&, char[5]>>);
        static_assert(std::same_as<std::tuple<int, bool, char*>,
            test_detail::decayed_tuple<int const&, bool volatile&&, char[5]>>);
    }

    auto test_variant_or_empty() -> void
    {
        static_assert(not std::default_initializable<test_detail::empty_variant>);

        static_assert(std::same_as<
            test_detail::variant_or_empty<>,
            test_detail::empty_variant
            >);
        static_assert(std::same_as<
            test_detail::variant_or_empty<int&>,
            std::variant<int>
            >);
        static_assert(std::same_as<
            test_detail::variant_or_empty<int&, int const>,
            std::variant<int>
            >);
    }

    struct sender_in
    {
        struct arg {};

        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
            test_std::set_value_t(arg),
            test_std::set_stopped_t()
        >;
    };

    template <typename T>
    auto test_completion_signatures_of_t() -> void
    {
        struct env {};
        struct non_sender {};
        static_assert(not requires{
            typename T;
            typename test_std::completion_signatures_of_t<non_sender>;
        });

        static_assert(test_std::sender<sender_in>);
        static_assert(test_std::sender_in<sender_in>);
        static_assert(requires{
            typename test_std::completion_signatures_of_t<sender_in>;
            { test_std::completion_signatures_of_t<sender_in>{} }
            -> std::same_as<sender_in::completion_signatures>;
        });

        struct sender_with_get
        {
            struct arg {};

            using sender_concept = test_std::sender_t;

            using empty_sigs = test_std::completion_signatures<
                test_std::set_value_t(arg),
                test_std::set_stopped_t()
            >;
            auto get_completion_signatures(test_std::empty_env) const noexcept
            {
                return empty_sigs{};
            }

            using env_sigs = test_std::completion_signatures<
                test_std::set_value_t(arg, arg),
                test_std::set_stopped_t()
            >;
            auto get_completion_signatures(env) const noexcept
            {
                return env_sigs{};
            }
        };
        static_assert(not std::same_as<typename sender_with_get::empty_sigs,
            typename sender_with_get::env_sigs>);
        static_assert(test_std::sender<sender_with_get>);
        static_assert(test_std::sender_in<sender_with_get>);
        static_assert(requires{
            typename test_std::completion_signatures_of_t<sender_with_get>;
            { test_std::completion_signatures_of_t<sender_with_get>{} }
            -> std::same_as<typename sender_with_get::empty_sigs>;
        });
        static_assert(requires{
            typename test_std::completion_signatures_of_t<sender_with_get>;
            { test_std::completion_signatures_of_t<sender_with_get, env>{} }
            -> std::same_as<typename sender_with_get::env_sigs>;
        });

    }
}

auto main() -> int
{
    test_schedule_result_t();
    test_env_of_t();
    test_decayed_tuple();
    test_variant_or_empty();
    test_completion_signatures_of_t<int>();
}
