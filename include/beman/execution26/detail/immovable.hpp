// include/beman/execution26/detail/immovable.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_IMMOVABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_IMMOVABLE

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
struct immovable {
    constexpr immovable()                          = default;
    immovable(immovable&&)                         = delete;
    immovable(const immovable&)                    = delete;
    ~immovable()                                   = default;
    auto operator=(immovable&&) -> immovable&      = delete;
    auto operator=(const immovable&) -> immovable& = delete;
};
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
