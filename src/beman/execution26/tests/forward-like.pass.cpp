// src/beman/execution26/tests/forward-like.pass.cpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/forward_like.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct object
    {
        int value{};
    };

    template <typename Expect, typename T>
    auto test_forward_like(T&& outer)
    {
        static_assert(std::same_as<
            Expect,
            decltype(test_detail::own_forward_like<T>(outer.value))
        >);
        static_assert(std::same_as<
            decltype(test_detail::forward_like<T>(outer.value)),
            decltype(test_detail::own_forward_like<T>(outer.value))
        >);
    }
}

TEST(forward_like)
{
    object       o{};
    object const co{};
    test_forward_like<int&>(o);
    test_forward_like<int&&>(std::move(o));
    test_forward_like<int const&>(co);
    test_forward_like<int const&&>(std::move(co));
}
