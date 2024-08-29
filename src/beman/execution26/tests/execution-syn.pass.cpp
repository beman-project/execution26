// src/beman/execution26/tests/execution-syn.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/single_sender.hpp>
#include <beman/execution26/detail/single_sender_value_type.hpp>
#include <beman/execution26/detail/type_list.hpp>
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

    struct test_env {};
    struct no_value_env {};
    struct single_type_sender
    {
        struct arg {};
        struct error {};
        using sender_concept = test_std::sender_t;
        using test_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_value_t(arg&),
            test_std::set_stopped_t()
            >;
        auto get_completion_signatures(test_env const&) const noexcept
        {
            return test_signatures();
        }
        using empty_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_value_t(bool&),
            test_std::set_stopped_t()
            >;
        auto get_completion_signatures(test_std::empty_env const&) const noexcept
        {
            return empty_signatures();
        }
        using no_value_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_stopped_t()
            >;
        auto get_completion_signatures(no_value_env const&) const noexcept
        {
            return no_value_signatures();
        }
    };

    struct void_sender
    {
        struct error {};
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_value_t(),
            test_std::set_stopped_t()
            >;
    };

    struct no_value_sender
    {
        struct error {};
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_stopped_t()
            >;
    };

    struct multi_single_sender
    {
        struct arg {};
        struct error {};
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_value_t(arg&),
            test_std::set_value_t(int),
            test_std::set_stopped_t()
            >;
    };

    struct multi_type_sender
    {
        struct arg {};
        struct error {};
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
            test_std::set_error_t(error),
            test_std::set_error_t(int),
            test_std::set_value_t(arg&, bool const&, int),
            test_std::set_stopped_t()
            >;
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

    struct env {};
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
        static_assert(requires{
            typename test_std::completion_signatures_of_t<sender_in>;
            { test_std::completion_signatures_of_t<sender_in>{} }
            -> std::same_as<sender_in::completion_signatures>;
        });

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

    auto test_type_list() -> void
    {
        struct arg {};
        static_assert(std::same_as<
            test_detail::type_list<>,
            test_detail::type_list<>
        >);
        static_assert(std::same_as<
            test_detail::type_list<arg, bool, char&, double const&, long&&>,
            test_detail::type_list<arg, bool, char&, double const&, long&&>
        >);
    }

    template <typename T>
    auto test_single_sender_value_type() -> void
    {
        static_assert(test_std::sender<single_type_sender>);
        static_assert(test_std::sender<void_sender>);
        static_assert(test_std::sender<multi_single_sender>);
        static_assert(test_std::sender<multi_type_sender>);
        static_assert(test_std::sender<no_value_sender>);

        static_assert(requires{
            typename test_detail::single_sender_value_type<single_type_sender, test_env>;
        });
        static_assert(std::same_as<
            single_type_sender::arg,
            test_detail::single_sender_value_type<single_type_sender, test_env>
        >);
        static_assert(requires{
            typename test_detail::single_sender_value_type<single_type_sender, test_std::empty_env>;
        });
        static_assert(std::same_as<
            bool,
            test_detail::single_sender_value_type<single_type_sender, test_std::empty_env>
        >);
        static_assert(requires{
            typename test_detail::single_sender_value_type<void_sender, test_std::empty_env>;
        });
        static_assert(std::same_as<
            void,
            test_detail::single_sender_value_type<void_sender, test_std::empty_env>
        >);
        static_assert(requires{
            typename test_detail::single_sender_value_type<multi_type_sender, test_std::empty_env>;
        });
        static_assert(std::same_as<
            std::tuple<multi_type_sender::arg, bool, int>,
            test_detail::single_sender_value_type<multi_type_sender, test_std::empty_env>
        >);
        static_assert(not requires{
            typename T;
            typename test_detail::single_sender_value_type<multi_single_sender, test_std::empty_env>;
        });
        static_assert(not requires{
            typename T;
            typename test_detail::single_sender_value_type<no_value_sender, test_std::empty_env>;
        });
    }

    auto test_single_sender() -> void
    {
        //static_assert(test_std::single_sender<single_type_sender, test_std::empty_env>);
        //static_assert(test_std::single_sender<single_type_sender, no_value_env>);
        //static_assert(test_std::single_sender<void_sender, test_std::empty_env>);
        //static_assert(test_std::single_sender<multi_single_sender, test_std::empty_env>);
        //static_assert(not test_std::single_sender<multi_type_sender, test_std::empty_env>);
        //static_assert(not test_std::single_sender<no_value_sender, test_std::empty_env>);
    }
}

auto main() -> int
{
    test_schedule_result_t();
    test_env_of_t();
    test_decayed_tuple();
    test_variant_or_empty();
    test_completion_signatures_of_t<int>();
    test_type_list();
    test_single_sender_value_type<int>();
    test_single_sender();
}
