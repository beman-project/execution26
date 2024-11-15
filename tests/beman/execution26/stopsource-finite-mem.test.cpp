// tests/beman/execution26/stopsource-finite-mem.test.cpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"

namespace {
    auto test_get_token() {
        test_std::finite_inplace_stop_source<1> source1_0{};
        test_std::finite_inplace_stop_source<1> source1_1{};
        auto token1_0{source1_0.get_token()};
        auto token1_0_0{source1_0.get_token<0>()};
        auto token1_1_0{source1_1.get_token<0>()};

        ASSERT(not token1_0.stop_requested());
        ASSERT(not token1_0_0.stop_requested());
        ASSERT(not token1_1_0.stop_requested());

        ASSERT(token1_0 == token1_0_0);
        ASSERT(token1_0_0 != token1_1_0);

        source1_0.request_stop();
        ASSERT(token1_0.stop_requested());
        ASSERT(token1_0_0.stop_requested());
        ASSERT(not token1_1_0.stop_requested());

        test_std::finite_inplace_stop_source<2> source2{};
        auto token2_0{source2.get_token<0>()};
        auto token2_1{source2.get_token<1>()};

        [](auto t0, auto t1){ static_assert(not requires{ t0 == t1; }); }(token2_0, token2_1); }

    auto test_request_stop() {
        test_std::finite_inplace_stop_source<3> source3{};
        auto token3_0{source3.get_token<0>()};
        auto token3_1{source3.get_token<1>()};

        //-dk:TODO setup calllbacks to be called

        ASSERT(source3.stop_requested() == false);
        ASSERT(token3_0.stop_requested() == false);
        ASSERT(token3_1.stop_requested() == false);
        bool rc1{source3.request_stop()};
        ASSERT(rc1 == true);
        ASSERT(source3.stop_requested() == true);

        bool rc2{source3.request_stop()};
        ASSERT(rc2 == false);
        ASSERT(source3.stop_requested() == true);
        ASSERT(token3_0.stop_requested() == true);
        ASSERT(token3_1.stop_requested() == true);

        bool rc3{source3.request_stop()};
        ASSERT(rc3 == false);
        ASSERT(source3.stop_requested() == true);
        ASSERT(token3_0.stop_requested() == true);
        ASSERT(token3_1.stop_requested() == true);
    }
}

TEST(stopsource_finite_mem) {
    test_get_token();
    test_request_stop();
}