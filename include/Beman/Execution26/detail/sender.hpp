// include/Beman/Execution26/detail/sender.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER

#include <Beman/Execution26/detail/empty_env.hpp>
#include <Beman/Execution26/detail/get_env.hpp>
#include <Beman/Execution26/detail/queryable.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct sender_t {};
}
namespace Beman::Execution26::Detail
{
    template <typename Sender>
    concept is_sender
        = ::std::derived_from<typename Sender::sender_concept, ::Beman::Execution26::sender_t>
        ;

    template <typename Sender>
    concept enable_sender
        =  ::Beman::Execution26::Detail::is_sender<Sender>
        //-dk:TODO || ::Beman::Execution26::Detail::is_awaitable<Sender,
        //    ::Beman::Execution::Detail::env_promise<::Beman::Execution::empty_env>>
        ;
}
namespace Beman::Execution26
{
    template <typename Sender>
    concept sender
        =  ::Beman::Execution26::Detail::enable_sender<::std::remove_cvref_t<Sender>>
        && requires(::std::remove_cvref_t<Sender> const& sndr)
        {
            { ::Beman::Execution26::get_env(sndr) } -> ::Beman::Execution26::Detail::queryable;
        }
        && ::std::move_constructible<::std::remove_cvref_t<Sender>>
        && ::std::constructible_from<::std::remove_cvref_t<Sender>, Sender>
        ;
}

// ----------------------------------------------------------------------------

#endif
