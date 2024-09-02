// src/beman/execution26/tests/exe-snd-expos.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/child_type.hpp>
#include <beman/execution26/detail/write_env.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/basic_sender.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/connect_all_result.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/operation_state.hpp>
#include <beman/execution26/detail/basic_operation.hpp>
#include <beman/execution26/detail/connect_all.hpp>
#include <beman/execution26/detail/fwd_env.hpp>
#include <beman/execution26/detail/make_env.hpp>
#include <beman/execution26/detail/join_env.hpp>
#include <beman/execution26/detail/sched_attrs.hpp>
#include <beman/execution26/detail/sched_env.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/query_with_default.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/get_domain_late.hpp>
#include <beman/execution26/detail/default_impls.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/state_type.hpp>
#include <beman/execution26/detail/basic_state.hpp>
#include <beman/execution26/detail/indices_for.hpp>
#include <beman/execution26/detail/valid_specialization.hpp>
#include <beman/execution26/detail/env_type.hpp>
#include <beman/execution26/detail/basic_receiver.hpp>
#include <beman/execution26/detail/completion_tag.hpp>
#include <beman/execution26/detail/scheduler.hpp>
#include <beman/execution26/execution.hpp>
#include <beman/execution26/detail/tag_of_t.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    auto use(auto&&...) {}

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

    struct tag {
        static auto name() { return "expos test anonymous"; }
    };

    template <typename Receiver>
    struct operation_state
    {
        using operation_state_concept = test_std::operation_state_t;
        int* counter;
        operation_state(int* counter): counter(counter) {}
        operation_state(operation_state&&) = delete;
        auto start() & noexcept -> void { ++*counter; }
    };

    struct sender0
    {
        struct env {};
        using sender_concept = test_std::sender_t;
        using indices_for = ::std::index_sequence_for<>;
        tag t{};
        int* data{};
        template <typename Receiver>
        auto connect(Receiver&&) const noexcept { return operation_state<Receiver>(data); }
        auto get_env() const noexcept -> env { return {}; }
    };

    struct sender1
    {
        using sender_concept = test_std::sender_t;
        using indices_for = ::std::index_sequence_for<sender0>;
        tag t{};
        int* data{};
        sender0 c0{};
        template <typename Receiver>
        auto connect(Receiver&&) const noexcept { return operation_state<Receiver>(data); }
    };

    struct sender2
    {
        using sender_concept = test_std::sender_t;
        using indices_for = ::std::index_sequence_for<sender0, sender0>;
        tag t{};
        int* data{};
        sender0 c0{};
        sender0 c1{};
        template <typename Receiver>
        auto connect(Receiver&&) const noexcept { return operation_state<Receiver>(data); }
    };

    struct sender3
    {
        using sender_concept = test_std::sender_t;
        using indices_for = ::std::index_sequence_for<sender0, sender0, sender0>;
        tag t{};
        int* data{};
        sender0 c0{};
        sender0 c1{};
        sender0 c2{};
        template <typename Receiver>
        auto connect(Receiver&&) const noexcept { return operation_state<Receiver>(data); }
    };

    struct sender4
    {
        using sender_concept = test_std::sender_t;
        using indices_for = ::std::index_sequence_for<sender0, sender0, sender0, sender0>;
        tag t{};
        int* data{};
        sender0 c0{};
        sender0 c1{};
        sender0 c2{};
        sender0 c3{};
        template <typename Receiver>
        auto connect(Receiver&&) const noexcept { return operation_state<Receiver>(data); }
    };

    struct receiver
    {
        using receiver_concept = test_std::receiver_t;
        auto set_value(auto&&...) noexcept -> void {}
        auto set_error(auto&&) noexcept -> void {}
        auto set_stopped() noexcept -> void {}
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
        static_assert(test_std::detail::queryable<decltype(test_detail::fwd_env(e))>);
        test_fwd_env_helper<true, test_std::get_domain_t>();
        test_fwd_env_helper<false, non_forwardable_t>();
        assert(129 == test_detail::fwd_env(e).query(forwardable, 1, 3));
    }

    auto test_make_env() -> void
    {
        auto env{test_detail::make_env(custom_query<0>, custom_result{43})};
        auto const cenv{env};
        static_assert(test_std::detail::queryable<decltype(env)>);
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
        
        static_assert(test_std::detail::queryable<decltype(env)>);
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
        struct env<domain>
        {
            template <typename Tag>
            auto query(test_std::get_completion_scheduler_t<Tag> const&) const noexcept
                -> scheduler<domain>
            {
                return {};
            }
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

    auto test_get_domain_early() -> void
    {
        struct plain_sender
        {
            using sender_concept = test_std::sender_t;
        };
        static_assert(test_std::sender<plain_sender>);
        static_assert(std::same_as<
            test_std::default_domain,
            decltype(test_detail::get_domain_early(plain_sender{}))
        >);

        namespace cd = completion_domain;
        static_assert(test_std::sender<cd::sender<cd::domain>>);
        static_assert(std::same_as<
            cd::domain,
            decltype(test_detail::get_domain_early(cd::sender<cd::domain>{}))
        >);

        struct sender_with_domain
        {
            using sender_concept = test_std::sender_t;
            struct domain {};
            struct env
            {
                auto query(test_std::get_domain_t const&) const noexcept -> domain { return {}; }
            };
            auto get_env() const noexcept -> env { return {}; }
        };
        static_assert(test_std::sender<sender_with_domain>);
        static_assert(std::same_as<
            sender_with_domain::env,
            decltype(test_std::get_env(sender_with_domain{}))
        >);
        static_assert(std::same_as<
            sender_with_domain::domain,
            decltype(test_std::get_domain(sender_with_domain::env{}))
        >);
        static_assert(std::same_as<
            sender_with_domain::domain,
            decltype(test_detail::get_domain_early(sender_with_domain{}))
        >);
    }

    template <typename Expect>
    auto test_get_domain_late(auto sender, auto env) -> void
    {
        static_assert(test_std::sender<decltype(sender)>);
        static_assert(test_detail::queryable<decltype(env)>);
        static_assert(std::same_as<Expect,
            decltype(test_detail::get_domain_late(sender, env))>);
    }

    struct get_domain_late_scheduler
    {
        struct dom {};
        struct env
        {
            template <typename Tag>
            auto query(test_std::get_completion_scheduler_t<Tag> const&)
                const noexcept -> get_domain_late_scheduler { return {}; }
        };
        struct sender
        {
            using sender_concept = test_std::sender_t;
            auto get_env() const noexcept -> env { return {}; };
        };
        using scheduler_concept = test_std::scheduler_t;
        auto schedule() noexcept -> sender { return {}; }
        auto operator== (get_domain_late_scheduler const&) const -> bool = default;
        auto query(test_std::get_domain_t const&) const noexcept -> dom { return {}; }
    };

    struct get_domain_late_env
    {
        struct dom {};
        template <typename Tag>
        auto query(test_std::get_completion_scheduler_t<Tag> const&)
            const noexcept -> get_domain_late_scheduler { return {}; }
        auto query(test_std::get_domain_t const&) const noexcept -> dom { return {}; }
    };

    auto test_get_domain_late() -> void
    {
        struct no_domain_sender
        {
            using sender_concept = test_std::sender_t;
        };
        static_assert(test_std::sender<no_domain_sender>);
        test_get_domain_late<test_std::default_domain>(no_domain_sender{}, test_std::empty_env{});

        struct scheduler_env
        {
            auto query(test_std::get_scheduler_t const&) const noexcept
                -> get_domain_late_scheduler { return {}; }
        };
        static_assert(test_std::scheduler<get_domain_late_scheduler>);
        static_assert(std::same_as<get_domain_late_scheduler,
                      decltype(test_std::get_scheduler(scheduler_env{}))>);
        static_assert(std::same_as<get_domain_late_scheduler::dom,
                      decltype(test_std::get_domain(get_domain_late_scheduler{}))>);
        test_get_domain_late<get_domain_late_scheduler::dom>(no_domain_sender{}, scheduler_env{});

        struct domain_env
        {
            struct dom {};
            auto query(test_std::get_scheduler_t const&) const noexcept
                -> get_domain_late_scheduler { return {}; }
            auto query(test_std::get_domain_t) const noexcept -> dom { return {}; }
        };
        static_assert(std::same_as<domain_env::dom,
                      decltype(test_std::get_domain(domain_env{}))>);
        test_get_domain_late<domain_env::dom>(no_domain_sender{}, domain_env{});

        struct scheduler_sender
        {
            using sender_concept = test_std::sender_t;
            auto get_env() const noexcept -> get_domain_late_scheduler::env { return {}; }
        };
        static_assert(test_std::sender<scheduler_sender>);
        static_assert(std::same_as<get_domain_late_scheduler::env,
            decltype(test_std::get_env(scheduler_sender{}))>);
        static_assert(std::same_as<get_domain_late_scheduler::dom,
            decltype(test_detail::completion_domain(scheduler_sender{}))>);
        test_get_domain_late<get_domain_late_scheduler::dom>(scheduler_sender{}, test_std::empty_env{});

        struct env_sender
        {
            using sender_concept = test_std::sender_t;
            auto get_env() const noexcept -> get_domain_late_env { return {}; }
        };
        static_assert(test_std::sender<env_sender>);
        static_assert(std::same_as<get_domain_late_env::dom,
                      decltype(test_std::get_domain(get_domain_late_env{}))>);
        static_assert(std::same_as<get_domain_late_env, decltype(test_std::get_env(env_sender{}))>);
        test_get_domain_late<get_domain_late_env::dom>(env_sender{}, test_std::empty_env{});
    }

    auto test_default_impls_get_attrs() -> void
    {
        struct env { int value; };
        struct child1 { auto get_env() const noexcept { return env{1}; } };
        struct child2 { auto get_env() const noexcept { return env{2}; } };

        static_assert(noexcept(test_detail::default_impls::get_attrs(0, child1{})));
        static_assert(std::same_as<test_detail::fwd_env<env>,
            decltype(test_detail::default_impls::get_attrs(0, child1{}))>);
        //static_assert(std::same_as<test_std::empty_env,
        //    decltype(test_detail::default_impls::get_attrs(0, child1{}, child2{}))>);
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
        struct tag
        {
            static auto name() { return "test_default_impls_get_state"; }
        };
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
        struct tag
        {
            static auto name() { return "test_impls_for"; }
        };

        static_assert(std::derived_from<test_detail::impls_for<tag>,
                                        test_detail::default_impls>);
    }

    auto test_state_type() -> void
    {
        struct tag
        {
            static auto name() { return "test_state_type"; }
        };
        struct state {};
        struct sender
        {
            tag t;
            state s;
        };
        struct receiver {};

        static_assert(std::same_as<state, test_detail::state_type<sender, receiver>>);
    }

    auto test_basic_state() -> void
    {
        struct tag
        {
            static auto name() { return "test_basic_state"; }
        };
        struct data {};
        struct sender
        {
            tag t;
            data d;
        };
        struct receiver {};

        test_detail::basic_state<sender, receiver> state(sender{}, receiver{});
    }

    auto test_indices_for() -> void
    {
        using indices = ::std::make_index_sequence<5>;
        struct sender
        {
            using indices_for = indices;
        };

        static_assert(::std::same_as<indices, test_detail::indices_for<sender>>);
        static_assert(::std::same_as<indices, test_detail::indices_for<sender&>>);
        static_assert(::std::same_as<indices, test_detail::indices_for<sender&&>>);
        static_assert(::std::same_as<indices, test_detail::indices_for<sender const&>>);
        static_assert(::std::same_as<indices, test_detail::indices_for<sender const&&>>);
    }

    template <typename, typename> struct test_valid_specializaton_template {};

    auto test_valid_specialization() -> void
    {
        static_assert(test_detail::valid_specialization<test_valid_specializaton_template, int, int>);
        static_assert(not test_detail::valid_specialization<test_valid_specializaton_template, int>);
        static_assert(not test_detail::valid_specialization<test_valid_specializaton_template, int, int, int>);
    }

    auto test_env_type() -> void
    {
        using index = std::integral_constant<int, 0>;
        struct tag
        {
            static auto name() { return "test_env_type"; }
        };
        struct data {};
        struct env {};
        struct sender
        {
            tag t;
            data d;
        };
        struct sender_with_env
        {
            tag t;
            data d;
            auto get_env() const noexcept -> env { return {}; }
        };
        struct receiver
        {
        };
        struct receiver_with_env
        {
            auto get_env() const noexcept -> env { return {}; }
        };

        static_assert(std::same_as<test_detail::fwd_env<test_std::empty_env>,
                                   test_detail::env_type<index, sender, receiver>>);
        static_assert(std::same_as<test_detail::fwd_env<test_std::empty_env>,
                                   test_detail::env_type<index, sender_with_env, receiver>>);
        static_assert(std::same_as<test_detail::fwd_env<env>,
                                   test_detail::env_type<index, sender, receiver_with_env>>);
    }

    template <typename T>
    auto test_basic_receiver() -> void
    {
        using index = std::integral_constant<int, 0>;
        struct tag
        {
            static auto name() { return "test_basic_receiver"; }
        };
        struct data {};
        struct err
        {
            int value{};
            auto operator== (err const&) const -> bool = default;
        };
        struct sender
        {
            tag  t{};
            data d{};
        };
        struct receiver
        {
            T    value{};
            err  error{};
            bool stopped{};

            auto set_value(T val) noexcept { this->value = val; }
            auto set_error(err error) noexcept { this->error = error; }
            auto set_stopped() noexcept { this->stopped = true; }
        };
        struct unstoppable_receiver
        {
            T value;
        };
        using basic_receiver = test_detail::basic_receiver<sender, receiver, index>;
        static_assert(test_std::receiver<basic_receiver>);
        static_assert(std::same_as<tag, typename basic_receiver::tag_t>);
        static_assert(std::same_as<test_detail::state_type<sender, receiver>,
                                   typename basic_receiver::state_t>);
        assert(&basic_receiver::complete == &test_detail::default_impls::complete);

        {
            test_detail::basic_state<sender, receiver> op(sender{}, receiver{});
            basic_receiver br{&op};
            static_assert(not requires{ std::move(br).set_value(); });
            static_assert(not requires{ std::move(br).set_value(42, 1); });
            static_assert(requires{ std::move(br).set_value(42); });
            static_assert(noexcept(std::move(br).set_value(42)));
            assert(op.receiver.value == 0);
            std::move(br).set_value(42);
            assert(op.receiver.value == 42);
        }
        {
            test_detail::basic_state<sender, receiver> op(sender{}, receiver{});
            basic_receiver br{&op};
            static_assert(not requires{ std::move(br).set_error(); });
            static_assert(not requires{ std::move(br).set_error(0); });
            static_assert(requires{ std::move(br).set_error(err{42}); });
            static_assert(noexcept(std::move(br).set_error(err{42})));
            assert(op.receiver.error == err{});
            std::move(br).set_error(err{42});
            assert(op.receiver.error == err{42});
        }
        {
            test_detail::basic_state<sender, receiver> op(sender{}, receiver{});
            basic_receiver br{&op};
            static_assert(requires{ std::move(br).set_stopped(); });
            static_assert(noexcept(std::move(br).set_stopped()));
            assert(op.receiver.stopped == false);
            std::move(br).set_stopped();
            assert(op.receiver.stopped == true);
        }
        {
            test_detail::basic_state<sender, unstoppable_receiver> op(sender{}, unstoppable_receiver{});
            test_detail::basic_receiver<sender, unstoppable_receiver, index> br{&op};
            static_assert(not requires{ std::move(br).set_stopped(); });
        }
        //-dk:TODO test basic_receiver::get_env
    }

    auto test_completion_tag() -> void
    {
        struct no_completion {};

        static_assert(test_detail::completion_tag<test_std::set_error_t>);
        static_assert(test_detail::completion_tag<test_std::set_stopped_t>);
        static_assert(test_detail::completion_tag<test_std::set_value_t>);
        static_assert(not test_detail::completion_tag<int>);
        static_assert(not test_detail::completion_tag<no_completion>);
    }

    auto test_product_type() -> void
    {
        struct nm
        {
            int value{};
            nm() = default;
            nm(int value): value(value) {}
            nm(nm&&) = delete;
            auto operator== (nm const&) const -> bool = default;
        };
        auto p0{test_detail::product_type{}};
        static_assert(p0.size() == 0u);
        assert(p0 == p0);

        auto p1{test_detail::product_type{nm(1)}};
        static_assert(p1.size() == 1u);
        assert(p1.get<0>() == 1);

        auto p2{test_detail::product_type{nm(1), nm(2)}};
        static_assert(p2.size() == 2u);
        assert(p2.get<0>() == 1);
        assert(p2.get<1>() == 2);

        auto p3{test_detail::product_type{nm(1), nm(2), nm(3)}};
        static_assert(p3.size() == 3u);
        assert(p3.get<0>() == 1);
        assert(p3.get<1>() == 2);
        assert(p3.get<2>() == 3);

        auto p4{test_detail::product_type{nm(1), nm(2), nm(3), nm(4)}};
        static_assert(p4.size() == 4u);
        assert(p4.get<0>() == 1);
        assert(p4.get<1>() == 2);
        assert(p4.get<2>() == 3);
        assert(p4.get<3>() == 4);

        auto p5{test_detail::product_type{nm(1), nm(2), nm(3), nm(4), nm(5)}};
        static_assert(p5.size() == 5u);
        assert(p5.get<0>() == 1);
        assert(p5.get<1>() == 2);
        assert(p5.get<2>() == 3);
        assert(p5.get<3>() == 4);
        assert(p5.get<4>() == 5);
    }
    auto test_connect_all() -> void
    {
        static_assert(test_std::operation_state<operation_state<receiver>>);
        {
            sender0 s{};
            test_detail::basic_state state{std::move(s), receiver{}};
            auto product{test_detail::connect_all(&state, std::move(s), std::index_sequence<>{})};
            assert(product.size() == 0);
            use(product);
        }
        {
            sender0 const s{};
            test_detail::basic_state state{std::move(s), receiver{}};
            auto product{test_detail::connect_all(&state, std::move(s), std::index_sequence<>{})};
            assert(product.size() == 0);
            use(product);
        }
        {
            static_assert(requires{ sender1{}.connect(receiver{}); });
            static_assert(requires{ test_std::connect(sender1{}, receiver{}); });
            test_detail::basic_state state{sender1{}, receiver{}};
            auto product{test_detail::connect_all(&state, sender1{}, std::index_sequence<0>{})};
            assert(product.size() == 1);
            use(product);
        }
        {
            sender1 const s{};
            static_assert(requires{ s.connect(receiver{}); });
            static_assert(requires{ test_std::connect(s, receiver{}); });
            test_detail::basic_state state{std::move(s), receiver{}};
            auto product{test_detail::connect_all(&state, std::move(s), std::index_sequence<0>{})};
            assert(product.size() == 1);
            use(product);
        }
        {
            static_assert(requires{ sender2{}.connect(receiver{}); });
            static_assert(requires{ test_std::connect(sender2{}, receiver{}); });
            test_detail::basic_state state{sender2{}, receiver{}};
            auto product{test_detail::connect_all(&state, sender2{}, std::index_sequence<0, 1>{})};
            assert(product.size() == 2);
            use(product);
        }
        {
            sender2 const s{};
            static_assert(requires{ s.connect(receiver{}); });
            static_assert(requires{ test_std::connect(s, receiver{}); });
            test_detail::basic_state state{std::move(s), receiver{}};
            auto product{test_detail::connect_all(&state, std::move(s), std::index_sequence<0, 1>{})};
            assert(product.size() == 2);
            use(product);
        }
        {
            static_assert(requires{ sender3{}.connect(receiver{}); });
            static_assert(requires{ test_std::connect(sender3{}, receiver{}); });
            test_detail::basic_state state{sender3{}, receiver{}};
            auto product{test_detail::connect_all(&state, sender3{}, std::index_sequence<0, 1, 2>{})};
            assert(product.size() == 3);
            use(product);
        }
        {
            sender3 const s{};
            static_assert(requires{ s.connect(receiver{}); });
            static_assert(requires{ test_std::connect(s, receiver{}); });
            test_detail::basic_state state{std::move(s), receiver{}};
            auto product{test_detail::connect_all(&state, std::move(s), std::index_sequence<0, 1, 2>{})};
            assert(product.size() == 3);
            use(product);
        }
        {
            static_assert(requires{ sender4{}.connect(receiver{}); });
            static_assert(requires{ test_std::connect(sender4{}, receiver{}); });
            test_detail::basic_state state{sender4{}, receiver{}};
            auto product{test_detail::connect_all(&state, sender4{}, std::index_sequence<0, 1, 2, 3>{})};
            assert(product.size() == 4);
            use(product);
        }
        {
            sender4 const s{};
            static_assert(requires{ s.connect(receiver{}); });
            static_assert(requires{ test_std::connect(s, receiver{}); });
            test_detail::basic_state state{std::move(s), receiver{}};
            auto product{test_detail::connect_all(&state, std::move(s), std::index_sequence<0, 1, 2, 3>{})};
            assert(product.size() == 4);
            use(product);
        }

        //-dk: TODO test connect_all
    }

    auto test_connect_all_result() -> void
    {
        {
            test_detail::basic_state state{sender0{}, receiver{}};
            static_assert(std::same_as<
                decltype(test_detail::connect_all(&state, sender0{}, std::index_sequence<0, 1, 2, 3>{})),
                test_detail::connect_all_result<sender0, receiver>
            >);
        }
        {
            test_detail::basic_state state{sender1{}, receiver{}};
            static_assert(std::same_as<
                decltype(test_detail::connect_all(&state, sender1{}, std::index_sequence<0, 1, 2, 3>{})),
                test_detail::connect_all_result<sender1, receiver>
            >);
        }
        {
            test_detail::basic_state state{sender2{}, receiver{}};
            static_assert(std::same_as<
                decltype(test_detail::connect_all(&state, sender2{}, std::index_sequence<0, 1, 2, 3>{})),
                test_detail::connect_all_result<sender2, receiver>
            >);
        }
        {
            test_detail::basic_state state{sender3{}, receiver{}};
            static_assert(std::same_as<
                decltype(test_detail::connect_all(&state, sender3{}, std::index_sequence<0, 1, 2, 3>{})),
                test_detail::connect_all_result<sender3, receiver>
            >);
        }
        {
            test_detail::basic_state state{sender4{}, receiver{}};
            static_assert(std::same_as<
                decltype(test_detail::connect_all(&state, sender4{}, std::index_sequence<0, 1, 2, 3>{})),
                test_detail::connect_all_result<sender4, receiver>
            >);
        }
    }

    auto test_basic_operation() -> void
    {
        int data{};
        test_detail::basic_operation op{sender4{tag{}, &data,
            sender0{tag{}, &data},
            sender0{tag{}, &data},
            sender0{tag{}, &data},
            sender0{tag{}, &data}}, receiver{}};
        static_assert(test_std::operation_state<decltype(op)>);
        static_assert(requires{ typename decltype(op)::tag_t; });
        op.start();
        assert(data == 4);
    }

    auto test_completion_signatures_for() -> void
    {
        struct arg {};
        struct env {};
        struct bad_env {};
        struct sender
        {
            using sender_concept = test_std::sender_t;
            using empty_env_sigs = test_std::completion_signatures<
                    test_std::set_value_t(arg)
                >;
            using env_sigs = test_std::completion_signatures<
                    test_std::set_value_t(arg, arg)
                >;
            
            auto get_completion_signatures(test_std::empty_env const&)
                -> empty_env_sigs { return {}; }
            auto get_completion_signatures(env const&)
                -> env_sigs { return {}; }
        };

        static_assert(test_std::sender_in<sender, test_std::empty_env>);
        static_assert(test_std::sender_in<sender, env>);
        static_assert(not test_std::sender_in<sender, bad_env>);

        static_assert(std::same_as<
            test_detail::completion_signatures_for<sender, test_std::empty_env>,
            sender::empty_env_sigs
        >);
        static_assert(std::same_as<
            test_detail::completion_signatures_for<sender, env>,
            sender::env_sigs
        >);
        static_assert(not test_detail::valid_completion_signatures<
            test_detail::no_completion_signatures_defined_in_sender
        >);
        static_assert(std::same_as<
            test_detail::completion_signatures_for<sender, bad_env>,
            test_detail::no_completion_signatures_defined_in_sender
        >);
    }

    struct basic_sender_tag
    {
        template <typename>
        struct state
        {
            using operation_state_concept = test_std::operation_state_t;
            auto start() & noexcept -> void {}
        };
        struct sender
        {
            using sender_concept = test_std::sender_t;
            using completion_signatures = test_std::completion_signatures<>;
            template <test_std::receiver Receiver>
            auto connect(Receiver) noexcept -> state<Receiver> { return {}; }
        };
        auto transform_sender(auto&&...) noexcept { return sender{}; }
    };

    struct data {};
    struct tagged_sender
        : test_detail::product_type<basic_sender_tag, data, sender0>
    {
        using sender_concept = test_std::sender_t;
    };
}
namespace std
{
    template <>
    struct tuple_size<tagged_sender>
        : ::std::integral_constant<std::size_t, 3>
    {
    };
    template <::std::size_t I>
    struct tuple_element<I, tagged_sender>
    {
        using type = ::std::decay_t<decltype(::std::declval<tagged_sender>().template get<I>())>;
    };
}
namespace
{
    auto test_basic_sender() -> void
    {
        struct env {};

        {
            auto&&[a, b, c] = tagged_sender{basic_sender_tag{}, data{}, sender0{}};
            use(a);
            use(b);
            use(c);
            static_assert(std::same_as<basic_sender_tag, std::remove_cvref_t<decltype(a)>>);
        }

        static_assert(test_std::sender<basic_sender_tag::sender>);
        static_assert(test_std::sender_in<basic_sender_tag::sender>);
        static_assert(test_std::sender_in<basic_sender_tag::sender, env>);
        static_assert(test_std::operation_state<basic_sender_tag::state<receiver>>);
        static_assert(test_std::sender<tagged_sender>);
        static_assert(std::same_as<
            basic_sender_tag,
            test_std::tag_of_t<tagged_sender>
        >);
        static_assert(std::same_as<
            basic_sender_tag::sender,
            decltype(test_std::transform_sender(test_std::default_domain{}, tagged_sender{}, env{}))
        >);

        using basic_sender = test_detail::basic_sender<basic_sender_tag, data, sender0>;
        static_assert(test_std::sender<basic_sender>);

        basic_sender bs{basic_sender_tag{}, data{}, sender0 {}};
        basic_sender const& cbs{bs};
        use(cbs);

        auto&&[tag, data, children] = test_detail::get_sender_data(bs);
        use(tag, data, children);
        static_assert(std::same_as<basic_sender_tag, std::remove_cvref_t<decltype(tag)>>);

        static_assert(std::same_as<
            basic_sender_tag,
            test_std::tag_of_t<basic_sender>
        >);
        static_assert(std::same_as<
            basic_sender_tag::sender,
            decltype(test_std::transform_sender(test_std::default_domain{}, basic_sender{}, env{}))
        >);
        static_assert(test_std::sender_in<basic_sender>);
        static_assert(std::same_as<
            basic_sender_tag::sender::completion_signatures,
            test_detail::completion_signatures_for<basic_sender, env>
        >);
        static_assert(std::same_as<
            basic_sender_tag::sender::completion_signatures,
            test_detail::completion_signatures_for<basic_sender, env>
        >);
        
        auto ge{test_std::get_env(bs)};
        use(ge);
        static_assert(std::same_as<
            test_detail::fwd_env<sender0::env>,
            decltype(ge)
        >);

        auto op{test_std::connect(bs, receiver{})};
        use(op);
#if 0
        static_assert(std::same_as<
            basic_sender_tag::sender::completion_signatures,
            decltype(bs.get_completion_signatures(env{}))
        >);
        static_assert(std::same_as<
            basic_sender_tag::sender::completion_signatures,
            decltype(cbs.get_completion_signatures(env{}))
        >);
        static_assert(std::same_as<
            basic_sender_tag::sender::completion_signatures,
            decltype(basic_sender{ basic_sender_tag{}, data{}, sender0 {} }
                .get_completion_signatures(env{}))
        >);
#endif
        static_assert(std::same_as<
            std::index_sequence_for<sender0>,
            basic_sender::indices_for
        >);
    }

    template <typename T>
    auto test_make_sender() -> void
    {
        {
            auto sender{test_detail::make_sender(tag{}, {})};
            static_assert(test_std::sender<decltype(sender)>);
        }
        {
            auto sender{test_detail::make_sender(tag{}, int{17})};
            static_assert(test_std::sender<decltype(sender)>);
            static_assert(std::same_as<test_detail::basic_sender<tag, int>, decltype(sender)>);
        }
        {
            static_assert(not requires{ test_detail::make_sender(tag{}, int{17}, T()); });
        }
        {
            auto sender{test_detail::make_sender(tag{}, int{17}, sender0{})};
            static_assert(test_std::sender<decltype(sender)>);
            static_assert(std::same_as<
                test_detail::basic_sender<tag, int, sender0>,
                decltype(sender)
            >);
        }
    }

    template <typename>
    struct property
    {
        struct data
        {
            int value{};
            auto operator== (data const&) const -> bool = default;
        };
    };

    struct write_env_env
    {
        struct base {};
        int value{};
        auto query(property<base> const&) const -> property<base>::data
        { return {this->value}; }
    };
    struct write_env_added
    {
        int value{};
        struct added {};
        auto query(property<added> const&) const noexcept -> property<added>::data
        {
            return {this->value};
        }
    };

    struct write_env_receiver
    {
        using receiver_concept = test_std::receiver_t;

        bool* result{nullptr};

        auto get_env() const noexcept -> write_env_env { return {42}; }
        auto set_value(bool value) && noexcept -> void
        {
            *this->result = value;
        }
    };

    struct write_env_sender
    {
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
                test_std::set_value_t(bool)
            >;
        template <typename Receiver>
        struct state
        {
            using operation_state_concept = test_std::operation_state_t;
            std::remove_cvref_t<Receiver> receiver;

            auto start() & noexcept -> void
            {
                using base_property = property<write_env_env::base>;
                using added_property = property<write_env_added::added>;
                bool result{false};

                if constexpr (requires{
                    test_std::get_env(receiver).query(base_property{});
                    test_std::get_env(receiver).query(added_property{});
                })
                {
                    result =
                            (base_property::data{42}
                             == test_std::get_env(receiver).query(base_property{}))
                        &&  (added_property::data{43}
                             == test_std::get_env(receiver).query(added_property{}))
                        ;
                }

                test_std::set_value(::std::move(receiver), result);
            }
        };

        template <typename Receiver>
        auto connect(Receiver&& receiver) noexcept -> state<Receiver>
        {
            return { std::forward<Receiver>(receiver) };
        }
    };

    auto test_write_env() -> void
    {
        static_assert(test_std::sender<write_env_sender>);
        static_assert(test_std::receiver<write_env_receiver>);
        static_assert(test_detail::queryable<write_env_added>);
        auto plain_op(test_std::connect(write_env_sender{}, write_env_receiver{}));
        static_assert(std::same_as<
            write_env_env,
            decltype(test_std::get_env(write_env_receiver{}))
        >);
        static_assert(std::same_as<
            write_env_env,
            decltype(test_std::get_env(plain_op.receiver))
        >);
        using base_property = property<write_env_env::base>;
        assert(base_property::data{42}
               == test_std::get_env(plain_op.receiver).query(base_property{}));

        auto we_sender{test_detail::write_env(write_env_sender{},
                                              write_env_added{43})};

        static_assert(test_std::sender_in<write_env_sender>);
        static_assert(std::same_as<
            test_std::completion_signatures<test_std::set_value_t(bool)>,
            decltype(test_std::get_completion_signatures(write_env_sender{}, write_env_env{}))
        >);

        static_assert(std::same_as<
            test_detail::completion_signatures_for<decltype(we_sender), write_env_env>,
            test_std::completion_signatures<test_std::set_value_t(bool)>
        >);
        static_assert(std::same_as<
            test_detail::completion_signatures_for<decltype(we_sender), test_std::empty_env>,
            test_std::completion_signatures<test_std::set_value_t(bool)>
        >);
        static_assert(std::same_as<
            test_detail::completion_signatures_for<decltype(we_sender)&, test_std::empty_env>,
            test_std::completion_signatures<test_std::set_value_t(bool)>
        >);
        static_assert(test_std::sender_in<decltype(we_sender)>);
        static_assert(std::same_as<
            test_std::completion_signatures<test_std::set_value_t(bool)>,
            decltype(test_std::get_completion_signatures(we_sender, write_env_env{}))
        >);

        static_assert(test_std::sender<decltype(we_sender)>);
        static_assert(std::same_as<
            test_detail::write_env_t,
            test_std::tag_of_t<decltype(we_sender)>
        >);

        bool has_both_properties{false};
        assert(not has_both_properties);
        auto we_op{test_std::connect(we_sender, write_env_receiver{&has_both_properties})};
        we_op.start();
        assert(has_both_properties);
        use(we_op);
    }

    template <typename... T>
    struct child_sender
        : test_detail::product_type<T...>
    {
    };
    auto test_child_type() -> void
    {
        static_assert(std::same_as<
            int,
            test_detail::child_type<child_sender<bool, char, int, double>>
        >);
        static_assert(std::same_as<
            int&,
            test_detail::child_type<child_sender<bool, char, int, double>>&
        >);
        static_assert(std::same_as<
            int const&,
            test_detail::child_type<child_sender<bool, char, int, double>> const&
        >);
        static_assert(std::same_as<
            int,
            test_detail::child_type<child_sender<bool, char, int, double>, 0>
        >);
        static_assert(std::same_as<
            double,
            test_detail::child_type<child_sender<bool, char, int, double>, 1>
        >);
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
    test_get_domain_early();
    test_get_domain_late();
    test_default_impls();
    test_impls_for();
    test_state_type();
    test_basic_state();
    test_indices_for();
    test_valid_specialization();
    test_env_type();
    test_basic_receiver<int>();
    test_completion_tag();
    test_product_type();
    test_connect_all();
    test_connect_all_result();
    test_basic_operation();
    test_completion_signatures_for();
    test_basic_sender();
    test_make_sender<int>();
    test_write_env();
    test_child_type();
}