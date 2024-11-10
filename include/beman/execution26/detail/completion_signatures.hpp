// include/beman/execution26/detail/completion_signatures.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES

#include <beman/execution26/detail/completion_signature.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
/*!
 * \brief Class template used to a specify a list of completion signatures
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 *
 * \details
 * This class template is primarily used for type computations and objects of
 * any specializaion are empty. Objects may be created to return them from
 * functions used for type computations like get_completion_signatures(sender, env).
 */
template <::beman::execution26::detail::completion_signature...>
struct completion_signatures {};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
