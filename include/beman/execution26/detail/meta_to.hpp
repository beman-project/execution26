// include/beman/execution26/detail/meta_to                           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_META_TO
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_META_TO

// ----------------------------------------------------------------------------

namespace beman::execution26::detail::meta
{
    template <template <typename...> class To, typename> struct to_type_list;
    template <template <typename...> class To, template <typename...> class List, typename... T>
    struct to_type_list<To, List<T...>>
    {
        using type = To<T...>;
    };
    template <template <typename...> class To, typename T>
    using to = typename ::beman::execution26::detail::meta::to_type_list<To, T>::type;
}

// ----------------------------------------------------------------------------

#endif
