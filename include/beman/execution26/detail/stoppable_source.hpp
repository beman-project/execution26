// include/beman/execution26/detail/stoppable_source.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_SOURCE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_SOURCE

#include <beman/execution26/detail/stoppable_token.hpp>
#include <beman/execution26/detail/common.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Source>
concept stoppable_source = requires(Source& source, const Source& csource) {
#if not defined(HAS_P3409)
    { csource.get_token() } -> ::beman::execution26::stoppable_token;
#endif
    { csource.stop_possible() } noexcept -> ::std::same_as<bool>;
    { csource.stop_requested() } noexcept -> ::std::same_as<bool>;
    { source.request_stop() } -> ::std::same_as<bool>;
};
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
