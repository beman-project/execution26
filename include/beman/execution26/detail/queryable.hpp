// include/beman/execution26/detail/queryable.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_QUERYABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_QUERYABLE

#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename T>
concept queryable = ::std::destructible<T>;
}

// ----------------------------------------------------------------------------

#endif
