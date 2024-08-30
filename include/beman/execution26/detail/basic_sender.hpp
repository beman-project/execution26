// include/beman/execution26/detail/basic_sender.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER

#include <beman/execution26/detail/decays_to.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/sender.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Tag, typename Data, typename... Child>
    struct basic_sender
    {
        using sender_concept = ::beman::execution26::sender_t;

#if __cpp_explicit_this_parameter < 302110L
        template <typename Env>
        auto get_completion_signatures(Env&&) && noexcept
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender&&, Env>
        {
            return {};
        }
        template <typename Env>
        auto get_completion_signatures(Env&&) const&& noexcept
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender const&&, Env>
        {
            return {};
        }
        template <typename Env>
        auto get_completion_signatures(Env&&) & noexcept
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender&, Env>
        {
            return {};
        }
        template <typename Env>
        auto get_completion_signatures(Env&&) const& noexcept
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender const&, Env>
        {
            return {};
        }
#else
        template <::beman::execution26::detail::decays_to<basic_sender> Self,
                  typename Env>
        auto get_completion_signatures(this Self&&, Env&&) noexcept
            -> ::beman::execution26::detail::completion_signatures_for<Self, Env>
        {
            return {};
        }
#endif
    };
}

// ----------------------------------------------------------------------------

#endif
