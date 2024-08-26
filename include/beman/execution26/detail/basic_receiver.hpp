// include/Beman/Execution26/detail/basic_receiver.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_RECEIVER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_RECEIVER

#include <Beman/Execution26/detail/basic_state.hpp>
#include <Beman/Execution26/detail/callable.hpp>
#include <Beman/Execution26/detail/env_type.hpp>
#include <Beman/Execution26/detail/impls_for.hpp>
#include <Beman/Execution26/detail/receiver.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <Beman/Execution26/detail/state_type.hpp>
#include <Beman/Execution26/detail/valid_specialization.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Sender, typename Receiver, typename Index>
        requires ::Beman::Execution26::Detail::valid_specialization<
            ::Beman::Execution26::Detail::env_type, Index, Sender, Receiver
        >
    struct basic_receiver
    {
        using receiver_concept = ::Beman::Execution26::receiver_t;
        using tag_t = ::Beman::Execution26::tag_of_t<Sender>;
        using state_t = ::Beman::Execution26::Detail::state_type<Sender, Receiver>;
        static constexpr const auto& complete = ::Beman::Execution26::Detail::impls_for<tag_t>::complete;

        template <typename... Args>
        auto set_value(Args&&... args) && noexcept -> void
            requires ::Beman::Execution26::Detail::callable<
                decltype(complete),
                Index,
                state_t&,
                Receiver&,
                ::Beman::Execution26::set_value_t,
                Args...>
        {
            this->complete(Index(), this->op->state, this->op->receiver,
                           ::Beman::Execution26::set_value_t(), ::std::forward<Args>(args)...);
        }

        template <typename Error>
        auto set_error(Error&& error) && noexcept -> void
            requires ::Beman::Execution26::Detail::callable<
                decltype(complete),
                Index,
                state_t&,
                Receiver&,
                ::Beman::Execution26::set_error_t,
                Error>
        {
            this->complete(Index(), this->op->state, this->op->receiver,
                           ::Beman::Execution26::set_error_t(), ::std::forward<Error>(error));
        }

        auto set_stopped() && noexcept -> void
            requires ::Beman::Execution26::Detail::callable<
                decltype(complete),
                Index,
                state_t&,
                Receiver&,
                ::Beman::Execution26::set_stopped_t>
        {
            this->complete(Index(), this->op->state, this->op->receiver,
                           ::Beman::Execution26::set_stopped_t());
        }

        ::Beman::Execution26::Detail::basic_state<Sender, Receiver>* op{};
    };  
}

// ----------------------------------------------------------------------------

#endif
