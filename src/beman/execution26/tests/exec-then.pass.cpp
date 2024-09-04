// src/beman/execution26/tests/exec-then.pass.cpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/then.hpp>
#include <beman/execution26/detail/just.hpp>
#include <beman/execution26/detail/connect.hpp>
#include <beman/execution26/detail/start.hpp>
#include <test/execution.hpp>
#include <concepts>

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
            test::use(sender);
            auto op{test_std::connect(sender, receiver{})};
            test::use(op);
            test_std::start(op);
        }
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
}