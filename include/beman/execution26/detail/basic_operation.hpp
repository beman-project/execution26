// include/beman/execution26/detail/basic_operation.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_OPERATION
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_OPERATION

#include <beman/execution26/detail/operation_state.hpp>
#include <beman/execution26/detail/basic_state.hpp>
#include <beman/execution26/detail/state_type.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/connect_all_result.hpp>
#include <beman/execution26/detail/valid_specialization.hpp>
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

        basic_operation(Sender&& sender, Receiver&& receiver) noexcept(true/*-dk:TODO*/)
            : ::beman::execution26::detail::basic_state<Sender, Receiver>(
                ::std::forward<Sender>(sender), ::std::move(receiver)
            )
            , inner_ops(::beman::execution26::detail::connect_all(
                this,
                ::std::forward<Sender>(sender),
                ::beman::execution26::detail::indices_for<Sender>()
            ))
        {
        }

        auto start() & noexcept -> void
        {
            if constexpr (requires(){
                []{ auto&&[op0, op1, op2, op3] = *static_cast<inner_ops_t*>(nullptr); };
                })
            {
                auto&[op0, op1, op2, op3] = this->inner_ops;
                ::beman::execution26::detail::impls_for<tag_t>::start(this->state, this->receiver, op0, op1, op2, op3);
            }
            else if constexpr (requires(){
                []{ auto&&[op0, op1, op2] = *static_cast<inner_ops_t*>(nullptr); };
                })
            {
                auto&[op0, op1, op2] = this->inner_ops;
                ::beman::execution26::detail::impls_for<tag_t>::start(this->state, this->receiver, op0, op1, op2);
            }
            else if constexpr (requires(){
                []{ auto&&[op0, op1] = *static_cast<inner_ops_t*>(nullptr); };
                })
            {
                auto&[op0, op1] = this->inner_ops;
                ::beman::execution26::detail::impls_for<tag_t>::start(this->state, this->receiver, op0, op1);
            }
            else if constexpr (requires(){
                []{ auto&&[op0] = *static_cast<inner_ops_t*>(nullptr); };
                })
            {
                auto&[op0] = this->inner_ops;
                ::beman::execution26::detail::impls_for<tag_t>::start(this->state, this->receiver, op0);
            }
        }
    };
}

// ----------------------------------------------------------------------------

#endif