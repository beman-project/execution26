// include/beman/execution26/detail/receiver_of.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER_OF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER_OF

#include <beman/execution26/detail/has_completions.hpp>
#include <beman/execution26/detail/receiver.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template <typename Receiver, typename Completions>
    concept receiver_of
        =  beman::execution26::receiver<Receiver>
        && beman::execution26::detail::has_completions<Receiver, Completions>
        ;

}

// ----------------------------------------------------------------------------

#endif
