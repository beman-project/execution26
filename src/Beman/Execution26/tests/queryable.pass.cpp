// src/Beman/Execution26/tests/queryable.pass.cpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/execution.hpp>
#include "test/execution.hpp"

// ----------------------------------------------------------------------------

namespace
{
    template <typename T>
    concept has_foo
        = test_std::Detail::queryable<T>
        && requires(T t) { t.foo; }
        ;

    class non_destructible
    {
        ~non_destructible() = default;
    };

    template <typename T>
        requires std::destructible<T>
    auto f(T const&) -> int
    {
        return 0;
    }

    template <typename T>
        requires has_foo<T>
    auto f(T const&) -> int
    {
        return 1;
    }

    struct bar { int foo; };
}

auto main() -> int
{
    static_assert(test_std::Detail::queryable<int>);
    static_assert(not test_std::Detail::queryable<non_destructible>);

    assert(f(0) == 0);
    assert(f(bar{}) == 1);
}
