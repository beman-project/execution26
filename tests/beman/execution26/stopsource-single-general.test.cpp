// tests/beman/execution26/stopsource-single-general.test.cpp            -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/single_inplace_stop_source.hpp>
#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"

TEST(stopsource_single_general) {
    static_assert(test_detail::stoppable_source<test_std::single_inplace_stop_source>);
}