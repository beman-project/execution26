// tests/beman/execution26/stoptoken-finite-general.test.cpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"

namespace{
    template <bool Result, std::size_t N, std::size_t I>
    auto test_mandates_i_less_than_n() -> void {
        static_assert(Result == requires{ typename test_std::finite_inplace_stop_token<N, I>; });
    }
}

TEST(stoptoken_finite_general) {
    static_assert(test_std::stoppable_token<test_std::finite_inplace_stop_token<1, 0>>);
    static_assert(test_std::stoppable_token<test_std::finite_inplace_stop_token<2, 0>>);
    static_assert(test_std::stoppable_token<test_std::finite_inplace_stop_token<2, 1>>);

    test_std::finite_inplace_stop_token<1, 0> t0;
    test_std::finite_inplace_stop_token<1, 0> t1;
    t0.swap(t1);

    test_mandates_i_less_than_n<true, 1, 0>();
    test_mandates_i_less_than_n<false, 1, 1>();
    test_mandates_i_less_than_n<true, 2, 0>();
    test_mandates_i_less_than_n<true, 2, 1>();
    test_mandates_i_less_than_n<false, 2, 2>();
}