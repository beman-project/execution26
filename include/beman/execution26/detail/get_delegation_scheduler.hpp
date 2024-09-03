// include/beman/execution26/detail/get_delegation_scheduler.hpp      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DELEGATION_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DELEGATION_SCHEDULER

#include <beman/execution26/detail/forwarding_query.hpp>
#include <beman/execution26/detail/scheduler.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct get_delegation_scheduler_t
    {
        template <typename Env>
            requires requires(Env&& env, get_delegation_scheduler_t const& g)
            {
                { ::std::as_const(env).query(g) } noexcept
                    -> ::beman::execution26::scheduler;
            }
        auto operator()(Env&& env) const noexcept
        {
            return ::std::as_const(env).query(*this);
        }
        constexpr auto query(::beman::execution26::forwarding_query_t const&) const
            noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_delegation_scheduler_t get_delegation_scheduler{};
}

// ----------------------------------------------------------------------------

#endif
