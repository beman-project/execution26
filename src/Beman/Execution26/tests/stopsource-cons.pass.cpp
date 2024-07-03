// Copyright © 2024 Beman Project
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

auto main() -> int
{
    // Reference: [stopsource.cons] p1
    ::test_std::stop_source source;
    assert(source.stop_possible());
    assert((not source.stop_requested()));

    // Reference: [stopsource.cons] p3
    fail = true;
    try
    {
         ::test_std::stop_source();
         assert(nullptr == "can't be reached");
    }
    catch (::std::bad_alloc const&)
    {
        assert("bad_alloc was thrown");
    }
}
