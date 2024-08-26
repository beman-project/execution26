// include/beman/execution26/detail/nothrow_callable.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NOTHROW_CALLABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NOTHROW_CALLABLE

#include <beman/execution26/detail/callable.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Fun, typename... Args>
    concept nothrow_callable
        =  ::beman::execution26::detail::callable<Fun, Args...>
        && requires(Fun&& fun, Args&&... args)
        {
            { ::std::forward<Fun>(fun)(::std::forward<Args>(args)...) } noexcept;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
