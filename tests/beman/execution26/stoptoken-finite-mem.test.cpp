// tests/beman/execution26/stoptoken-finite-mem.test.cpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include "test/execution.hpp"

namespace {
    auto test_swap() -> void {
        test_std::finite_inplace_stop_source<3> source0;
        test_std::finite_inplace_stop_source<3> source1;

        auto token0{source0.get_token<1>()};
        auto token1{source1.get_token<1>()};

        ASSERT(token0 == source0.get_token<1>());
        ASSERT(token1 == source1.get_token<1>());
        ASSERT(token0 != token1);

        token0.swap(token1);

        ASSERT(token0 == source1.get_token<1>());
        ASSERT(token1 == source0.get_token<1>());
        ASSERT(token0 != token1);
    }

    auto test_stop_requested() -> void {
        test_std::finite_inplace_stop_source<3> source;
        auto token0{source.get_token<1>()};
        decltype(token0) token1;

        ASSERT(token0.stop_requested() == false);
        ASSERT(token1.stop_requested() == false);

        source.request_stop();

        ASSERT(token0.stop_requested() == true);
        ASSERT(token1.stop_requested() == false);
    }
    
    auto test_stop_possible() {
        test_std::finite_inplace_stop_source<3> source;
        auto token0{source.get_token<1>()};
        decltype(token0) token1;

        ASSERT(token0.stop_possible());
        ASSERT(not token1.stop_possible());
    }
}

TEST(stoptoken_finite_mem) {
    test_swap();
    test_stop_requested();
    test_stop_possible();
}