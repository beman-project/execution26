// include/beman/execution26/detail/emplace_from.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_EMPLACE_FROM
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_EMPLACE_FROM

#include <beman/execution26/detail/call_result_t.hpp>
#include <beman/execution26/detail/nothrow_callable.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Fun>
    struct emplace_from
    {
        using type = ::beman::execution26::detail::call_result_t<Fun>;
        Fun fun;

        constexpr operator type() && noexcept(::beman::execution26::detail::nothrow_callable<Fun>) 
        {
            return ::std::move(fun)();
        }
    };
    template <typename Fun>
    emplace_from(Fun&&) -> emplace_from<::std::remove_cvref_t<Fun>>;
}

// ----------------------------------------------------------------------------

#endif
