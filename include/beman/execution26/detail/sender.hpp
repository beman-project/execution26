// include/beman/execution26/detail/sender.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER

#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/queryable.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct sender_t {};
}
namespace beman::execution26::detail
{
    template <typename Sender>
    concept is_sender
        = ::std::derived_from<typename Sender::sender_concept, ::beman::execution26::sender_t>
        ;

    template <typename Sender>
    concept enable_sender
        =  ::beman::execution26::detail::is_sender<Sender>
        //-dk:TODO || ::beman::execution26::detail::is_awaitable<Sender,
        //    ::beman::Execution::detail::env_promise<::beman::Execution::empty_env>>
        ;
}
namespace beman::execution26
{
    template <typename Sender>
    concept sender
        =  ::beman::execution26::detail::enable_sender<::std::remove_cvref_t<Sender>>
        && requires(::std::remove_cvref_t<Sender> const& sndr)
        {
            { ::beman::execution26::get_env(sndr) } -> ::beman::execution26::detail::queryable;
        }
        && ::std::move_constructible<::std::remove_cvref_t<Sender>>
        && ::std::constructible_from<::std::remove_cvref_t<Sender>, Sender>
        ;
}

// ----------------------------------------------------------------------------

#endif
