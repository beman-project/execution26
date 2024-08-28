// include/beman/execution26/detail/value_types_of_t.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_VALUE_TYPE_OF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_VALUE_TYPE_OF

#include <beman/execution26/detail/decayed_tuple.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/variant_or_empty.hpp>
#include <beman/execution26/detail/gather_signatures.hpp>
#include <beman/execution26/detail/completion_signatures_of_t.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template <typename Sender,
             typename Env = ::beman::execution26::empty_env,
             template <typename...> class Tuple = ::beman::execution26::detail::decayed_tuple,
             template <typename...> class Variant = ::beman::execution26::detail::variant_or_empty
             >
        requires ::beman::execution26::sender_in<Sender, Env>
    using value_types_of_t
        = ::beman::execution26::detail::gather_signatures<
            ::beman::execution26::set_value_t,
            ::beman::execution26::completion_signatures_of_t<Sender, Env>,
            Tuple,
            Variant
        >;
}
// ----------------------------------------------------------------------------

#endif
