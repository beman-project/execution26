// include/beman/execution26/detail/when_all_with_variant.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_WHEN_ALL_WITH_VARIANT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_WHEN_ALL_WITH_VARIANT

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct when_all_with_variant_t
    {
    };
}

namespace beman::execution26
{
    using ::beman::execution26::detail::when_all_with_variant_t;
    inline constexpr ::beman::execution26::when_all_with_variant_t when_all_with_variant{};
}


// ----------------------------------------------------------------------------

#endif
