// include/beman/execution26/detail/basic_sender.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_SENDER

#include <beman/execution26/detail/basic_operation.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/decays_to.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
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

        auto get_env() const noexcept -> decltype(auto)
        {
            auto data{::beman::execution26::detail::get_sender_data(*this)};
            return ::std::apply([&data](auto&&... c){
                return ::beman::execution26::detail::impls_for<Tag>
                    ::get_attrs(data.data, c...);
            }, data.children);
        }

#if __cpp_explicit_this_parameter < 202110L
        template <::beman::execution26::receiver Receiver>
        auto connect(Receiver receiver) &
            noexcept(true/*-dk:TODO*/)
            -> ::beman::execution26::detail::basic_operation<basic_sender&, Receiver>
        {
            return { *this, ::std::move(receiver) };
        }
        template <::beman::execution26::receiver Receiver>
        auto connect(Receiver receiver) const&
            noexcept(true/*-dk:TODO*/)
            -> ::beman::execution26::detail::basic_operation<basic_sender const&, Receiver>
        {
            return { *this, ::std::move(receiver) };
        }
        template <::beman::execution26::receiver Receiver>
        auto connect(Receiver receiver) &&
            noexcept(true/*-dk:TODO*/)
            -> ::beman::execution26::detail::basic_operation<basic_sender, Receiver>
        {
            return { ::std::move(*this), ::std::move(receiver) };
        }
#else
        template <::beman::execution26::detail::decays_to<basic_sender> Self,
                  ::beman::execution26::receiver Receiver>
        auto connect(this Self&& self, Receiver receiver)
            noexcept(true/*-dk:TODO*/)
            -> ::beman::execution26::detail::basic_operation<basic_sender, Receiver>
        {
            return { ::std::forward<Self>(self), ::std::move(receiver) };
        }
#endif
#if __cpp_explicit_this_parameter < 202110L
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
    };
}

// ----------------------------------------------------------------------------

#endif