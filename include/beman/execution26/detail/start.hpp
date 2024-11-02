// include/beman/execution26/detail/start.hpp                         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_START
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_START

#include <beman/execution26/detail/common.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
struct start_t {
    template <typename State>
    auto operator()(State&&) const -> void = BEMAN_EXECUTION26_DELETE("start(obj) requires an lvalue argument");
    template <typename State>
        requires(not requires(State& state) { state.start(); })
    auto operator()(State&) const -> void = BEMAN_EXECUTION26_DELETE("state needs to have a start() member");
    template <typename State>
        requires(not requires(const State& state) { state.start(); })
    auto operator()(const State&) const -> void = BEMAN_EXECUTION26_DELETE("state needs to have a start() member");

    template <typename State>
        requires(not requires(State& state) {
                    { state.start() } noexcept;
                })
    auto operator()(State&) const -> void = BEMAN_EXECUTION26_DELETE("state start() member has to be noexcept");
    template <typename State>
        requires(not requires(const State& state) {
                    { state.start() } noexcept;
                })
    auto operator()(const State&) const -> void = BEMAN_EXECUTION26_DELETE("state start() member has to be noexcept");

    template <typename State>
    auto operator()(const State& state) const noexcept -> void {
        state.start();
    }
    template <typename State>
    auto operator()(State& state) const noexcept -> void {
        state.start();
    }
};

inline constexpr start_t start{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
