// src/Beman/Execution26/tests/functional-syn.pass.cpp                -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#include <Beman/Execution26/functional.hpp>
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
        static_assert(test_ex::Detail::callable<auto()->void>);
        static_assert(not test_ex::Detail::callable<auto()->void, int>);
        static_assert(test_ex::Detail::callable<auto(int)->void, short>);

        static_assert(not test_ex::Detail::callable<auto()->non_destructible>);

        static_assert(test_ex::Detail::callable<auto() noexcept ->void>);
        static_assert(not test_ex::Detail::callable<auto() noexcept ->void, int>);
        static_assert(test_ex::Detail::callable<auto(int) noexcept ->void, short>);

        static_assert(not test_ex::Detail::callable<auto() noexcept ->non_destructible>);

        static_assert(test_ex::Detail::callable<fun<true>>);
        static_assert(test_ex::Detail::callable<fun<true> const>);
        static_assert(test_ex::Detail::callable<fun<true>, int>);
        static_assert(not test_ex::Detail::callable<fun<true> const, int>);

        static_assert(test_ex::Detail::callable<fun<true>, non_copyable>);
        static_assert(test_ex::Detail::callable<fun<true>, arg>);
        static_assert(test_ex::Detail::callable<fun<true>, throws>);

        static_assert(test_ex::Detail::callable<fun<false>>);
        static_assert(test_ex::Detail::callable<fun<false> const>);
        static_assert(test_ex::Detail::callable<fun<false>, int>);
        static_assert(not test_ex::Detail::callable<fun<false> const, int>);

        static_assert(test_ex::Detail::callable<fun<false>, non_copyable>);
        static_assert(test_ex::Detail::callable<fun<false>, arg>);
        static_assert(test_ex::Detail::callable<fun<false>, throws>);
    }

    auto test_nothrow_callable() -> void
    {
        static_assert(not test_ex::Detail::nothrow_callable<auto()->void>);
        static_assert(not test_ex::Detail::nothrow_callable<auto()->void, int>);
        static_assert(not test_ex::Detail::nothrow_callable<auto(int)->void, short>);

        static_assert(not test_ex::Detail::nothrow_callable<auto()->non_destructible>);

        static_assert(test_ex::Detail::nothrow_callable<auto() noexcept ->void>);
        static_assert(not test_ex::Detail::nothrow_callable<auto() noexcept ->void, int>);
        static_assert(test_ex::Detail::nothrow_callable<auto(int) noexcept ->void, short>);

        static_assert(not test_ex::Detail::nothrow_callable<auto() noexcept ->non_destructible>);

        static_assert(test_ex::Detail::nothrow_callable<fun<true>>);
        static_assert(test_ex::Detail::nothrow_callable<fun<true> const>);
        static_assert(test_ex::Detail::nothrow_callable<fun<true>, int>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<true> const, int>);

        static_assert(test_ex::Detail::nothrow_callable<fun<true>, non_copyable>);
        static_assert(test_ex::Detail::nothrow_callable<fun<true>, arg>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<true>, throws>);

        static_assert(not test_ex::Detail::nothrow_callable<fun<false>>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<false> const>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<false>, int>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<false> const, int>);

        static_assert(not test_ex::Detail::nothrow_callable<fun<false>, non_copyable>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<false>, arg>);
        static_assert(not test_ex::Detail::nothrow_callable<fun<false>, throws>);
    }

    auto test_call_result_t() -> void
    {
        static_assert(std::same_as<void, test_ex::Detail::call_result_t<auto()->void>>);
        static_assert(std::same_as<int, test_ex::Detail::call_result_t<auto()->int>>);
        static_assert(std::same_as<int&, test_ex::Detail::call_result_t<auto()->int&>>);
        static_assert(std::same_as<int const&, test_ex::Detail::call_result_t<auto()->int const&>>);

        static_assert(std::same_as<bool, test_ex::Detail::call_result_t<overloaded>>);
        static_assert(std::same_as<int, test_ex::Detail::call_result_t<overloaded const>>);
        static_assert(std::same_as<double, test_ex::Detail::call_result_t<overloaded&>>);

        static_assert(std::same_as<bool, test_ex::Detail::call_result_t<overloaded, int>>);
        static_assert(std::same_as<int, test_ex::Detail::call_result_t<overloaded, int const>>);
        static_assert(std::same_as<double, test_ex::Detail::call_result_t<overloaded, int&>>);
    }

    struct test_tag {};
    constexpr test_tag test_object{};

    auto test_decayed_typeof() -> void
    {
        static_assert(std::same_as<test_tag, test_ex::Detail::decayed_typeof<test_object>>);
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