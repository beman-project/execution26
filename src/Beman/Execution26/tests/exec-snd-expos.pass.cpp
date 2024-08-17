// src/Beman/Execution26/tests/exe-snd-expos.pass.cpp                 -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#include <Beman/Execution26/detail/query_with_default.hpp>
#include <Beman/Execution26/detail/fwd_env.hpp>
#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct domain
    {
        int value{};
    };
    struct default_domain
    {
        int default_value{};
    };

    constexpr struct forwardable_t
        : test_std::forwarding_query_t
    {
    } forwardable;
    struct non_forwardable_t {};

    struct env
    {
        int value{};
        auto query(test_std::get_domain_t const&) const noexcept { return domain{value}; }
        auto query(non_forwardable_t const&) const noexcept { return true; }
        auto query(forwardable_t const&, int a, int b) const noexcept { return (value + a) * b; }
    };

    template <bool Expect, typename Query>
    auto test_fwd_env_helper() -> void
    {
        env e{42};
        static_assert(Expect == requires(){
            test_detail::fwd_env(e).query(Query());
        });
    }
    auto test_fwd_env() -> void
    {
        env e{42};
        static_assert(test_std::Detail::queryable<decltype(test_detail::fwd_env(e))>);
        test_fwd_env_helper<true, test_std::get_domain_t>();
        test_fwd_env_helper<false, non_forwardable_t>();
        assert(129 == test_detail::fwd_env(e).query(forwardable, 1, 3));
    }

    auto test_query_with_default() -> void
    {
        auto result1{test_detail::query_with_default(test_std::get_domain,
                                                     env{43},
                                                     default_domain{74})};
        static_assert(std::same_as<domain, decltype(result1)>);
        assert(result1.value == 43);

        auto result2{test_detail::query_with_default(test_std::get_domain,
                                                     test_std::empty_env(),
                                                     default_domain{74})};
        static_assert(std::same_as<default_domain, decltype(result2)>);
        assert(result2.default_value == 74);
    }
}

auto main() -> int
{
    test_fwd_env();
    test_query_with_default();
}