// include/beman/execution26/detail/stop_token_of_t.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOP_TOKEN_OF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOP_TOKEN_OF

#include <beman/execution26/detail/get_stop_token.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template<typename T>
    using stop_token_of_t = ::std::remove_cvref_t<decltype(
        ::beman::execution26::get_stop_token(::std::declval<T>())
    )>;
}

// ----------------------------------------------------------------------------

#endif
