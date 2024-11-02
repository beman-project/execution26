// inclue/beman/execution26/detail/set_error.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_ERROR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_ERROR

#include <beman/execution26/detail/common.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
struct set_error_t {
    template <typename Receiver, typename Error>
    auto operator()(Receiver&, Error&&) const
        -> void = BEMAN_EXECUTION26_DELETE("set_error requires the receiver to be passed as non-const rvalue");
    template <typename Receiver, typename Error>
    auto operator()(const Receiver&&, Error&&) const
        -> void = BEMAN_EXECUTION26_DELETE("set_error requires the receiver to be passed as non-const rvalue");
    template <typename Receiver, typename Error>
    auto operator()(Receiver&&, Error&&) const -> void
        requires(not requires(Receiver&& receiver, Error&& error) {
                    ::std::forward<Receiver>(receiver).set_error(::std::forward<Error>(error));
                })
    = BEMAN_EXECUTION26_DELETE("set_error requires a suitable member overload on the receiver");
    template <typename Receiver, typename Error>
        requires(not noexcept(::std::declval<Receiver>().set_error(::std::declval<Error>())))
    auto operator()(Receiver&&, Error&&) const
        -> void = BEMAN_EXECUTION26_DELETE("the call to receiver.set_error(error) has to be noexcept");

    template <typename Receiver, typename Error>
    auto operator()(Receiver&& receiver, Error&& error) const noexcept -> void {
        ::std::forward<Receiver>(receiver).set_error(::std::forward<Error>(error));
    }
};

inline constexpr set_error_t set_error{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
