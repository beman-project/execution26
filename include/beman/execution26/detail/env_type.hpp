// include/Beman/Execution26/detail/env_type.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ENV_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ENV_TYPE

#include <Beman/Execution26/detail/call_result_t.hpp>
#include <Beman/Execution26/detail/impls_for.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <Beman/Execution26/detail/state_type.hpp>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Index, typename Sender, typename Receiver>
    using env_type = ::Beman::Execution26::Detail::call_result_t<
        decltype(::Beman::Execution26::Detail::impls_for<
            ::Beman::Execution26::tag_of_t<Sender>
        >::get_env),
        Index,
        ::Beman::Execution26::Detail::state_type<Sender, Receiver>&,
        Receiver const&
    >;
}

// ----------------------------------------------------------------------------

#endif
