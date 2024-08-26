// include/Beman/Execution26/detail/sender_for.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_FOR

#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Sender, typename Tag>
    concept sender_for
        =  ::Beman::Execution26::sender<Sender>
        && ::std::same_as<::Beman::Execution26::tag_of_t<Sender>, Tag>
        ;
}

// ----------------------------------------------------------------------------

#endif
