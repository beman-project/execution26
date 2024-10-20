// include/beman/execution26/detail/as_awaitable.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_AS_AWAITABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_AS_AWAITABLE

#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct as_awaitable_t
    {
        template <typename E, typename P>
        auto operator()(E&& e, P& p) const
        {
            if constexpr (requires{ ::std::forward<E>(e).as_awaitable(p); })
            {
                //-dk:TODO mandates is-awaitable<E, Promise>
                return ::std::forward<E>(e).as_awaitable(p);
            }
            //-dk:TODO deal with other cases
        }
    };
    inline constexpr ::beman::execution26::as_awaitable_t as_awaitable{};
}

// ----------------------------------------------------------------------------

#endif
