// include/beman/execution26/detail/decayed_typeof.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_TYPEOF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_TYPEOF

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <auto const& Tag>
    // using decayed_typeof = decltype(auto(Tag));
    using decayed_typeof = decltype([]{ return Tag; }());
}

// ----------------------------------------------------------------------------

#endif
