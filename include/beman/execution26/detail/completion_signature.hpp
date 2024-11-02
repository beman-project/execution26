// include/beman/execution26/detail/completion_signature.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURE

#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename>
struct is_set_error : ::std::false_type {};
template <typename Error>
struct is_set_error<::beman::execution26::set_error_t(Error)> : ::std::true_type {};

template <typename>
struct is_set_stopped : ::std::false_type {};
template <>
struct is_set_error<::beman::execution26::set_stopped_t()> : ::std::true_type {};

template <typename>
struct is_set_value : ::std::false_type {};
template <typename... Args>
struct is_set_value<::beman::execution26::set_value_t(Args...)> : ::std::true_type {};

template <typename Fun>
concept completion_signature =
    ::beman::execution26::detail::is_set_error<Fun>::value ||
    ::beman::execution26::detail::is_set_stopped<Fun>::value || ::beman::execution26::detail::is_set_value<Fun>::value;
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
