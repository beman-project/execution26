// src/beman/execution26/tests/exec-then.pass.cpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/then.hpp>

#include <beman/execution26/detail/completion_signatures_of_t.hpp>
#include <beman/execution26/detail/connect.hpp>
#include <beman/execution26/detail/just.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_in.hpp>
#include <beman/execution26/detail/start.hpp>
#include <beman/execution26/detail/sync_wait.hpp>
#include <concepts>
#include <test/execution.hpp>

// ----------------------------------------------------------------------------

namespace
{
    struct error { int value{}; };
    struct non_sender {};

    struct receiver
    {
        using receiver_concept = test_std::receiver_t;

        auto set_error(auto&&) && noexcept -> void {}
        auto set_stopped() && noexcept -> void {}
        auto set_value(auto&&...) && noexcept -> void {}
    };

    template <typename... T>
    struct sender
    {
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<T...>;
    };

    template <bool Expect>
    auto test_has(auto cpo, auto in_sender, auto fun) -> void
    {
        static_assert(test_std::receiver<receiver>);
        static_assert(Expect == requires{ { cpo(in_sender, fun) } -> test_std::sender; });
        if constexpr (Expect)
        {
            static_assert(requires{ cpo(fun); });
            static_assert(requires{ in_sender | cpo(fun); });
            static_assert(requires{ { in_sender | cpo(fun) } -> test_std::sender; });
            static_assert(requires{ { in_sender | cpo(fun) | cpo([](auto&&...){}) } -> test_std::sender; });
            auto sender{cpo(in_sender, fun)};
            auto op{test_std::connect(::std::move(sender), receiver{})};
            test_std::start(op);

            auto const csender{cpo(in_sender, fun)};
            auto cop{test_std::connect(std::move(csender), receiver{})};
            test_std::start(cop);
        }
    }

    template <typename... Completions>
    auto test_then_type(auto sender)
    {
        static_assert(std::same_as<
            test_std::completion_signatures<Completions...>,
            test_std::completion_signatures_of_t<decltype(sender), test_std::empty_env>
        >);
    }

    auto test_then_type() -> void
    {
        test_then_type< test_std::set_value_t()>(test_std::just(0) | test_std::then([](auto)noexcept{}));
        test_then_type< test_std::set_value_t(int)>(test_std::just() | test_std::then([]()noexcept{ return 0; }));
        test_then_type< test_std::set_error_t(int)>(test_std::just_error(0) | test_std::then([]()noexcept{ return 0; }));
        test_then_type< test_std::set_stopped_t()>(test_std::just_stopped() | test_std::then([]()noexcept{ return 0; }));

        test_then_type< test_std::set_value_t()>(test_std::just() | test_std::upon_error([]()noexcept{ return 0; }));
        test_then_type< test_std::set_value_t(int)>(test_std::just_error(error{})
            | test_std::upon_error([](error)noexcept{ return 0; }));
        test_then_type< test_std::set_stopped_t()>(test_std::just_stopped() | test_std::upon_error([]()noexcept{}));

        test_then_type< test_std::set_value_t(int)>(test_std::just(0) | test_std::upon_stopped([]()noexcept{}));
        test_then_type< test_std::set_error_t(int)>(test_std::just_error(0) | test_std::upon_stopped([]()noexcept{}));
        test_then_type< test_std::set_value_t()>(test_std::just_stopped() | test_std::upon_stopped([]()noexcept{}));
    }

    auto test_then_multi_type() -> void
    {
        static_assert(test_std::sender<sender<>>);
        static_assert(test_std::sender_in<sender<>>);

        test_then_type<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >(sender<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >());
        test_then_type<
                test_std::set_value_t(bool),
                test_std::set_error_t(error),
                test_std::set_stopped_t(),
                test_std::set_error_t(::std::exception_ptr)
            >(sender<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >() | test_std::then([](auto&&...){ return true; }));
        test_then_type<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_value_t(bool),
                test_std::set_stopped_t(),
                test_std::set_error_t(::std::exception_ptr)
            >(sender<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >() | test_std::upon_error([](error){ return true; }));
        test_then_type<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_stopped_t(),
                test_std::set_error_t(::std::exception_ptr)
            >(sender<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >() | test_std::upon_error([](error){}));
        test_then_type<
                test_std::set_value_t(int),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_value_t()
            >(sender<
                test_std::set_value_t(int),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >() | test_std::upon_stopped([]()noexcept{}));
        test_then_type<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error)
            >(sender<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >() | test_std::upon_stopped([]()noexcept{}));
        test_then_type<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_error_t(::std::exception_ptr)
            >(sender<
                test_std::set_value_t(),
                test_std::set_value_t(int, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >() | test_std::upon_stopped([]{}));
    }

    auto test_then_value() -> void
    {
        assert(std::tuple{17} == *test_std::sync_wait(
            test_std::just(5, 12) | test_std::then([](int a, int b){ return a + b; })
        ));
        assert(std::tuple{17} == test_std::sync_wait(
            test_std::just_error(17) | test_std::upon_error([](int a){ return a; })
        ));
        assert(std::tuple{17} == test_std::sync_wait(
            test_std::just_stopped() | test_std::upon_stopped([](){ return 17; })
        ));
    }
}

auto main() -> int
{
    static_assert(std::same_as<test_std::then_t const, decltype(test_std::then)>);
    static_assert(std::same_as<test_std::upon_error_t const, decltype(test_std::upon_error)>);
    static_assert(std::same_as<test_std::upon_stopped_t const, decltype(test_std::upon_stopped)>);

    test_has<false>(test_std::then, non_sender{}, []{});
    test_has<true>(test_std::then, test_std::just(), []{});
    test_has<true>(test_std::then, test_std::just_error(error{3}), []{});
    test_has<true>(test_std::then, test_std::just_stopped(), []{});

    test_then_type();
    test_then_multi_type();
    test_then_value();
}
