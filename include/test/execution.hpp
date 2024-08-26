// include/test/execution.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_TEST_EXECUTION
#define INCLUDED_TEST_EXECUTION

#include <beman/execution26/stop_token.hpp>
#include <cassert>
#include <cstddef>

namespace beman::execution26 {}

namespace test_std    = ::beman::execution26;
namespace test_detail = ::beman::execution26::detail;

namespace test
{
    template <typename> auto type_exists() {}
}

#endif // INCLUDED_TEST_EXECUTION
