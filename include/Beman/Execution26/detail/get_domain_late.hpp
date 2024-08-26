// include/Beman/Execution26/detail/get_domain_late.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_LATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_LATE

#include <Beman/Execution26/detail/default_domain.hpp>
#include <Beman/Execution26/detail/completion_domain.hpp>
#include <Beman/Execution26/detail/get_env.hpp>
#include <Beman/Execution26/detail/get_domain.hpp>
#include <Beman/Execution26/detail/get_scheduler.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename T>
    concept not_void = not ::std::same_as<T, void>;

    template <typename Tag>
    struct get_domain_late_helper
    {
        template <typename Sender, typename Env>
        static constexpr auto get(Sender const& sender, Env const& env) noexcept
        {
            if constexpr (requires{ { ::Beman::Execution26::get_domain(::Beman::Execution26::get_env(sender)) } -> ::Beman::Execution26::Detail::not_void; })
                return ::Beman::Execution26::get_domain(::Beman::Execution26::get_env(sender));
            else if constexpr (requires{ { ::Beman::Execution26::Detail::completion_domain<void>(sender) } -> ::Beman::Execution26::Detail::not_void; })
                return ::Beman::Execution26::Detail::completion_domain<void>(sender);
            else if constexpr (requires{ { ::Beman::Execution26::get_domain(env) } -> ::Beman::Execution26::Detail::not_void; })
                return ::Beman::Execution26::get_domain(env);
            else if constexpr (requires{ { ::Beman::Execution26::get_domain(::Beman::Execution26::get_scheduler(env)) } -> ::Beman::Execution26::Detail::not_void; })
                return ::Beman::Execution26::get_domain(::Beman::Execution26::get_scheduler(env));
            else
                return ::Beman::Execution26::default_domain();
        }
    };
    template <typename Sender, typename Env>
    constexpr auto get_domain_late(Sender const& sender, Env const& env) noexcept
    {
        using tag_t = ::Beman::Execution26::tag_of_t<Sender>;
        return ::Beman::Execution26::Detail::get_domain_late_helper<tag_t>::get(sender, env);
    }
}

// ----------------------------------------------------------------------------

#endif
