// include/beman/execution26/detail/completion_signatures.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES

#include <beman/execution26/detail/completion_signature.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
template <::beman::execution26::detail::completion_signature...>
struct completion_signatures {};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
