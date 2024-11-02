// include/beman/execution26/detail/decayed_same_as.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_SAME_AS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_SAME_AS

#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
//-dk:TODO thoroughly test the concept
template <typename T0, typename T1>
concept decayed_same_as = ::std::same_as<::std::remove_cvref_t<T0>, ::std::remove_cvref_t<T1>>;
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
