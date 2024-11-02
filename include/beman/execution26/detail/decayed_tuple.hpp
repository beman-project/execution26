// include/beman/execution26/detail/decayed_tuple.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_TUPLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_TUPLE

#include <tuple>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename... T>
using decayed_tuple = ::std::tuple<::std::decay_t<T>...>;
}

// ----------------------------------------------------------------------------

#endif
