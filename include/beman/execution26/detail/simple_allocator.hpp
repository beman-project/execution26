// include/beman/execution26/detail/simple_allocator.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_ALLOCATOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_ALLOCATOR

#include <cstddef>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Alloc = bool>
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
