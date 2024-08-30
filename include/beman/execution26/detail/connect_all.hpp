// include/beman/execution26/detail/connect_all.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_ALL
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_ALL

#include <beman/execution26/detail/connect_result_t.hpp>
#include <beman/execution26/detail/connect.hpp>
#include <beman/execution26/detail/basic_receiver.hpp>
#include <beman/execution26/detail/basic_state.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <cstddef>
#include <tuple>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct connect_all_t
    {
        auto use(auto&&) const {}
        template <typename Sender, typename Receiver, ::std::size_t... I>
        auto operator()(::beman::execution26::detail::basic_state<Sender, Receiver>* op,
                        Sender&& sender,
                        ::std::index_sequence<I...>) const noexcept(true/*-dk:TODO*/)
        {
            using sender_type = ::std::remove_cvref_t<Sender>;
            static constexpr ::beman::execution26::detail::sender_any_t at{};
            if constexpr (requires(){ sender_type{ at, at, at, at, at, at }; })
            {
                auto&& [tag, data, c0, c1, c2, c3] = sender;
                use(tag);
                use(data);
                return ::beman::execution26::detail::product_type{
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c0),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 0>>{op}),
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c1),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 1>>{op}),
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c2),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 2>>{op}),
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c3),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 3>>{op})
                };
            }
            else if constexpr (requires(){ sender_type{ at, at, at, at, at }; })
            {
                auto&& [tag, data, c0, c1, c2] = sender;
                use(tag);
                use(data);
                return ::beman::execution26::detail::product_type{
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c0),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 0>>{op}),
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c1),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 1>>{op}),
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c2),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 2>>{op})
                };
            }
            else if constexpr (requires(){ sender_type{ at, at, at, at }; })
            {
                auto&& [tag, data, c0, c1] = sender;
                use(tag);
                use(data);
                return ::beman::execution26::detail::product_type{
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c0),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 0>>{op}),
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c1),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 1>>{op})
                };
            }
            else if constexpr (requires(){ sender_type{ at, at, at }; })
            {
                auto&& [tag, data, c0] = sender;
                use(tag);
                use(data);
                return ::beman::execution26::detail::product_type{
                    ::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c0),
                        ::beman::execution26::detail::basic_receiver<Sender, Receiver,
                            ::std::integral_constant<::size_t, 0>>{op})
                };
            }
            else if constexpr (requires(){ sender_type{ at, at }; })
            {
                auto&& [tag, data] = sender;
                use(tag);
                use(data);
                return ::beman::execution26::detail::product_type{
                };
            }
        }
    };
    inline constexpr connect_all_t connect_all{};
}

// ----------------------------------------------------------------------------

#endif
