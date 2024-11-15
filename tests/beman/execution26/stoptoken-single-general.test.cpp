// tests/beman/execution26/stoptoken-single-general.test.cpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/single_inplace_stop_token.hpp>
#include "test/execution.hpp"

TEST(stoptoken_single_general) {
    static_assert(test_std::stoppable_token<test_std::single_inplace_stop_token>);
    test_std::single_inplace_stop_token token{};
    ASSERT(token == token);
}