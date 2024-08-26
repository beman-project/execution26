// src/beman/execution26/tests/stoptoken-general.pass.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"
#include "test/stop_token.hpp"

auto main() -> int
{
    // [stoptoken.general] p1:
    static_assert(::test_std::stoppable_token<::test_std::stop_token>);
}
