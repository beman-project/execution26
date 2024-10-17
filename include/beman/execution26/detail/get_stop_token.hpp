// include/beman/execution26/detail/get_stop_token.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_STOP_TOKEN

#include <beman/execution26/detail/forwarding_query.hpp>
#include <beman/execution26/detail/never_stop_token.hpp>
#include <beman/execution26/detail/stoppable_token.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Token>
    concept decayed_stoppable_token
        = ::beman::execution26::stoppable_token<::std::decay_t<Token>>
        ;
}
namespace beman::execution26
{
    struct get_stop_token_t
    {
        template <typename Object>
            requires requires(Object&& object, get_stop_token_t const& tag)
            {
                { ::std::as_const(object).query(tag) } noexcept -> ::beman::execution26::detail::decayed_stoppable_token;
            }
        auto operator()(Object&& object) const noexcept
        {
            return ::std::as_const(object).query(*this);
        }

        template <typename Object>
        auto operator()(Object&&) const noexcept -> ::beman::execution26::never_stop_token
        {
            return {};
        }

        constexpr auto query(::beman::execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_stop_token_t get_stop_token{};
}

// ----------------------------------------------------------------------------

#endif
