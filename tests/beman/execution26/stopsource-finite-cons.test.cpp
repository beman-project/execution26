// tests/beman/execution26/stopsource-finite-cons.test.cpp            -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include "test/stop_token.hpp"
#include "test/common.hpp" //-dk:TODO remove

TEST(stopsource_finite_cons) {
    test_std::finite_inplace_stop_token<3, 0> token{};

    test_std::finite_inplace_stop_source<3> source3{};
    ASSERT(source3.stop_possible() == true);
    ASSERT(source3.stop_requested() == false);
    token = source3.get_token<0>();

    test::kill_test([&]{
        //-dk:TODO test that destroying with a requested stop callback terminates
    });
}