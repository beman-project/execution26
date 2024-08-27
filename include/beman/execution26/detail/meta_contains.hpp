// include/beman/execution26/detail/meta_contains.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_META_CONTAINS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_META_CONTAINS

#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail::meta::detail
{
    template <typename C, typename... S>
    struct contains;

    template <typename C>
    struct contains<C>
    {
        static constexpr bool value = false;
    };

    template <typename C, typename S, typename... T>
    struct contains<C, S, T...>
    {
        static constexpr bool value
            = ::std::same_as<C, S> || contains<C, T...>::value;
    };
}

namespace beman::execution26::detail::meta
{
    template <typename T, typename... S>
    inline constexpr bool contains = ::beman::execution26::detail::meta::detail::contains<T, S...>::value;
}


// ----------------------------------------------------------------------------

#endif
