// include/Beman/Execution26/detail/basic_state.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_STATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_STATE

#include <Beman/Execution26/detail/impls_for.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <Beman/Execution26/detail/state_type.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Sender, typename Receiver>
    struct basic_state
    {
        basic_state(Sender&& sender, Receiver&& receiver) noexcept(true)
            : receiver(::std::move(receiver))
            , state(::Beman::Execution26::Detail::impls_for<
                ::Beman::Execution26::tag_of_t<Sender>
            >::get_state(::std::forward<Sender>(sender), receiver))
        {
        }

        Receiver receiver;
        ::Beman::Execution26::Detail::state_type<Sender, Receiver> state;
    };
}

// ----------------------------------------------------------------------------

#endif
