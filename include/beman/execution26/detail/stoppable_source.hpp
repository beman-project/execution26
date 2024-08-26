// include/beman/execution26/detail/stoppable_source.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_SOURCE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_SOURCE

#include <beman/execution26/detail/stoppable_token.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template<typename Source>
    concept stoppable_source
        =   requires(Source& source, Source const& csource)
            {
                { csource.get_token() } -> ::beman::execution26::stoppable_token;
                { csource.stop_possible() } noexcept -> ::std::same_as<bool>;
                { csource.stop_requested() } noexcept -> ::std::same_as<bool>;
                { source.request_stop() } -> ::std::same_as<bool>;
            }
        ;
}

// ----------------------------------------------------------------------------

#endif
