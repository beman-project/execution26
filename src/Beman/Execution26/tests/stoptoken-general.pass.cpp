// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/stop_token.hpp>
#include "test/execution.hpp"
#include "test/stop_token.hpp"

auto main() -> int
{
    // [stoptoken.general] p1:
    static_assert(::test_std::stoppable_token<::test_std::stop_token>);
}
