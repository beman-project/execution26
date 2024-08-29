// include/beman/execution26/detail/connect_all.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_ALL
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_ALL

#include <beman/execution26/detail/connect_result_t.hpp>
#include <beman/execution26/detail/connect.hpp>
#include <tuple>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct connect_all_t
    {
        template <typename Sender, typename Receiver>
        struct state
        {
            ::beman::execution26::connect_result_t<Sender, Receiver> op;
            state(Sender&& sender, Receiver&& receiver)
                : op(::beman::execution26::connect(::std::forward<Sender>(sender),
                                                   ::std::forward<Receiver>(receiver)))
            {
            }
        };
        auto operator()(auto&&...) const noexcept(true/*-dk:TODO*/)
        {
            return 0;
        }
    };
    inline constexpr connect_all_t connect_all{};
}

// ----------------------------------------------------------------------------

#endif
