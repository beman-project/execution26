// include/beman/execution26/detail/value_types_of_t.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_VALUE_TYPE_OF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_VALUE_TYPE_OF

#include <beman/execution26/detail/decayed_tuple.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/variant_or_empty.hpp>
#include <beman/execution26/detail/gather_signatures.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    #if 0
    -dk:TODO implement value_types_of_t
    template <typename Sender,
             typename Env = ::beman::execution26::empty_env,
             template <typeame...> class Tuple = ::beman::execution26::detail::decayed_tuple,
             template <typeame...> class Tuple = ::beman::execution26::detail::variant_or_empty,
             >
    using value_types_of_t = bool;
    #endif
}

// ----------------------------------------------------------------------------

#endif
