// src/beman/execution26/tests/stopsource-inplace-general.pass.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"
#include "test/stop_token.hpp"

TEST(stopsource_inplace_general)
{
    // Reference: [stopsource.inplace.general]
    static_assert(::test_detail::stoppable_source<::test_std::inplace_stop_source>);
    ::test_std::inplace_stop_source source;
    static_assert(source.stop_possible());
    ::test::stop_source([]{ return ::test_std::inplace_stop_source(); });
}
