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
#include <Beman/Execution26/detail/default_impls.hpp>
#include <Beman/Execution26/detail/impls_for.hpp>
#include <Beman/Execution26/detail/state_type.hpp>
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

    namespace completion_domain {
        struct domain {};
        struct other {};
        struct common {};
        struct none {};
        struct mismatch {};

        template <typename> struct env;
        template <typename> struct scheduler;

        template <typename W>
        struct sender
        {
            using sender_concept = test_std::sender_t;
            auto get_env() const noexcept -> env<W> { return {}; }
        };
        template <typename W>
        struct scheduler
        {
            using scheduler_concept = test_std::scheduler_t;
            auto schedule() noexcept -> sender<W> { return {}; }
            auto operator== (scheduler const&) const -> bool = default;
            auto query(test_std::get_domain_t const&) const noexcept -> domain { return {}; }
        };

        template <>
        struct scheduler<none>
        {
            using scheduler_concept = test_std::scheduler_t;
            auto schedule() noexcept -> sender<none> { return {}; }
            auto operator== (scheduler const&) const -> bool = default;
        };

        template <>
        struct env<common>
        {
            template <typename Tag>
            auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
                -> scheduler<common>
            {
                return {};
            }
        };

        template <>
        struct env<none>
        {
            template <typename Tag>
            auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
                -> scheduler<none>
            {
                return {};
            }
        };

        struct one_missing {};
        template <>
        struct env<one_missing>
        {
            template <typename Tag>
            auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
                -> scheduler<one_missing>
            {
                return {};
            }
            auto query(test_std::get_completion_scheduler_t<test_std::set_error_t> const&) const noexcept = delete;
        };

        struct two_missing {};
        template <>
        struct env<two_missing>
        {
            auto query(test_std::get_completion_scheduler_t<test_std::set_value_t> const&) const noexcept
                -> scheduler<two_missing>
            {
                return {};
            }
        };
    }
    auto test_completion_domain() -> void
    {
        namespace cd = completion_domain;
        static_assert(test_std::sender<cd::sender<cd::common>>);
        static_assert(std::same_as<cd::env<cd::common>, decltype(test_std::get_env(cd::sender<cd::common>{}))>);
        static_assert(std::same_as<cd::domain, decltype(test_std::get_domain(test_std::get_completion_scheduler<test_std::set_error_t>(cd::env<cd::common>{})))>);
        static_assert(std::same_as<cd::domain, decltype(test_std::get_domain(test_std::get_completion_scheduler<test_std::set_stopped_t>(cd::env<cd::common>{})))>);
        static_assert(std::same_as<cd::domain, decltype(test_std::get_domain(test_std::get_completion_scheduler<test_std::set_value_t>(cd::env<cd::common>{})))>);
        static_assert(std::same_as<cd::domain, decltype(test_detail::completion_domain((cd::sender<cd::common>{})))>);
        static_assert(std::same_as<cd::domain, decltype(test_detail::completion_domain((cd::sender<cd::one_missing>{})))>);
        static_assert(std::same_as<cd::domain, decltype(test_detail::completion_domain((cd::sender<cd::two_missing>{})))>);
        static_assert(std::same_as<test_std::default_domain, decltype(test_detail::completion_domain((cd::sender<cd::none>{})))>);
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
        #if 0
        static_assert(test_std::sender<test_sender>);
        env const e;
        test_sender const sndr;
        auto dom{test_detail::get_domain_late(sndr, e)};
        (void)dom;
        //-dk:TOO test
        #endif
    }

    auto test_default_impls_get_attrs() -> void
    {
        struct env { int value; };
        struct child1 { auto get_env() const noexcept { return env{1}; } };
        struct child2 { auto get_env() const noexcept { return env{2}; } };

        static_assert(noexcept(test_detail::default_impls::get_attrs(0, child1{})));
        static_assert(std::same_as<test_detail::fwd_env<env>,
            decltype(test_detail::default_impls::get_attrs(0, child1{}))>);
        static_assert(std::same_as<test_std::empty_env,
            decltype(test_detail::default_impls::get_attrs(0, child1{}, child2{}))>);
    }

    auto test_default_impls_get_env() -> void
    {
        struct env { int value; };
        struct receiver { auto get_env() const noexcept { return env{1}; } };

        int arg{};
        static_assert(noexcept(test_detail::default_impls::get_env(0, arg, receiver{})));
        static_assert(std::same_as<test_detail::fwd_env<env>,
            decltype(test_detail::default_impls::get_env(0, arg, receiver{}))>);
    }

    auto test_default_impls_get_state() -> void
    {
        struct tag {};
        struct data
        {
            int v1{}; int v2{};
            auto operator== (data const&) const -> bool = default;
        };
        struct sender0
        {
            tag t;
            data d{1, 2};
        };
        struct sender1
        {
            tag t;
            data d{1, 2};
            int i1;
        };
        struct sender2
        {
            tag t;
            data d{1, 2};
            int i1;
            int i2;
        };
        struct sender3
        {
            tag t;
            data d{1, 2};
            int i1;
            int i2;
            int i3;
        };
        struct sender4
        {
            tag t;
            data d{1, 2};
            int i1;
            int i2;
            int i3;
            int i4;
        };
        struct receiver
        {
        };

        sender0       s{};
        sender0 const cs{};
        receiver      r{};
        static_assert(noexcept(test_detail::default_impls::get_state(sender0{}, r)));
        static_assert(std::same_as<data&&,
            decltype(test_detail::default_impls::get_state(sender0{}, r))>);
        assert((data{1, 2}) == test_detail::default_impls::get_state(sender0{}, r));
        static_assert(std::same_as<data&&,
            decltype(test_detail::default_impls::get_state(sender1{}, r))>);
        assert((data{1, 2}) == test_detail::default_impls::get_state(sender1{}, r));
        static_assert(std::same_as<data&&,
            decltype(test_detail::default_impls::get_state(sender2{}, r))>);
        assert((data{1, 2}) == test_detail::default_impls::get_state(sender2{}, r));
        static_assert(std::same_as<data&&,
            decltype(test_detail::default_impls::get_state(sender3{}, r))>);
        assert((data{1, 2}) == test_detail::default_impls::get_state(sender3{}, r));
        static_assert(std::same_as<data&&,
            decltype(test_detail::default_impls::get_state(sender4{}, r))>);
        assert((data{1, 2}) == test_detail::default_impls::get_state(sender4{}, r));
        static_assert(std::same_as<data&,
            decltype(test_detail::default_impls::get_state(s, r))>);
        static_assert(std::same_as<data const&,
            decltype(test_detail::default_impls::get_state(cs, r))>);
    }
    auto test_default_impls_start() -> void
    {
        struct state
        {
            bool called{false};
            auto start() noexcept -> void { called = true; }
        };
        int a0{};
        int a1{};
        state s1;
        state s2;
        state s3;
        state s4;
        state s5;
        static_assert(noexcept(test_detail::default_impls::start(a0, a1, s1, s2, s3, s4, s5)));
        assert(s1.called == false);
        assert(s2.called == false);
        assert(s3.called == false);
        assert(s4.called == false);
        assert(s5.called == false);
        test_detail::default_impls::start(a0, a1, s1, s2, s3, s4, s5);
        assert(s1.called == true);
        assert(s2.called == true);
        assert(s3.called == true);
        assert(s4.called == true);
        assert(s5.called == true);
    }
    template <typename Impls>
    auto test_default_impls_complete(Impls) -> void
    {
        struct arg {};
        struct receiver { bool& called; };
        struct state {};

        bool called{false};
        auto non_tag = [](receiver&&, int){};
        auto tag = [](receiver&& r, int, arg){ r.called = true; };
        receiver r{called};
        state s{};

        static_assert(not requires{
            Impls::complete(::std::integral_constant<int, 0>{}, s, r, non_tag, 0, arg{});
        });
        static_assert(requires{
            Impls::complete(::std::integral_constant<int, 0>{}, s, r, tag, 0, arg{});
        });
        static_assert(noexcept(
            Impls::complete(::std::integral_constant<int, 0>{}, s, r, tag, 0, arg{})
        ));

        assert(r.called == false);
        Impls::complete(::std::integral_constant<int, 0>{}, s, r, tag, 0, arg{});
        assert(r.called == true);
    }

    auto test_default_impls() -> void
    {
        test_default_impls_get_attrs();
        test_default_impls_get_env();
        test_default_impls_get_state();
        test_default_impls_start();
        test_default_impls_complete(test_detail::default_impls{});
    }

    auto test_impls_for() -> void
    {
        struct tag {};

        static_assert(std::derived_from<test_detail::impls_for<tag>,
                                        test_detail::default_impls>);
    }

    auto test_state_type() -> void
    {
        struct tag {};
        struct state {};
        struct sender
        {
            tag t;
            state s;
        };
        struct receiver {};

        static_assert(std::same_as<state, test_detail::state_type<sender, receiver>>);
    }
}

auto main() -> int
{
    test_fwd_env();
    test_make_env();
    test_join_env();
    test_sched_attrs();
    test_sched_env();
    test_completion_domain();
    test_query_with_default();
    test_get_domain_late();
    test_default_impls();
    test_impls_for();
    test_state_type();
}