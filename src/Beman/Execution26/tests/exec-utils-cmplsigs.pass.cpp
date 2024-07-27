// src/Beman/Execution26/tests/exec-utils-cmplsigs.pass.cpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>

// ----------------------------------------------------------------------------

namespace
{
    struct error {};
    template <int> struct arg {};

    auto test_completion_signature()
    {
        static_assert(not test_detail::completion_signature<int>);
        static_assert(not test_detail::completion_signature<int()>);

        static_assert(not test_detail::completion_signature<test_std::set_error_t()>);
        static_assert(test_detail::completion_signature<test_std::set_error_t(error)>);
        static_assert(not test_detail::completion_signature<test_std::set_error_t(int, int)>);

        static_assert(test_detail::completion_signature<test_std::set_stopped_t()>);
        static_assert(not test_detail::completion_signature<test_std::set_stopped_t(int)>);

        static_assert(test_detail::completion_signature<test_std::set_value_t()>);
        static_assert(test_detail::completion_signature<test_std::set_value_t(arg<0>)>);
        static_assert(test_detail::completion_signature<test_std::set_value_t(arg<0>, arg<1>)>);
        static_assert(test_detail::completion_signature<test_std::set_value_t(arg<0>, arg<1>, arg<2>)>);
    }

    auto test_completion_signatures() -> void
    {
        test_std::completion_signatures<>();
        test_std::completion_signatures<test_std::set_error_t(error)>();
        //-dk:TODO create negative compilation test_std::completion_signatures<test_std::set_error_t()>();
        //-dk:TODO create negative compilation test_std::completion_signatures<test_std::set_error_t(error, error)>();

        test_std::completion_signatures<test_std::set_stopped_t()>();
        //-dk:TODO create negative compilation test_std::completion_signatures<test_std::set_stopped_t(int)>();

        test_std::completion_signatures<test_std::set_value_t()>();
        test_std::completion_signatures<test_std::set_value_t(arg<0>)>();
        test_std::completion_signatures<test_std::set_value_t(arg<0>, arg<1>)>();
        test_std::completion_signatures<test_std::set_value_t(arg<0>, arg<1>, arg<2>)>();

        test_std::completion_signatures<
            test_std::set_value_t(),
            test_std::set_value_t(arg<0>),
            test_std::set_value_t(arg<0>, arg<1>),
            test_std::set_value_t(arg<0>, arg<1>, arg<2>),
            test_std::set_error_t(error),
            test_std::set_error_t(error),
            test_std::set_stopped_t(),
            test_std::set_stopped_t()
            >();
    }

    auto test_valid_completion_signatures() -> void
    {
        static_assert(not test_detail::valid_completion_signatures<int>);
        static_assert(test_detail::valid_completion_signatures<
            test_std::completion_signatures<>>);
        static_assert(test_detail::valid_completion_signatures<
            test_std::completion_signatures<test_std::set_value_t()>>);
    }
}

auto main() -> int
{
    test_completion_signature();
    test_completion_signatures();
    test_valid_completion_signatures();
    //-dk:TODO add other components mentioned by [exec.utils.cmplsigs]:
    //-dk:TODO test indirect_meta_apply
    //-dk:TODO test always_true
    //-dk:TODO test gather-signatures
    //-dk:TODO test value_types_of_t
    //-dk:TODO test error_types_of_t
    //-dk:TODO test sends_stopped
}