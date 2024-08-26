// include/beman/execution26/detail/forwarding_query.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_FORWARDING_QUERY
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_FORWARDING_QUERY

#include <concepts>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct forwarding_query_t
    {
        template <typename Object>
            requires requires(Object&& object, forwarding_query_t const& query)
            {
                 { ::std::forward<Object>(object).query(query) } noexcept -> ::std::same_as<bool>;
            }
        constexpr auto operator()(Object&& object) const noexcept -> bool
        {
            return ::std::forward<Object>(object).query(*this);
        }
        template <typename Object>
        constexpr auto operator()(Object&&) const noexcept -> bool
        {
            return ::std::derived_from<::std::remove_cvref_t<Object>, ::beman::execution26::forwarding_query_t>;
        }
    };

    inline constexpr forwarding_query_t forwarding_query{};
}

// ----------------------------------------------------------------------------

#endif
