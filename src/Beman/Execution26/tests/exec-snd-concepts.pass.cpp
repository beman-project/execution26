// src/Beman/Execution26/tests/exec-snd-concepts.pass.cpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/detail/sender_for.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/sender_in.hpp>
#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <tuple>

// ----------------------------------------------------------------------------

namespace
{
    struct non_sender {};

    struct own_sender_t: test_std::sender_t {};

    struct std_sender { using sender_concept = test_std::sender_t; };
    struct own_sender { using sender_concept = own_sender_t; };

    struct tag_t {};
    struct tagged_sender
    {
        using sender_concept = test_std::sender_t;
        tag_t tag;
        int   data;
    };
    struct tagged_sender1
    {
        using sender_concept = test_std::sender_t;
        tag_t tag;
        int   data;
        int   child1;
    };
    struct tagged_sender2
    {
        using sender_concept = test_std::sender_t;
        tag_t tag;
        int   data;
        int   child1;
        int   child2;
    };
    struct tagged_sender3
    {
        using sender_concept = test_std::sender_t;
        tag_t tag;
        int   data;
        int   child1;
        int   child2;
        int   child3;
    };
    struct tagged_sender4
    {
        using sender_concept = test_std::sender_t;
        tag_t tag;
        int   data;
        int   child1;
        int   child2;
        int   child3;
        int   child4;
    };

    // -------------------------------------------------------------------------

    auto test_valid_completion_signatures()
    {
        static_assert(not test_detail::valid_completion_signatures<int>);
        static_assert(test_detail::valid_completion_signatures<
            test_std::completion_signatures<>>);
        static_assert(test_detail::valid_completion_signatures<
            test_std::completion_signatures<test_std::set_error_t(int)>>);
    }

    auto test_is_sender() -> void
    {
        static_assert(not test_detail::is_sender<non_sender>);
        static_assert(not test_detail::is_sender<std_sender const&&>);
        static_assert(not test_detail::is_sender<std_sender const&>);
        static_assert(not test_detail::is_sender<std_sender&>);
        static_assert(test_detail::is_sender<std_sender const>);
        static_assert(test_detail::is_sender<std_sender>);
        static_assert(test_detail::is_sender<own_sender>);
    }

    auto test_enable_sender() -> void
    {
        static_assert(not test_detail::enable_sender<non_sender>);
        static_assert(not test_detail::enable_sender<std_sender const&&>);
        static_assert(not test_detail::enable_sender<std_sender const&>);
        static_assert(not test_detail::enable_sender<std_sender&>);
        static_assert(test_detail::enable_sender<std_sender const>);
        static_assert(test_detail::enable_sender<std_sender>);
        static_assert(test_detail::enable_sender<own_sender>);
    }

    auto test_sender() -> void
    {
        static_assert(not test_std::sender<non_sender>);
        static_assert(test_std::sender<std_sender const&&>);
        static_assert(test_std::sender<std_sender const&>);
        static_assert(test_std::sender<std_sender&>);
        static_assert(test_std::sender<std_sender const>);
        static_assert(test_std::sender<std_sender>);
        static_assert(test_std::sender<own_sender>);
    }

    auto test_sender_in() -> void
    {
        struct non_queryable { ~non_queryable() = delete; };
        struct queryable {};
        struct non_sender_in {};
        struct sender_in
        {
            using sender_concept = test_std::sender_t;
        };

        static_assert(not test_std::sender_in<non_sender_in>);
        static_assert(not test_std::sender_in<sender_in, non_queryable>);
        static_assert(test_std::sender_in<sender_in>);
        static_assert(test_std::sender_in<sender_in, queryable>);
        //-dk:TODO add missing test cases
    }

    auto test_tag_of_t() -> void
    {
        //auto&& [tag, data, ...children] = tagged_sender();
        static_assert(std::same_as<tag_t, test_std::tag_of_t<tagged_sender>>);
        static_assert(std::same_as<tag_t, test_std::tag_of_t<tagged_sender1>>);
        static_assert(std::same_as<tag_t, test_std::tag_of_t<tagged_sender2>>);
        static_assert(std::same_as<tag_t, test_std::tag_of_t<tagged_sender3>>);
        static_assert(std::same_as<tag_t, test_std::tag_of_t<tagged_sender4>>);
    }

    auto test_sender_for() -> void
    {
        static_assert(test_std::sender<tagged_sender>);
        static_assert(test_detail::sender_for<tagged_sender, tag_t>);
        static_assert(test_std::sender<tagged_sender1>);
        static_assert(test_detail::sender_for<tagged_sender1, tag_t>);

        static_assert(test_std::sender<std_sender>);
        static_assert(not test_detail::sender_for<std_sender, tag_t>);
    };
}

auto main() -> int
{
    test_valid_completion_signatures();
    test_is_sender();
    test_enable_sender();
    test_sender();
    test_sender_in();
    test_tag_of_t();
    test_sender_for();
}