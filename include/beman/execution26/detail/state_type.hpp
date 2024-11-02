// include/beman/execution26/detail/state_type.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STATE_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STATE_TYPE

#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/call_result_t.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/tag_of_t.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Sender, typename Receiver>
using state_type = ::std::decay_t< ::beman::execution26::detail::call_result_t<
    decltype(::beman::execution26::detail::impls_for< ::beman::execution26::tag_of_t<Sender> >::get_state),
    Sender,
    Receiver&> >;
}

// ----------------------------------------------------------------------------

#endif
