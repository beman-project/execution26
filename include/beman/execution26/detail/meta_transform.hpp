// include/beman/execution26/detail/meta_transform.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_META_TRANSFORM
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_META_TRANSFORM

#include <beman/execution26/detail/meta_contains.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail::meta::detail {
template <template <typename> class Transform, typename List>
struct transform;

template <template <typename> class Transform, template <typename... T> class List, typename... T>
struct transform<Transform, List<T...>> {
    using type = List<Transform<T>...>;
};
} // namespace beman::execution26::detail::meta::detail

namespace beman::execution26::detail::meta {
template <template <typename> class Transform, typename List>
using transform = typename ::beman::execution26::detail::meta::detail::transform<Transform, List>::type;
}

// ----------------------------------------------------------------------------

#endif
