// src/Beman/Execution26/tests/exec-getcomplsigs.pass.cpp             -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#include <Beman/Execution26/detail/get_completion_signatures.hpp>
#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

auto main() -> int
{
    static_assert(std::same_as<test_std::get_completion_signatures_t const,
                               decltype(test_std::get_completion_signatures)>);
}