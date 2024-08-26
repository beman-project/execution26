// include/Beman/Execution26/detail/state_type.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STATE_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STATE_TYPE

#include <Beman/Execution26/detail/impls_for.hpp>
#include <Beman/Execution26/detail/call_result_t.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Sender, typename Receiver>
    using state_type = ::std::decay_t<
        ::Beman::Execution26::Detail::call_result_t<
            decltype(::Beman::Execution26::Detail::impls_for<
                ::Beman::Execution26::tag_of_t<Sender>
            >::get_state), Sender, Receiver&
        >
    >;
}

// ----------------------------------------------------------------------------

#endif
