// src/Beman/Execution26/tests/stopsource-general.pass.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "test/execution.hpp"
#include "test/stop_token.hpp"
#include <cassert>

auto main() -> int
{
    // Reference: [stopsource.general] p1.
    test::stop_source([]{ return ::test_std::stop_source(); });

    ::test_std::stop_source source1;
    ::test_std::stop_source source2{::test_std::nostopstate};
    static_assert(noexcept(::test_std::stop_source(::test_std::nostopstate_t())));
    assert(not source2.stop_possible());
    assert(not source2.stop_requested());

    static_assert(noexcept(source1.swap(source2)));
    static_assert(noexcept(source1.stop_requested()));
    static_assert(noexcept(source1.stop_possible()));
    static_assert(noexcept(source1.request_stop()));
}
