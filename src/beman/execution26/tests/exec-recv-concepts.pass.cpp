// src/beman/execution26/tests/exec-recv-concepts.pass.cpp            -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/valid_completion_for.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>

// ----------------------------------------------------------------------------

namespace
{
    struct arg {};
    struct error {};

    template <typename... T>
    struct value_receiver
    {
        auto set_value(T...) && noexcept -> void {}
    };

    template <typename T>
    struct error_receiver
    {
        auto set_error(T) && noexcept -> void {}
    };

    struct stopped_receiver
    {
        auto set_stopped() && noexcept -> void {}
    };

    auto test_valid_completion_for() -> void
    {
        static_assert(test_detail::valid_completion_for<
            test_std::set_value_t(), value_receiver<>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_stopped_t(), value_receiver<>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_value_t(arg), value_receiver<>>);
        static_assert(test_detail::valid_completion_for<
            test_std::set_value_t(arg), value_receiver<arg>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_value_t(int, arg), value_receiver<int>>);
        static_assert(test_detail::valid_completion_for<
            test_std::set_value_t(int, arg), value_receiver<int, arg>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_error_t(error), value_receiver<error>>);

        static_assert(not test_detail::valid_completion_for<
            test_std::set_stopped_t(), error_receiver<error>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_value_t(error), error_receiver<error>>);
        static_assert(test_detail::valid_completion_for<
            test_std::set_error_t(error), error_receiver<error>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_error_t(error), error_receiver<int>>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_error_t(int), error_receiver<error>>);

        static_assert(test_detail::valid_completion_for<
            test_std::set_stopped_t(), stopped_receiver>);
        static_assert(not test_detail::valid_completion_for<
            test_std::set_value_t(), stopped_receiver>);
    }
}

auto main() -> int
{
    test_valid_completion_for();
}