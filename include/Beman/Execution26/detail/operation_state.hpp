// include/Beman/Execution26/detail/operation_state.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_OPERATION_STATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_OPERATION_STATE

#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct operation_state_t {};

    template <typename State>
    concept operation_state
        =  ::std::derived_from<typename State::operation_state_concept,
                               ::Beman::Execution26::operation_state_t>
        && ::std::is_object_v<State>
        && requires(State& state) { { state.start() } noexcept; }
        ;
}

// ----------------------------------------------------------------------------

#endif
