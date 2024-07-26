// include/test/execution.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_TEST_EXECUTION
#define INCLUDED_TEST_EXECUTION

#include <Beman/Execution26/stop_token.hpp>
#include <cassert>
#include <cstddef>

namespace Beman::Execution26 {}

namespace test_std    = ::Beman::Execution26;
namespace test_detail = ::Beman::Execution26::Detail;

namespace test
{
    template <typename> auto type_exists() {}
}

#endif // INCLUDED_TEST_EXECUTION
