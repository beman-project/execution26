// include/beman/execution26/detail/stoppable_token.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_TOKEN

#include <beman/execution26/detail/check_type_alias_exist.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template<typename Token>
    concept stoppable_token
        =   requires(Token const& token)
            {
                typename ::beman::execution26::detail::check_type_alias_exist<Token::template callback_type>;
                { token.stop_requested() } noexcept -> ::std::same_as<bool>;
                { token.stop_possible() } noexcept -> ::std::same_as<bool>;
                { Token(token) } noexcept;
            }
        &&  ::std::copyable<Token>
        &&  ::std::equality_comparable<Token>
        &&  ::std::swappable<Token>
        ;
}

// ----------------------------------------------------------------------------

#endif
