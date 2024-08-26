// include/beman/execution26/detail/unstoppable_token.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_UNSTOPPABLE_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_UNSTOPPABLE_TOKEN

#include <beman/execution26/detail/stoppable_source.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template<typename Token>
    concept unstoppable_token
        =  ::beman::execution26::stoppable_token<Token>
        && requires()
            {
                requires ::std::bool_constant<not Token::stop_possible()>::value;
            }
        ;
}

// ----------------------------------------------------------------------------

#endif
