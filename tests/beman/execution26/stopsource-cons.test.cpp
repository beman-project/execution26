// src/beman/execution26/tests/stopsource-cons.test.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "test/execution.hpp"
#include <new>
#include <stdexcept>
#include <cassert>
#include <cstddef>
#include <cstring>

bool fail{};

auto operator new(::std::size_t size) -> void*
{
    return fail? throw ::std::bad_alloc(): ::std::malloc(size);
}

auto operator delete(void* ptr) noexcept -> void
{
    ::std::free(ptr);
}
auto operator delete(void* ptr, ::std::size_t) noexcept -> void
{
    ::std::free(ptr);
}

TEST(stopsource_cons)
{
    // Reference: [stopsource.cons] p1
    ::test_std::stop_source source;
    ASSERT(source.stop_possible());
    ASSERT((not source.stop_requested()));

    // Reference: [stopsource.cons] p3
    fail = true;
    try
    {
         ::test_std::stop_source();
         ASSERT(nullptr == "can't be reached");
    }
    catch (::std::bad_alloc const&)
    {
        ASSERT(nullptr != "bad_alloc was thrown");
    }
}
