// include/beman/execution26/detail/basic_operation.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_OPERATION
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_OPERATION

#include <beman/execution26/detail/operation_state.hpp>
#include <beman/execution26/detail/basic_state.hpp>
#include <beman/execution26/detail/state_type.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/tag_of_t.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/connect_all_result.hpp>
#include <beman/execution26/detail/valid_specialization.hpp>
#include <functional>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Sender, typename Receiver>
        requires ::beman::execution26::detail::valid_specialization<
            ::beman::execution26::detail::state_type, Sender, Receiver
        >
    struct basic_operation
        : ::beman::execution26::detail::basic_state<Sender, Receiver>
    {
        using operation_state_concept = ::beman::execution26::operation_state_t;
        using tag_t = ::beman::execution26::tag_of_t<Sender>;

        using inner_ops_t = ::beman::execution26::detail::connect_all_result<Sender, Receiver>;
        inner_ops_t inner_ops;

        template <typename S> //-dk:TODO is that deviating from the spec?
        basic_operation(S&& sender, Receiver&& receiver) noexcept(true/*-dk:TODO*/)
            : ::beman::execution26::detail::basic_state<Sender, Receiver>(
                ::std::forward<S>(sender), ::std::move(receiver)
            )
            , inner_ops(::beman::execution26::detail::connect_all(
                this,
                ::std::forward<S>(sender),
                ::beman::execution26::detail::indices_for<Sender>()
            ))
        {
        }

        auto start() & noexcept -> void
        {
            ::std::invoke([this]<::std::size_t... I>(::std::index_sequence<I...>)
                {
                    ::beman::execution26::detail::impls_for<tag_t>::start(
                        this->state,
                        this->receiver,
                        this->inner_ops.template get<I>()...
                        );
                },
                ::std::make_index_sequence<inner_ops_t::size()>{}
            );
        }
    };
    template <typename Sender, typename Receiver>
    basic_operation(Sender&&, Receiver&&) -> basic_operation<Sender&&, Receiver>;
}

// ----------------------------------------------------------------------------

#endif
