// include/beman/execution26/detail/completion_signatures_for.hpp     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES_FOR

#include <beman/execution26/detail/sender_in.hpp>
#include <beman/execution26/detail/get_completion_signatures.hpp>
#include <functional>
#include <type_traits>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct no_completion_signatures_defined_in_sender {};

    template <typename Sender, typename Env>
    struct completion_signatures_for_impl
    {
        using type = ::beman::execution26::detail::no_completion_signatures_defined_in_sender;
    };
    #if 0
    {
        using type = decltype(::std::invoke([]{
            if constexpr (::beman::execution26::sender_in<Sender, Env>)
                return decltype(::beman::execution26::get_completion_signatures(
                    ::std::declval<Sender>(), ::std::declval<Env>())
                ){};
            else
                return ::beman::execution26::detail::no_completion_signatures_defined_in_sender{};
        }));
    };
    #endif

    template <typename Sender, typename Env>
    using completion_signatures_for
        = ::std::conditional_t<
            ::std::same_as<
                beman::execution26::detail::no_completion_signatures_defined_in_sender,
                    typename ::beman::execution26::detail::completion_signatures_for_impl<
                        Sender, Env>::type
                >,
            typename ::beman::execution26::detail::completion_signatures_for_impl<
                ::std::remove_cvref_t<Sender>, Env>::type,
            typename ::beman::execution26::detail::completion_signatures_for_impl<
                Sender, Env>::type
        >;
}

// ----------------------------------------------------------------------------

#endif
