// include/Beman/Execution26/detail/get_stop_token.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_STOP_TOKEN

#include <Beman/Execution26/detail/forwarding_query.hpp>
#include <Beman/Execution26/detail/never_stop_token.hpp>
#include <Beman/Execution26/detail/stoppable_token.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct get_stop_token_t
    {
        template <typename Object>
            requires requires(Object&& object, get_stop_token_t const& tag)
            {
                { ::std::as_const(object).query(tag) } noexcept -> ::Beman::Execution26::stoppable_token;
            }
        auto operator()(Object&& object) const noexcept
        {
            return ::std::as_const(object).query(*this);
        }

        template <typename Object>
        auto operator()(Object&&) const noexcept -> ::Beman::Execution26::never_stop_token
        {
            return {};
        }

        constexpr auto query(::Beman::Execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_stop_token_t get_stop_token{};
}

// ----------------------------------------------------------------------------

#endif
