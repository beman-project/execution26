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
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<>;
    };

    template <typename T>
    auto test_completion_signatures_of_t() -> void
    {
        struct non_sender {};
        static_assert(not requires{
            typename T;
            typename test_std::completion_signatures_of_t<non_sender>;
        });

        static_assert(test_std::sender<sender_in>);
        static_assert(test_std::sender_in<sender_in>);
        //-dk:TODO add actually meaningful test

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
