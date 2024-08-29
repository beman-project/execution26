// include/beman/execution26/detail/connect_all.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_ALL
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_ALL

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct connect_all_t
    {
        template <typename Sender, typename Receiver>
        struct state
        {
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
