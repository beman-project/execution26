// include/Beman/Execution26/detail/receiver.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER

#include <Beman/Execution26/detail/queryable.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct receiver_t {};

    template <typename Rcvr>
    concept receiver
        =  ::std::derived_from<typename ::std::remove_cvref_t<Rcvr>::receiver_concept,
                             ::Beman::Execution26::receiver_t>
        && requires(::std::remove_cvref_t<Rcvr> const& rcvr)
            {
                { ::Beman::Execution26::get_env(rcvr) } -> ::Beman::Execution26::Detail::queryable;
            }
        && ::std::move_constructible<::std::remove_cvref_t<Rcvr>>
        && ::std::constructible_from<::std::remove_cvref_t<Rcvr>, Rcvr>
        && (not ::std::is_final_v<::std::remove_cvref_t<Rcvr>>)
        ;
}

// ----------------------------------------------------------------------------

#endif
