// src/beman/execution26/tests/exec-general.test.cpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>
#include <exception>
#include <system_error>

// ----------------------------------------------------------------------------

namespace
{
    struct error
    {
        int value;
    };

    struct non_movable
    {
        non_movable(non_movable&&) = delete;
    };
    struct non_copyable
    {
        non_copyable(non_copyable&&) = default;
        non_copyable(non_copyable const&) = delete;
    };

    auto test_movable_value() -> void
    {
        static_assert(test_detail::movable_value<int>);
        static_assert(test_detail::movable_value<int&>);
        static_assert(test_detail::movable_value<int const&>);

        static_assert(not test_detail::movable_value<non_movable>);
        static_assert(not test_detail::movable_value<non_copyable const>);
        static_assert(not test_detail::movable_value<int[1]>);
        static_assert(not test_detail::movable_value<int(&)[1]>);
    }

    auto test_matching_sig() -> void
    {
        static_assert(test_detail::matching_sig<int(), int()>);
        static_assert(not test_detail::matching_sig<bool(), int()>);

        static_assert(test_detail::matching_sig<int(int), int(int)>);
        static_assert(test_detail::matching_sig<int(int&&), int(int)>);

        static_assert(test_detail::matching_sig<int(int&&, bool&, double), int(int, bool&, double&&)>);
    }

    auto test_as_except_ptr() -> void
    {
        std::exception_ptr       ep(std::make_exception_ptr(42));
        std::exception_ptr       mep(ep);
        std::exception_ptr const cep(std::make_exception_ptr(42));
        static_assert(std::same_as<std::exception_ptr&, decltype(test_detail::as_except_ptr(ep))>);
        static_assert(std::same_as<std::exception_ptr&&, decltype(test_detail::as_except_ptr(std::move(ep)))>);
        static_assert(std::same_as<std::exception_ptr const&, decltype(test_detail::as_except_ptr(cep))>);

        ASSERT(ep == test_detail::as_except_ptr(ep));
        ASSERT(ep == mep);
        ASSERT(ep == test_detail::as_except_ptr(std::move(mep)));
        ASSERT(cep == test_detail::as_except_ptr(cep));
        //-dk:TODO set up death test: test_detail::as_except_ptr(::std::exception_ptr());

        static_assert(std::same_as<std::exception_ptr, decltype(test_detail::as_except_ptr(error{17}))>);
        try
        {
            std::rethrow_exception(test_detail::as_except_ptr(error{17}));
        }
        catch(error& e)
        {
            ASSERT(e.value == 17);
        }
        catch(...)
        {
            ASSERT(nullptr == "wrong exception type produced by as_except_ptr for random error");
        }
        
        std::error_code errc(17, ::std::system_category());
        static_assert(std::same_as<std::exception_ptr, decltype(test_detail::as_except_ptr(errc))>);
        try
        {
            std::rethrow_exception(test_detail::as_except_ptr(errc));
        }
        catch(std::system_error& e)
        {
            ASSERT(e.code() == errc);
        }
        catch (...)
        {
            ASSERT(nullptr == "wrong exception type produced by as_except_ptr for error code");
        }
    }
}

// ----------------------------------------------------------------------------

TEST(exec_general)
{
    // Tests for [exec.general]
    // p1 doesn't contain anything testable
    // p2 doesn't contain anything testable
    // p3 doesn't contain anything testable
    // p4.1 needs to be tested by uses of MANDATE-NOTHROW: 13 places
    test_movable_value(); // p4.2
    test_matching_sig(); // p4.3
    test_as_except_ptr(); // p4.4

}
