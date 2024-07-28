// include/Beman/Execution26/detail/simple-allocator.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_ALLOCATOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_ALLOCATOR

#include <cstddef>
#include <concepts>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Alloc>
    concept simple_allocator
        = requires(Alloc alloc, ::std::size_t n)
        {
            { *alloc.allocate(n) } -> ::std::same_as<typename Alloc::value_type&>;
            alloc.deallocate(alloc.allocate(n), n);
        }
        && ::std::copy_constructible<Alloc>
        && ::std::equality_comparable<Alloc>
        ;
}

// ----------------------------------------------------------------------------

#endif
