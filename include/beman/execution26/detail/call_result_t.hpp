// include/beman/execution26/detail/call_result_t.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CALL_RESULT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CALL_RESULT

#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Fun, typename... Args>
/*!
 * \brief Type alias used determine the result of function [object] call.
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 * \internal
 */
using call_result_t = decltype(::std::declval<Fun>()(std::declval<Args>()...));
}

// ----------------------------------------------------------------------------

#endif
