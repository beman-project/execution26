// src/Beman/Execution26/tests/exec-fwd-env.pass.cpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct derived
        : test_std::forwarding_query_t
    {
    };

    template <bool Noexcept = true, typename Result = bool, Result Value = true>
    struct static_query
    {
        constexpr auto query(test_std::forwarding_query_t) noexcept(Noexcept) -> Result
        {
            return Value;
        }
    };

    struct rvalue_query
    {
        constexpr auto query(test_std::forwarding_query_t) && noexcept -> bool
        {
            return true;
        }
    };

    struct const_query
    {
        constexpr auto query(test_std::forwarding_query_t&&) noexcept -> bool = delete;
        constexpr auto query(test_std::forwarding_query_t&) noexcept -> bool = delete;
        constexpr auto query(test_std::forwarding_query_t const&) noexcept -> bool
        {
            return true;
        }
    };

    struct dynamic_query
    {
        bool value{true};
        constexpr auto query(test_std::forwarding_query_t) && noexcept -> bool
        {
            return value;
        }
    };
}

auto main(int ac, char*[]) -> int
{
    static_assert(std::same_as<test_std::forwarding_query_t const,
                               decltype(test_std::forwarding_query)>);

    static_assert(not test_std::forwarding_query(0));
    static_assert(test_std::forwarding_query(derived()));
    static_assert(test_std::forwarding_query(static_query<>()));
    static_assert(not test_std::forwarding_query(static_query<false>()));
    static_assert(not test_std::forwarding_query(static_query<true, int>()));
    static_assert(not test_std::forwarding_query(static_query<true, bool, false>()));

    static_assert(test_std::forwarding_query(rvalue_query()));
    rvalue_query rq{};
    static_assert(not test_std::forwarding_query(rq));
    rvalue_query crq{};
    static_assert(not test_std::forwarding_query(crq));

    static_assert(test_std::forwarding_query(const_query()));

    static_assert(test_std::forwarding_query(dynamic_query()));
    static_assert(test_std::forwarding_query(dynamic_query{true}));
    static_assert(not test_std::forwarding_query(dynamic_query{false}));
    assert(test_std::forwarding_query(dynamic_query{ac == 1}));
    assert(not test_std::forwarding_query(dynamic_query{ac != 1}));
}