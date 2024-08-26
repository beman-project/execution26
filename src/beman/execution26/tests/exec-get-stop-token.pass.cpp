// src/beman/execution26/tests/exec-get-stop-token.pass.cpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <beman/execution26/stop_token.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct non_stop_token {};
    struct stop_token
    {
        template <typename CB>
        struct callback_type {};
        auto stop_requested() const noexcept -> bool { return {}; }
        auto stop_possible() const noexcept -> bool { return {}; }
        auto operator== (stop_token const&) const noexcept -> bool = default;
    };

    struct no_get_stop_token {};

    struct non_const_get_stop_token
    {
        auto query(test_std::get_stop_token_t const&) noexcept -> stop_token { return {}; }
    };
    template <bool Noexcept>
    struct has_get_stop_token
    {
        auto query(test_std::get_stop_token_t const&) const noexcept(Noexcept) -> stop_token { return {}; }
    };

    struct inconsistent_get_stop_token
    {
        auto query(test_std::get_stop_token_t const&) const noexcept -> stop_token { return {}; }
        auto query(test_std::get_stop_token_t const&) noexcept -> non_stop_token { return {}; }
    };


    template <typename Result, typename Object>
    auto test_get_stop_token(Object&& object)
    {
        static_assert(requires { test_std::get_stop_token(object); });
        static_assert(std::same_as<Result, decltype(test_std::get_stop_token(object))>);
    }
}

auto main() -> int
{
    static_assert(std::same_as<test_std::get_stop_token_t const,
                               decltype(test_std::get_stop_token)>);
    static_assert(test_std::forwarding_query(test_std::get_stop_token));

    test_get_stop_token<test_std::never_stop_token>(no_get_stop_token());
    test_get_stop_token<stop_token>(has_get_stop_token<true>());
    test_get_stop_token<test_std::never_stop_token>(has_get_stop_token<false>());
    test_get_stop_token<test_std::never_stop_token>(non_const_get_stop_token());
    test_get_stop_token<stop_token>(inconsistent_get_stop_token());

    static_assert(test_std::stoppable_token<stop_token>);
}