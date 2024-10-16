// src/beman/execution26/tests/exec-when-all.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/when_all.hpp>
#include <beman/execution26/detail/when_all_with_variant.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>
#include <utility>

// ----------------------------------------------------------------------------

namespace
{
    template <typename...C>
    struct multi_sender
    {
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<C...>;
    };

    template <int> struct domain {};

    template <typename D>
    struct domain_sender
    {
        struct env
        {
            auto query(test_std::get_domain_t) const noexcept -> D { return {}; }
        };
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<test_std::set_value_t()>;
        auto get_env() const noexcept -> env { return {}; }
    };

    template <bool Expect, typename... S>
    auto test_when_all_available(S&&... s)
    {
        static_assert(Expect == requires{ test_std::when_all(std::forward<S>(s)...); });
    }

    auto test_when_all() -> void
    {
        test_when_all_available<false>();
        test_when_all_available<true>(test_std::just());
        test_when_all_available<true>(test_std::just(0), test_std::just('a', true), test_std::just(3.5));
        test_when_all_available<false>(test_std::just(), test_std::just(), int());
        test_when_all_available<true>(multi_sender<test_std::set_value_t(), test_std::set_stopped_t()>());
        test_when_all_available<true>(multi_sender<test_std::set_value_t(), test_std::set_error_t(int)>());
        test_when_all_available<false>(multi_sender<test_std::set_value_t(), test_std::set_value_t(int)>());
        test_when_all_available<false>(multi_sender<test_std::set_error_t(int)>());
        test_when_all_available<true>(domain_sender<domain<0>>(), domain_sender<domain<0>>());
        test_when_all_available<false>(domain_sender<domain<0>>(), domain_sender<domain<1>>());

        auto s{test_std::when_all(test_std::just(), test_std::just(0, true), test_std::just(0.0))};
        static_assert(test_std::sender<decltype(s)>);
        test::check_type<test_std::completion_signatures<
            test_std::set_value_t(int, bool, double)
        >>(
            test_std::get_completion_signatures(s, test_std::empty_env{})
        );
        test_std::sync_wait(std::move(s));
    }
}

// ----------------------------------------------------------------------------

auto main() -> int
{
    static_assert(std::same_as<test_std::when_all_t const, decltype(test_std::when_all)>);
    static_assert(std::same_as<test_std::when_all_with_variant_t const, decltype(test_std::when_all_with_variant)>);

    test_when_all();
}