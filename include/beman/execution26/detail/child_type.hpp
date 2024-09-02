// include/beman/execution26/detail/child_type.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CHILD_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CHILD_TYPE

#include <cstddef>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Sender, ::std::size_t I = 0u>
    using child_type = decltype(::std::declval<Sender>().template get<I + 2>());
}

// ----------------------------------------------------------------------------

#endif
