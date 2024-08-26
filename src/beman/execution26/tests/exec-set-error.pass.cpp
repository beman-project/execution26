// src/beman/execution26/tests/exec-set-error.pass.cpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include "test/execution.hpp"
#include <concepts>
#include <utility>

// ----------------------------------------------------------------------------

namespace
{
    struct arg { int value; };
    struct arg_throwing {};

    struct throws
    {
        throws() = default;
        throws(throws const&) {}
    };

    struct receiver
    {
        template <typename Error>
        auto set_error(Error&&) noexcept -> void {}
        auto set_error(throws) noexcept -> void {}
        auto set_error(arg a) noexcept -> void
        {
            assert(a.value == 43);
        }
        auto set_error(arg_throwing) -> void {}
    };

    template <typename R>
    void test_callable()
    {
        static_assert(requires{ test_std::set_error(std::declval<R>(), 42); });
        static_assert(requires{ test_std::set_error(std::declval<R&&>(), 42); });

        static_assert(not requires{ test_std::set_error(std::declval<R const>(), 42); });
        static_assert(not requires{ test_std::set_error(std::declval<R const&&>(), 42); });
        static_assert(not requires{ test_std::set_error(std::declval<R&>(), 42); });
        static_assert(not requires{ test_std::set_error(std::declval<R const&>(), 42); });
        static_assert(not requires{ test_std::set_error(std::declval<R volatile&>(), 42); });
        static_assert(not requires{ test_std::set_error(std::declval<R const volatile&>(), 42); });
    }

    template <typename R>
    auto test_noexcept()
    {
        static_assert(requires{ test_std::set_error(std::declval<R>(), arg()); });
        static_assert(not requires{ test_std::set_error(std::declval<R>(), throws()); });
        static_assert(not requires{ test_std::set_error(std::declval<R>(), arg_throwing()); });
    }
}

auto main() -> int
{
    static_assert(std::semiregular<test_std::set_error_t>);
    static_assert(std::same_as<test_std::set_error_t const, decltype(test_std::set_error)>);

    test_callable<receiver>();

    test_std::set_error(receiver{}, 42);
    test_std::set_error(receiver{}, arg{43});

    test_noexcept<receiver>();
}
