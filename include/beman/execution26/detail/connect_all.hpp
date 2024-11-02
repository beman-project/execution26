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

#include <beman/execution26/detail/suppress_push.hpp>

namespace beman::execution26::detail {
struct connect_all_t {
    static auto use(auto&&...) {}
    //-dk:TODO is the S parameter deviating from the spec?
    template <typename Sender, typename S, typename Receiver, ::std::size_t... I>
    auto operator()(::beman::execution26::detail::basic_state<Sender, Receiver>* op,
                    S&&                                                          sender,
                    ::std::index_sequence<I...>) const noexcept(true /*-dk:TODO*/) {
        auto data{::beman::execution26::detail::get_sender_data(::std::forward<S>(sender))};
        return ::std::apply(
            [&op](auto&&... c) {
                return [&op]<::std::size_t... J>(::std::index_sequence<J...>, auto&&... c) {
                    use(op);
                    return ::beman::execution26::detail::product_type{::beman::execution26::connect(
                        ::beman::execution26::detail::forward_like<Sender>(c),
                        ::beman::execution26::detail::
                            basic_receiver<Sender, Receiver, ::std::integral_constant<::size_t, J>>{op})...};
                }(::std::make_index_sequence<::std::tuple_size_v<::std::decay_t<decltype(data.children)>>>{}, c...);
            },
            data.children);
    }
};
inline constexpr connect_all_t connect_all{};
} // namespace beman::execution26::detail

#include <beman/execution26/detail/suppress_pop.hpp>

// ----------------------------------------------------------------------------

#endif
