// include/beman/execution26/detail/connect.hpp                       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT

#include <beman/execution26/detail/transform_sender.hpp>
#include <beman/execution26/detail/get_domain_late.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/operation_state.hpp>
#include <type_traits>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct connect_t
    {
        template <typename Sender, typename Receiver>
        auto operator()(Sender&& sender, Receiver&& receiver)  const
        {
            auto new_sender = [&sender, &receiver]() -> decltype(auto) {
                return ::beman::execution26::transform_sender(
                    decltype(
                        ::beman::execution26::detail::get_domain_late(::std::forward<Sender>(sender),
                            ::beman::execution26::get_env(receiver))
                    ){},
                    ::std::forward<Sender>(sender),
                    ::beman::execution26::get_env(receiver)
                );
            };

            if constexpr (requires{ new_sender().connect(::std::forward<Receiver>(receiver)); })
            {
                using state_type = decltype(new_sender().connect(::std::forward<Receiver>(receiver)));
                static_assert(::beman::execution26::operation_state<state_type>);
                return new_sender().connect(::std::forward<Receiver>(receiver));
            }
            else
            {
                static_assert(::std::same_as<int, Sender>);
            }
        }
    };
    inline constexpr connect_t connect{};
}

// ----------------------------------------------------------------------------

#endif
