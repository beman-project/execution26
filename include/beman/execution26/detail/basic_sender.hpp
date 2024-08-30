// include/beman/execution26/detail/basic_sender.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER

#include <beman/execution26/detail/decays_to.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Tag, typename Data, typename... Child>
    struct basic_sender
        : ::beman::execution26::detail::product_type<Tag, Data, Child...>
    {
        using sender_concept = ::beman::execution26::sender_t;
        using indices_for = ::std::index_sequence_for<Child...>;

#if 0
#if __cpp_explicit_this_parameter < 302110L
        template <typename Env>
        auto get_completion_signatures(Env&&) &&
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender&&, Env>
        {
            return {};
        }
        template <typename Env>
        auto get_completion_signatures(Env&&) const&&
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender const&&, Env>
        {
            return {};
        }
        template <typename Env>
        auto get_completion_signatures(Env&&) &
            -> ::beman::execution26::detail::completion_signatures_for<basic_sender&, Env>
        {
            return {};
        }
        template <typename Env>
        auto get_completion_signatures(Env&&) const& 
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
#endif
    };
}

// ----------------------------------------------------------------------------

#endif
