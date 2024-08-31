// include/beman/execution26/detail/get_domain_early.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_EARLY
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_EARLY

#include <beman/execution26/detail/completion_domain.hpp>
#include <beman/execution26/detail/default_domain.hpp>
#include <beman/execution26/detail/get_domain.hpp>
#include <beman/execution26/detail/get_env.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Sender>
    constexpr auto get_domain_early(Sender const& sender) noexcept
    {
        if constexpr (requires{
                ::beman::execution26::get_domain(
                    ::beman::execution26::get_env(sender)
                );
            })
            return decltype(
                ::beman::execution26::get_domain(
                    ::beman::execution26::get_env(sender)
                )
            ){};
        else if constexpr (requires{
                ::beman::execution26::detail::completion_domain(sender);
            })
            return decltype(::beman::execution26::detail::completion_domain(sender)){};
        else
            return ::beman::execution26::default_domain{};
    }
}

// ----------------------------------------------------------------------------

#endif
