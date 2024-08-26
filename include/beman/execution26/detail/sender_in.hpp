// include/beman/execution26/detail/sender_in.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_IN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_IN

#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/get_completion_signatures.hpp>
#include <beman/execution26/detail/queryable.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/valid_completion_signatures.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template <typename Sender, typename Env = ::beman::execution26::empty_env>
    concept sender_in
        =  ::beman::execution26::sender<Sender>
        && ::beman::execution26::detail::queryable<Env>
        && requires(Sender&& sender, Env&& env) {
            {
                ::beman::execution26::get_completion_signatures(
                    ::std::forward<Sender>(sender),
                    ::std::forward<Env>(env)
                )
            } -> ::beman::execution26::detail::valid_completion_signatures;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
