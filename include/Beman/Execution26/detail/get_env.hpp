// include/Beman/Execution26/detail/get_env.hpp                       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ENV

#include <Beman/Execution26/detail/queryable.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct empty_env {};

    struct get_env_t
    {
        template <typename Object>
            requires(
                not requires(Object&& object) { ::std::as_const(object).get_env(); }
                || ::Beman::Execution26::Detail::queryable<std::remove_cvref_t<decltype(::std::declval<::std::remove_cvref_t<Object> const&>().get_env())>>
            )
        auto operator()(Object&& object) const -> decltype(auto)
        {
            if constexpr (requires{ ::std::as_const(object).get_env(); })
            {
                static_assert(noexcept(::std::as_const(object).get_env()),
                              "get_env requires the xpression to be noexcept");
                static_assert(::Beman::Execution26::Detail::queryable<std::remove_cvref_t<decltype(::std::as_const(object).get_env())>>,
                              "get_env requires the result type to be destructible");
                return ::std::as_const(object).get_env();
            }
            else
            {
                return ::Beman::Execution26::empty_env{};
            }
        }
    };

    inline constexpr get_env_t get_env{};
}

// ----------------------------------------------------------------------------

#endif
