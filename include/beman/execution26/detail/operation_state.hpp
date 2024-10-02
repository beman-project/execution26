// include/beman/execution26/detail/operation_state.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_OPERATION_STATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_OPERATION_STATE

#include <beman/execution26/detail/start.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct operation_state_t {};

    template <typename State>
    concept operation_state
        =  ::std::derived_from<typename State::operation_state_concept,
                               ::beman::execution26::operation_state_t>
        && ::std::is_object_v<State>
        && requires(State& state) { { ::beman::execution26::start(state) } noexcept; }
        ;
}

// ----------------------------------------------------------------------------

#endif
