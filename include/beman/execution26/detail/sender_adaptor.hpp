// include/beman/execution26/detail/sender_adaptor.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_ADAPTOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_ADAPTOR

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_adaptor_closure.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/forward_like.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Adaptor, typename... T>
    struct sender_adaptor
        : ::beman::execution26::detail::product_type<
            ::std::decay_t<Adaptor>, ::std::decay_t<T>...>
        , ::beman::execution26::sender_adaptor_closure<sender_adaptor<Adaptor, T...>>
    {
        template <::beman::execution26::sender Sender, typename Self>
        static auto apply(Sender&& sender, Self&& self)
        {
            return [&self, &sender]<::std::size_t... I>(::std::index_sequence<I...>){
                auto&& fun(self.template get<0>());
                return fun(
                    ::std::forward<Sender>(sender),
                    ::beman::execution26::detail::forward_like<Self>(self.template get<I + 1>())...
                );
            }(::std::make_index_sequence<sender_adaptor::size() - 1u>{});
        }
        template <::beman::execution26::sender Sender>
        auto operator()(Sender&& sender)
        {
            return apply(::std::forward<Sender>(sender), ::std::move(*this));
        }
        template <::beman::execution26::sender Sender>
        auto operator()(Sender&& sender) const
        {
            return apply(::std::forward<Sender>(sender), *this);
        }
    };
    template <typename... T>
    sender_adaptor(T&&...) -> sender_adaptor<T...>;
}

// ----------------------------------------------------------------------------

#endif
