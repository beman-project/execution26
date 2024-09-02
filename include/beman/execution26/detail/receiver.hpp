// include/beman/execution26/detail/receiver.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER

#include <beman/execution26/detail/queryable.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct receiver_t {};

    template <typename Rcvr>
    concept receiver
        =  ::std::derived_from<typename ::std::remove_cvref_t<Rcvr>::receiver_concept,
                             ::beman::execution26::receiver_t>
        && requires(::std::remove_cvref_t<Rcvr> const& rcvr)
            {
                { ::beman::execution26::get_env(rcvr) } -> ::beman::execution26::detail::queryable;
            }
        && ::std::move_constructible<::std::remove_cvref_t<Rcvr>>
        && ::std::constructible_from<::std::remove_cvref_t<Rcvr>, Rcvr>
        && (not ::std::is_final_v<::std::remove_cvref_t<Rcvr>>)
        ;
}

// ----------------------------------------------------------------------------

#endif
