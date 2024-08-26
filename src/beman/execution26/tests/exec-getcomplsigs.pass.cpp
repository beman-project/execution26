// src/beman/execution26/tests/exec-getcomplsigs.pass.cpp             -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#include <beman/execution26/detail/get_completion_signatures.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

auto main() -> int
{
    static_assert(std::same_as<test_std::get_completion_signatures_t const,
                               decltype(test_std::get_completion_signatures)>);
    //-dk:TODO add actual tests
}