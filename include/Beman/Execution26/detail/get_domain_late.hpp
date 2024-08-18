// include/Beman/Execution26/detail/get_domain_late.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_LATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN_LATE

// #include <Beman/Execution26/detail/continues_on.hpp>
#include <Beman/Execution26/detail/default_domain.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Tag>
    struct get_domain_late_helper
    {
        template <typename Sender, typename Env>
        static constexpr auto get(Sender const& sender, Env const& env) noexcept
        {
            (void)sender; (void)env;
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
