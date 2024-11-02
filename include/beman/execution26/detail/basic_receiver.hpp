// include/beman/execution26/detail/basic_receiver.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_RECEIVER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BASIC_RECEIVER

#include <beman/execution26/detail/basic_state.hpp>
#include <beman/execution26/detail/callable.hpp>
#include <beman/execution26/detail/env_type.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/receiver.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/tag_of_t.hpp>
#include <beman/execution26/detail/state_type.hpp>
#include <beman/execution26/detail/valid_specialization.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Sender, typename Receiver, typename Index>
    requires ::beman::execution26::detail::
        valid_specialization< ::beman::execution26::detail::env_type, Index, Sender, Receiver>
    struct basic_receiver {
    friend struct ::beman::execution26::get_env_t;
    friend struct ::beman::execution26::set_error_t;
    friend struct ::beman::execution26::set_stopped_t;
    friend struct ::beman::execution26::set_value_t;

    using receiver_concept                = ::beman::execution26::receiver_t;
    using tag_t                           = ::beman::execution26::tag_of_t<Sender>;
    using state_t                         = ::beman::execution26::detail::state_type<Sender, Receiver>;
    static constexpr const auto& complete = ::beman::execution26::detail::impls_for<tag_t>::complete;
    ::beman::execution26::detail::basic_state<Sender, Receiver>* op{};

  private:
    template <typename... Args>
    auto set_value(Args&&... args) && noexcept -> void
        requires ::beman::execution26::detail::
            callable<decltype(complete), Index, state_t&, Receiver&, ::beman::execution26::set_value_t, Args...>
    {
        this->complete(Index(),
                       this->op->state,
                       this->op->receiver,
                       ::beman::execution26::set_value_t(),
                       ::std::forward<Args>(args)...);
    }

    template <typename Error>
    auto set_error(Error&& error) && noexcept -> void
        requires ::beman::execution26::detail::
            callable<decltype(complete), Index, state_t&, Receiver&, ::beman::execution26::set_error_t, Error>
    {
        this->complete(Index(),
                       this->op->state,
                       this->op->receiver,
                       ::beman::execution26::set_error_t(),
                       ::std::forward<Error>(error));
    }

    auto set_stopped() && noexcept -> void
        requires ::beman::execution26::detail::
            callable<decltype(complete), Index, state_t&, Receiver&, ::beman::execution26::set_stopped_t>
    {
        this->complete(Index(), this->op->state, this->op->receiver, ::beman::execution26::set_stopped_t());
    }

    auto get_env() const noexcept -> ::beman::execution26::detail::env_type<Index, Sender, Receiver> {
        return ::beman::execution26::detail::impls_for<tag_t>::get_env(Index(), this->op->state, this->op->receiver);
    }
};
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
