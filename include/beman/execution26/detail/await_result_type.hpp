// include/beman/execution26/detail/await_result_type.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_AWAIT_RESULT_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_AWAIT_RESULT_TYPE

#include <beman/execution26/detail/get_awaiter.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename T, typename Promise>
using await_result_type =
    decltype(::beman::execution26::detail::get_awaiter(::std::declval<T>(), ::std::declval<Promise&>())
                 .await_resume());
}

// ----------------------------------------------------------------------------

#endif
