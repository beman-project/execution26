// include/beman/execution26/detail/nostopstate.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NOSTOPSTATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NOSTOPSTATE

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct nostopstate_t {
        explicit nostopstate_t() = default;
    };

    inline constexpr nostopstate_t nostopstate{};
}

// ----------------------------------------------------------------------------

#endif
