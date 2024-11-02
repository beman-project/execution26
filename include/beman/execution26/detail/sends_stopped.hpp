// include/beman/execution26/detail/sends_stopped.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDS_STOPPED
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDS_STOPPED

#include <beman/execution26/detail/completion_signatures_of_t.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/gather_signatures.hpp>
#include <beman/execution26/detail/sender_in.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/type_list.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
template <typename Sender, typename Env = ::beman::execution26::empty_env>
    requires ::beman::execution26::sender_in<Sender, Env>
inline constexpr bool sends_stopped{not::std::same_as<
    ::beman::execution26::detail::type_list<>,
    ::beman::execution26::detail::gather_signatures< ::beman::execution26::set_stopped_t,
                                                     ::beman::execution26::completion_signatures_of_t<Sender, Env>,
                                                     ::beman::execution26::detail::type_list,
                                                     ::beman::execution26::detail::type_list> >};
}

// ----------------------------------------------------------------------------

#endif
