// include/beman/execution26/detail/basic_sender.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER

#include <beman/execution26/detail/sender.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Tag, typename Data, typename... Child>
    struct basic_sender
    {
        using sender_concept = ::beman::execution26::sender_t;
    };
}

// ----------------------------------------------------------------------------

#endif
