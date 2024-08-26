// src/beman/execution26/tests/functional-syn.pass.cpp                -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#include <beman/execution26/functional.hpp>
#include "test/execution.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    class non_destructible { ~non_destructible(); };
    struct non_copyable { non_copyable(non_copyable&&) = delete; };
    struct arg {};
    struct throws { throws(throws const&) {} };

    template <bool Noexcept>
    struct fun
    {
        auto operator()() const noexcept(Noexcept) -> void {}
        auto operator()(int) noexcept(Noexcept) -> void {}
        auto operator()(non_copyable&&) noexcept(Noexcept) -> void {}
        auto operator()(arg) && noexcept(Noexcept) -> void {}
        auto operator()(throws) noexcept(Noexcept) -> void {}
    };

    struct overloaded
    {
        auto operator()() && -> bool { return {}; };
        auto operator()() const& -> int { return {}; };
        auto operator()() & -> double { return {}; };
        auto operator()(int&&) -> bool { return {}; };
        auto operator()(int const&) -> int { return {}; };
        auto operator()(int&) -> double { return {}; };
    };

    auto test_callable() -> void
    {
        static_assert(test_std::detail::callable<auto()->void>);
        static_assert(not test_std::detail::callable<auto()->void, int>);
        static_assert(test_std::detail::callable<auto(int)->void, short>);

        static_assert(not test_std::detail::callable<auto()->non_destructible>);

        static_assert(test_std::detail::callable<auto() noexcept ->void>);
        static_assert(not test_std::detail::callable<auto() noexcept ->void, int>);
        static_assert(test_std::detail::callable<auto(int) noexcept ->void, short>);

        static_assert(not test_std::detail::callable<auto() noexcept ->non_destructible>);

        static_assert(test_std::detail::callable<fun<true>>);
        static_assert(test_std::detail::callable<fun<true> const>);
        static_assert(test_std::detail::callable<fun<true>, int>);
        static_assert(not test_std::detail::callable<fun<true> const, int>);

        static_assert(test_std::detail::callable<fun<true>, non_copyable>);
        static_assert(test_std::detail::callable<fun<true>, arg>);
        static_assert(test_std::detail::callable<fun<true>, throws>);

        static_assert(test_std::detail::callable<fun<false>>);
        static_assert(test_std::detail::callable<fun<false> const>);
        static_assert(test_std::detail::callable<fun<false>, int>);
        static_assert(not test_std::detail::callable<fun<false> const, int>);

        static_assert(test_std::detail::callable<fun<false>, non_copyable>);
        static_assert(test_std::detail::callable<fun<false>, arg>);
        static_assert(test_std::detail::callable<fun<false>, throws>);
    }

    auto test_nothrow_callable() -> void
    {
        static_assert(not test_std::detail::nothrow_callable<auto()->void>);
        static_assert(not test_std::detail::nothrow_callable<auto()->void, int>);
        static_assert(not test_std::detail::nothrow_callable<auto(int)->void, short>);

        static_assert(not test_std::detail::nothrow_callable<auto()->non_destructible>);

        static_assert(test_std::detail::nothrow_callable<auto() noexcept ->void>);
        static_assert(not test_std::detail::nothrow_callable<auto() noexcept ->void, int>);
        static_assert(test_std::detail::nothrow_callable<auto(int) noexcept ->void, short>);

        static_assert(not test_std::detail::nothrow_callable<auto() noexcept ->non_destructible>);

        static_assert(test_std::detail::nothrow_callable<fun<true>>);
        static_assert(test_std::detail::nothrow_callable<fun<true> const>);
        static_assert(test_std::detail::nothrow_callable<fun<true>, int>);
        static_assert(not test_std::detail::nothrow_callable<fun<true> const, int>);

        static_assert(test_std::detail::nothrow_callable<fun<true>, non_copyable>);
        static_assert(test_std::detail::nothrow_callable<fun<true>, arg>);
        static_assert(not test_std::detail::nothrow_callable<fun<true>, throws>);

        static_assert(not test_std::detail::nothrow_callable<fun<false>>);
        static_assert(not test_std::detail::nothrow_callable<fun<false> const>);
        static_assert(not test_std::detail::nothrow_callable<fun<false>, int>);
        static_assert(not test_std::detail::nothrow_callable<fun<false> const, int>);

        static_assert(not test_std::detail::nothrow_callable<fun<false>, non_copyable>);
        static_assert(not test_std::detail::nothrow_callable<fun<false>, arg>);
        static_assert(not test_std::detail::nothrow_callable<fun<false>, throws>);
    }

    auto test_call_result_t() -> void
    {
        static_assert(std::same_as<void, test_std::detail::call_result_t<auto()->void>>);
        static_assert(std::same_as<int, test_std::detail::call_result_t<auto()->int>>);
        static_assert(std::same_as<int&, test_std::detail::call_result_t<auto()->int&>>);
        static_assert(std::same_as<int const&, test_std::detail::call_result_t<auto()->int const&>>);

        static_assert(std::same_as<bool, test_std::detail::call_result_t<overloaded>>);
        static_assert(std::same_as<int, test_std::detail::call_result_t<overloaded const>>);
        static_assert(std::same_as<double, test_std::detail::call_result_t<overloaded&>>);

        static_assert(std::same_as<bool, test_std::detail::call_result_t<overloaded, int>>);
        static_assert(std::same_as<int, test_std::detail::call_result_t<overloaded, int const>>);
        static_assert(std::same_as<double, test_std::detail::call_result_t<overloaded, int&>>);
    }

    struct test_tag {};
    constexpr test_tag test_object{};

    auto test_decayed_typeof() -> void
    {
        static_assert(std::same_as<test_tag, test_std::detail::decayed_typeof<test_object>>);
    }
}

// ----------------------------------------------------------------------------

auto main() -> int
{
    test_callable();
    test_nothrow_callable();
    test_call_result_t();
    test_decayed_typeof();
}
