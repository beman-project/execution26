// tests/beman/execution26/stopsource-finite-general.test.cpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include "test/execution.hpp"
#include <concepts>
#include <type_traits>

TEST(stopsource_finite_gernal) {
    static_assert(test_detail::stoppable_source<test_std::finite_inplace_stop_source<1>>);
    static_assert(test_detail::stoppable_source<test_std::finite_inplace_stop_source<2>>);
    static_assert(test_detail::stoppable_source<test_std::finite_inplace_stop_source<17>>);

    static_assert(not std::move_constructible<test_std::finite_inplace_stop_source<1>>);
    static_assert(not std::copy_constructible<test_std::finite_inplace_stop_source<1>>);
    static_assert(not std::is_move_assignable_v<test_std::finite_inplace_stop_source<1>>);
    static_assert(not std::is_copy_assignable_v<test_std::finite_inplace_stop_source<1>>);

    test_std::finite_inplace_stop_source<1> source1{};
    auto t1_1{source1.get_token<0>()};
    auto t1{source1.get_token()};
    static_assert(std::same_as<decltype(t1_1), decltype(t1)>);
    static_assert(std::same_as<decltype(t1_1), test_std::finite_inplace_stop_token<1, 0>>);

    test_std::finite_inplace_stop_source<2> source2{};
    auto t2_0{source2.get_token<0>()};
    static_assert(std::same_as<decltype(t2_0), test_std::finite_inplace_stop_token<2, 0>>);
    auto t2_1{source2.get_token<1>()};
    static_assert(std::same_as<decltype(t2_1), test_std::finite_inplace_stop_token<2, 1>>);
}