// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "test/execution.hpp"
#include "test/stop_token.hpp"

int main()
{
    // Reference: [stopsource.general] p1.
    test::stop_source([]{ return ::beman::cpp26::stop_source(); });
}
