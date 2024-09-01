// include/beman/execution26/detail/get_domain_late.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_LATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_LATE

#include <beman/execution26/detail/default_domain.hpp>
#include <beman/execution26/detail/completion_domain.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/get_domain.hpp>
#include <beman/execution26/detail/get_scheduler.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/tag_of_t.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename T>
    concept not_void = not ::std::same_as<T, void>;

    template <typename Tag>
    struct get_domain_late_helper
    {
        template <typename Sender, typename Env>
        static constexpr auto get(Sender const& sender, Env const& env) noexcept
        {
            if constexpr (requires{ { ::beman::execution26::get_domain(::beman::execution26::get_env(sender)) } -> ::beman::execution26::detail::not_void; })
                return ::beman::execution26::get_domain(::beman::execution26::get_env(sender));
            else if constexpr (requires{ { ::beman::execution26::detail::completion_domain<void>(sender) } -> ::beman::execution26::detail::not_void; })
                return ::beman::execution26::detail::completion_domain<void>(sender);
            else if constexpr (requires{ { ::beman::execution26::get_domain(env) } -> ::beman::execution26::detail::not_void; })
                return ::beman::execution26::get_domain(env);
            else if constexpr (requires{ { ::beman::execution26::get_domain(::beman::execution26::get_scheduler(env)) } -> ::beman::execution26::detail::not_void; })
                return ::beman::execution26::get_domain(::beman::execution26::get_scheduler(env));
            else
                return ::beman::execution26::default_domain();
        }
    };
    template <typename Sender, typename Env>
    constexpr auto get_domain_late(Sender const& sender, Env const& env) noexcept
    {
        using tag_t = ::beman::execution26::tag_of_t<Sender>;
        return ::beman::execution26::detail::get_domain_late_helper<tag_t>::get(sender, env);
    }
}

// ----------------------------------------------------------------------------

#endif
