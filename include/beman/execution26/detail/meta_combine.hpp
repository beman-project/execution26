// include/beman/execution26/detail/meta_combine.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_META_COMBINE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_META_COMBINE

#include <beman/execution26/detail/type_list.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail::meta::detail
{
    template <typename, typename> struct combine;

    template <template <typename...> class L0, typename... T0,
              template <typename...> class L1, typename... T1>
    struct combine<L0<T0...>, L1<T1...>>
    {
        using type = L0<T0..., T1...>;
    };
}

namespace beman::execution26::detail::meta
{
    template <typename L0, typename L1>
    using combine
        = ::beman::execution26::detail::meta::detail::combine<L0, L1>::type;
}

// ----------------------------------------------------------------------------

#endif
