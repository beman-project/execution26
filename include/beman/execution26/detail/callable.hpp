// include/beman/execution26/detail/callable.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CALLABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CALLABLE

#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
/*!
 * \brief Concept used to detect callable objects.
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 * \internal
 */
template <typename Fun, typename... Args>
concept callable = requires(Fun&& fun, Args&&... args) { ::std::forward<Fun>(fun)(::std::forward<Args>(args)...); };
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
