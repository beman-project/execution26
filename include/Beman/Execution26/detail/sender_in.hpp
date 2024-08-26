// include/Beman/Execution26/detail/sender_in.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_IN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_IN

#include <Beman/Execution26/detail/empty_env.hpp>
#include <Beman/Execution26/detail/get_completion_signatures.hpp>
#include <Beman/Execution26/detail/queryable.hpp>
#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/valid_completion_signatures.hpp>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <typename Sender, typename Env = ::Beman::Execution26::empty_env>
    concept sender_in
        =  ::Beman::Execution26::sender<Sender>
        && ::Beman::Execution26::Detail::queryable<Env>
        && requires(Sender&& sender, Env&& env) {
            {
                ::Beman::Execution26::get_completion_signatures(
                    ::std::forward<Sender>(sender),
                    ::std::forward<Env>(env)
                )
            } -> ::Beman::Execution26::Detail::valid_completion_signatures;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
