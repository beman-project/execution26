// src/Beman/Execution26/tests/exe-snd-expos.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/detail/fwd_env.hpp>
#include <Beman/Execution26/detail/make_env.hpp>
#include <Beman/Execution26/detail/join_env.hpp>
#include <Beman/Execution26/detail/sched_attrs.hpp>
#include <Beman/Execution26/detail/sched_env.hpp>
#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/query_with_default.hpp>
#include <Beman/Execution26/detail/get_domain_late.hpp>
#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct domain
    {
        int value{};
        auto operator== (domain const&) const -> bool = default;
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

    struct test_sender
    {
        using sender_concept = test_std::sender_t;
    };

    struct scheduler
    {
        int value{};
        auto query(test_std::get_domain_t) const { return domain{this->value}; }

        auto operator== (scheduler const&) const -> bool = default;
    };

    // ------------------------------------------------------------------------

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

        assert(env.query(custom_query<0>, 13) == 55);
        assert(cenv.query(custom_query<0>, 13) == 55);
        assert(env.query(custom_query<1>, 13) == 55);
        assert(cenv.query(custom_query<1>, 13) == 55);
        assert(env.query(custom_query<2>, 13) == 30);
        assert(cenv.query(custom_query<2>, 13) == 30);
    }

    auto test_sched_attrs() -> void
    {
        scheduler sched{17};
        assert(sched.query(test_std::get_domain) == domain{17});

        auto attrs{test_detail::sched_attrs(sched)};
        static_assert(test_detail::queryable<decltype(attrs)>);

        auto sched_error{attrs.query(test_std::get_completion_scheduler<test_std::set_error_t>)};
        static_assert(::std::same_as<scheduler, decltype(sched_error)>);
        assert(sched_error == sched);

        auto sched_stopped{attrs.query(test_std::get_completion_scheduler<test_std::set_stopped_t>)};
        static_assert(::std::same_as<scheduler, decltype(sched_stopped)>);
        assert(sched_stopped == sched);

        auto sched_value{attrs.query(test_std::get_completion_scheduler<test_std::set_value_t>)};
        static_assert(::std::same_as<scheduler, decltype(sched_value)>);
        assert(sched_value == sched);

        auto dom{attrs.query(test_std::get_domain)};
        static_assert(::std::same_as<domain, decltype(dom)>);
        assert(dom == domain{17});
    }

    auto test_sched_env() -> void
    {
        scheduler sched{17};
        assert(sched.query(test_std::get_domain) == domain{17});

        auto env{test_detail::sched_env(sched)};
        static_assert(test_detail::queryable<decltype(env)>);

        auto qsched{env.query(test_std::get_scheduler)};
        static_assert(::std::same_as<scheduler, decltype(qsched)>);
        assert(qsched == sched);

        auto dom{env.query(test_std::get_domain)};
        static_assert(::std::same_as<domain, decltype(dom)>);
        assert(dom == domain{17});
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

    auto test_get_domain_late() -> void
    {
        static_assert(test_std::sender<test_sender>);
        env const e;
        test_sender const sndr;
        auto dom{test_detail::get_domain_late(sndr, e)};
        (void)dom;
    }
}

auto main() -> int
{
    test_fwd_env();
    test_make_env();
    test_join_env();
    test_sched_attrs();
    test_sched_env();
    test_query_with_default();
    test_get_domain_late();
}