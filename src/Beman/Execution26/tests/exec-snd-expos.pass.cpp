// src/Beman/Execution26/tests/exe-snd-expos.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/detail/fwd_env.hpp>
#include <Beman/Execution26/detail/make_env.hpp>
#include <Beman/Execution26/detail/join_env.hpp>
#include <Beman/Execution26/detail/query_with_default.hpp>
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

    template <int>
    struct custom_query_t {};
    template <int I>
    constexpr custom_query_t<I> custom_query;
    struct custom_result
    {
        int value;
        auto operator== (custom_result const&) const -> bool = default;
    };

    struct env
    {
        int value{};
        auto query(test_std::get_domain_t const&) const noexcept { return domain{value}; }
        auto query(non_forwardable_t const&) const noexcept { return true; }
        auto query(forwardable_t const&, int a, int b) const noexcept { return (value + a) * b; }
    };

    struct env1
    {
        int value{};
        auto query(custom_query_t<0>, int a) const { return this->value + a; }
        auto query(custom_query_t<1>, int a) const { return this->value + a; }
    };

    struct env2
    {
        int value{};
        auto query(custom_query_t<0>, int a) const { return this->value + a; }
        auto query(custom_query_t<2>, int a) const { return this->value + a; }
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

    auto test_make_env() -> void
    {
        auto env{test_detail::make_env(custom_query<0>, custom_result{43})};
        auto const cenv{env};
        static_assert(test_std::Detail::queryable<decltype(env)>);
        assert(env.query(custom_query<0>) == custom_result{43});
        assert(cenv.query(custom_query<0>) == custom_result{43});
    }

    template <bool Expect, typename Query, typename Env>
    auto test_join_env(Env&& env) -> void
    {
        static_assert(Expect == requires{ env.query(Query(), 17); });
    }
    auto test_join_env() -> void
    {
        env1        e1{42};
        env1 const  ce1{e1};
        env2        e2{17};
        env2 const  ce2{e2};
        
        test_join_env<true, custom_query_t<0>>(e1);
        test_join_env<true, custom_query_t<0>>(ce1);
        test_join_env<true, custom_query_t<1>>(e1);
        test_join_env<true, custom_query_t<1>>(ce1);
        test_join_env<false, custom_query_t<2>>(e1);
        test_join_env<false, custom_query_t<2>>(ce1);
        test_join_env<false, custom_query_t<3>>(e1);
        test_join_env<false, custom_query_t<3>>(ce1);
        test_join_env<true, custom_query_t<0>>(e2);
        test_join_env<true, custom_query_t<0>>(ce2);
        test_join_env<false, custom_query_t<1>>(e2);
        test_join_env<false, custom_query_t<1>>(ce2);
        test_join_env<true, custom_query_t<2>>(e2);
        test_join_env<true, custom_query_t<2>>(ce2);
        test_join_env<false, custom_query_t<3>>(e2);
        test_join_env<false, custom_query_t<3>>(ce2);

        auto        env{test_detail::join_env(e1, e2)};
        auto const& cenv{env};
        
        static_assert(test_std::Detail::queryable<decltype(env)>);
        test_join_env<true, custom_query_t<0>>(env);
        test_join_env<true, custom_query_t<0>>(cenv);
        test_join_env<true, custom_query_t<1>>(env);
        test_join_env<true, custom_query_t<1>>(cenv);
        test_join_env<true, custom_query_t<2>>(env);
        test_join_env<true, custom_query_t<2>>(cenv);
        test_join_env<false, custom_query_t<3>>(env);
        test_join_env<false, custom_query_t<3>>(cenv);
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
    test_make_env();
    test_join_env();
    test_query_with_default();
}