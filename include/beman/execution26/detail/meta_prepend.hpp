// include/beman/execution26/detail/meta_prepend.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_META_PREPEND
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_META_PREPEND

// ----------------------------------------------------------------------------

namespace beman::execution26::detail::meta::detail
{
    template <template <typename...> class, typename, typename>
    struct prepend;

    template <template <typename...> class List, typename H, typename... T>
    struct prepend<List, H, List<T...>>
    {
        using type = List<H, T...>;
    };
}

namespace beman::execution26::detail::meta
{
    template <template <typename...> class List, typename H, typename Tail>
    using prepend = ::beman::execution26::detail::meta::detail::prepend<List, H, Tail>::type;
}

// ----------------------------------------------------------------------------

#endif
