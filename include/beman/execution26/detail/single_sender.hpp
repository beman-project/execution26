// include/beman/execution26/detail/single_sender.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SINGLE_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SINGLE_SENDER

#include <beman/execution26/detail/sender_in.hpp>
#include <beman/execution26/detail/single_sender_value_type.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Sender, typename Env>
concept single_sender = ::beman::execution26::sender_in<Sender, Env> &&
                        requires { typename ::beman::execution26::detail::single_sender_value_type<Sender, Env>; };
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
