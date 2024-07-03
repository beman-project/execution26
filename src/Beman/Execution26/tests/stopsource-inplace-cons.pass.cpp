// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/stop_token.hpp>
#include "test/execution.hpp"
#include <cassert>

auto main() -> int
{
    // Plan:
    // - Given a default constructed inplace_stop_source.
    // - Then stop_requested() yields true.
    // Reference: [stopsource.inplace.general] p2

    ::test_std::inplace_stop_source source;
    assert(source.stop_requested() == false);
}
