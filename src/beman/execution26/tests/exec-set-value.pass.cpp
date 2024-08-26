// src/beman/execution26/tests/exec.set.value.pass.cpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace
{
    template <int> struct arg {};
    struct throws
    {
        throws() = default;
        throws(throws const&) {}
    };
    struct receiver
    {
        template <typename ...T>
        auto set_value(T&&...) noexcept -> void {}
        auto set_value(throws) noexcept -> void {}
        auto set_value(int i, bool b, double d) noexcept
        {
            assert(i == 42);
            assert(b == true);
            assert(d == 123.75);
        }
    };

    template <typename R, typename... Args>
    auto test_callable(Args&&... args) -> void
    {
        // can be called with non-const rvalue receiver
        static_assert(requires{ test_std::set_value(std::declval<R>(), args...); });
        static_assert(requires{ test_std::set_value(std::declval<R&&>(), args...); });

        // cannot be called with const or lvalue receiver
        static_assert(not requires{ test_std::set_value(std::declval<R const>(), args...); });
        static_assert(not requires{ test_std::set_value(std::declval<R const&&>(), args...); });
        static_assert(not requires{ test_std::set_value(std::declval<R&>(), args...); });
        static_assert(not requires{ test_std::set_value(std::declval<R const&>(), args...); });
        static_assert(not requires{ test_std::set_value(std::declval<R volatile&>(), args...); });
        static_assert(not requires{ test_std::set_value(std::declval<R const volatile&>(), args...); });
    }

    template <typename R>
    auto test_noexcept()
    {
        static_assert(requires{ test_std::set_value(std::declval<R>()); });
        static_assert(requires{ test_std::set_value(std::declval<R>(), arg<0>()); });
        static_assert(not requires{ test_std::set_value(std::declval<R>(), throws()); });
    }
}

auto main() -> int
{
    static_assert(std::semiregular<test_std::set_value_t>);
    static_assert(std::same_as<test_std::set_value_t const, decltype(test_std::set_value)>);

    test_callable<receiver>();
    test_callable<receiver>(arg<0>());
    test_callable<receiver>(arg<0>(), arg<1>());
    test_callable<receiver>(arg<0>(), arg<1>(), arg<2>());

    test_std::set_value(receiver{}, 1);
    test_std::set_value(receiver{}, arg<0>(), arg<1>(), arg<2>(), arg<3>(), arg<4>());
    test_std::set_value(receiver{}, 42, true, 123.75);

    test_noexcept<receiver>();
}

