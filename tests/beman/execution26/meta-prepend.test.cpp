// src/beman/execution26/tests/meta-prepend.test.cpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/meta_prepend.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    template <typename...> struct type_list {};
}

TEST(meta_prepend)
{
    static_assert(std::same_as<
        type_list<bool>,
        test_detail::meta::prepend<bool, type_list<>>
        >);
    static_assert(std::same_as<
        type_list<bool, char>,
        test_detail::meta::prepend<bool, type_list<char>>
        >);
    static_assert(std::same_as<
        type_list<bool, char, double>,
        test_detail::meta::prepend<bool, type_list<char, double>>
        >);
    static_assert(std::same_as<
        type_list<bool, char, double, int>,
        test_detail::meta::prepend<bool, type_list<char, double, int>>
        >);
}
