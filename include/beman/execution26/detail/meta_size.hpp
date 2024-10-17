// include/beman/execution26/detail/meta_size.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_META_SIZE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_META_SIZE

#include <cstddef>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail::meta
{
    template <typename> struct size;
    template <template <typename...> class L, typename... T>
    struct size<L<T...>>
    {
        static constexpr ::std::size_t value{sizeof...(T)};
    };
    template <typename T>
    inline constexpr ::std::size_t size_v{::beman::execution26::detail::meta::size<T>::value};
}

// ----------------------------------------------------------------------------

#endif
