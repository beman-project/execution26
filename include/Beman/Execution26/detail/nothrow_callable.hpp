// include/Beman/Execution26/detail/nothrow_callable.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NOTHROW_CALLABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NOTHROW_CALLABLE

#include <Beman/Execution26/detail/callable.hpp>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Fun, typename... Args>
    concept nothrow_callable
        =  ::Beman::Execution26::Detail::callable<Fun, Args...>
        && requires(Fun&& fun, Args&&... args)
        {
            { ::std::forward<Fun>(fun)(::std::forward<Args>(args)...) } noexcept;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
