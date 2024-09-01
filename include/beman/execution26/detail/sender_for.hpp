// include/beman/execution26/detail/sender_for.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_FOR

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/tag_of_t.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Sender, typename Tag>
    concept sender_for
        =  ::beman::execution26::sender<Sender>
        && ::std::same_as<::beman::execution26::tag_of_t<Sender>, Tag>
        ;
}

// ----------------------------------------------------------------------------

#endif
