// include/beman/execution26/detail/sender_adaptor.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_ADAPTOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_ADAPTOR

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_adaptor_closure.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Adaptor, typename T0, typename... T>
    struct sender_adaptor
        : ::beman::execution26::detail::product_type<
            ::std::decay_t<Adaptor>, ::std::decay_t<T0>, ::std::decay_t<T>...>
        , ::beman::execution26::sender_adaptor_closure<sender_adaptor<Adaptor, T0, T...>>
    {
        template <::beman::execution26::sender Sender>
        static auto apply(Sender&& sender, auto&& self)
        {
            using base_type = ::beman::execution26::detail::product_type<
                ::std::decay_t<Adaptor>, ::std::decay_t<T0>, ::std::decay_t<T>...>;
            static constexpr ::beman::execution26::detail::sender_any_t at{};
            if constexpr (requires{ base_type{ at, at, at, at }; })
            {
                auto&&[adaptor, arg0, arg1, arg2] = self;
                return adaptor(::std::forward<Sender>(sender), arg0, arg1, arg2);
            }
            if constexpr (requires{ base_type{ at, at, at }; })
            {
                auto&&[adaptor, arg0, arg1] = self;
                return adaptor(::std::forward<Sender>(sender), arg0, arg1);
            }
            else if constexpr (requires{ base_type{ at, at }; })
            {
                auto&&[adaptor, arg0] = self;
                return adaptor(::std::forward<Sender>(sender), arg0);
            }
        }
        template <::beman::execution26::sender Sender>
        auto operator()(Sender&& sender)
        {
            return apply(::std::forward<Sender>(sender), *this);
        }
        template <::beman::execution26::sender Sender>
        auto operator()(Sender&& sender) const
        {
            return apply(::std::forward<Sender>(sender), *this);
        }
    };
}

// ----------------------------------------------------------------------------

#endif
