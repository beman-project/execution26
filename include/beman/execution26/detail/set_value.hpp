// include/beman/execution26/detail/set_value.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_VALUE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_VALUE

#include <beman/execution26/detail/common.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct set_value_t
    {
        template <typename Receiver, typename... Args>
        auto operator()(Receiver&, Args&&...) const -> void
            = BEMAN_EXECUTION26_DELETE("set_value requires the receiver to be passed as non-const rvalue");
        template <typename Receiver, typename... Args>
        auto operator()(Receiver const&&, Args&&...) const -> void
            = BEMAN_EXECUTION26_DELETE("set_value requires the receiver to be passed as non-const rvalue");
        template <typename Receiver, typename... Args>
        auto operator()(Receiver&&, Args&&...) const -> void
            requires (not requires(Receiver&& receiver, Args&&... args)
            {
                ::std::forward<Receiver>(receiver).set_value(::std::forward<Args>(args)...);
            })
            = BEMAN_EXECUTION26_DELETE("set_value requires a suitable member overload on the receiver");
        template <typename Receiver, typename... Args>
            requires (not noexcept(::std::declval<Receiver>().set_value(::std::declval<Args>()...)))
        auto operator()(Receiver&&, Args&&...) const -> void
            = BEMAN_EXECUTION26_DELETE("the call to receiver.set_value(args...) has to be noexcept");

        template <typename Receiver, typename... Args>
        auto operator()(Receiver&& receiver, Args&&... args) const noexcept -> void
        {
            ::std::forward<Receiver>(receiver).set_value(::std::forward<Args>(args)...);
        }
    };
    inline constexpr set_value_t set_value{};
}

// ----------------------------------------------------------------------------

#endif
