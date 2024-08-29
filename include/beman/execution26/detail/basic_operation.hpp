// include/beman/execution26/detail/basic_operation.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_OPERATION
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_OPERATION

#include <beman/execution26/detail/basic_state.hpp>
#include <beman/execution26/detail/state_type.hpp>
#include <beman/execution26/detail/valid_specialization.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Sender, typename Receiver>
        requires ::beman::execution26::detail::valid_specialization<
            ::beman::execution26::detail::state_type, Sender, Receiver
        >
    struct basic_operation
        : ::beman::execution26::detail::basic_state<Sender, Receiver>
    {
        basic_operation(Sender&& sender, Receiver&& receiver) noexcept(true/*-dk:TODO*/)
            : ::beman::execution26::detail::basic_state<Sender, Receiver>(
                ::std::forward<Sender>(sender), ::std::move(receiver)
            )
        {
        }
    };
}

// ----------------------------------------------------------------------------

#endif
