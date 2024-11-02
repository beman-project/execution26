// include/beman/execution26/detail/indirect_meta_apply.hpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_INDIRECT_META_APPLY
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_INDIRECT_META_APPLY

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <bool>
struct indirect_meta_apply {
    template <template <typename...> class T, class... A>
    using meta_apply = T<A...>;
};
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
