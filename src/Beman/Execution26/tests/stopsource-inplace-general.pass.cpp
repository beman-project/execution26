// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/stop_token.hpp>
#include "test/execution.hpp"
#include "test/stop_token.hpp"

auto main() -> int
{
    // Reference: [stopsource.inplace.general]
    static_assert(::test_detail::stoppable_source<::test_std::inplace_stop_source>);
    ::test_std::inplace_stop_source source;
    static_assert(source.stop_possible());
    ::test::stop_source([]{ return ::test_std::inplace_stop_source(); });
}
