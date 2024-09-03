// include/beman/execution26/detail/sync_wait.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SYNC_WAIT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SYNC_WAIT

#include <beman/execution26/detail/sender_in.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/get_scheduler.hpp>
#include <beman/execution26/detail/get_delegation_scheduler.hpp>
#include <beman/execution26/detail/apply_sender.hpp>
#include <beman/execution26/detail/connect.hpp>
#include <beman/execution26/detail/start.hpp>
#include <beman/execution26/detail/run_loop.hpp>
#include <beman/execution26/detail/receiver.hpp>
#include <exception>
#include <optional>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{}
namespace bed = beman::execution26::detail;
namespace be = beman::execution26;

namespace beman::execution26::detail
{
    struct sync_wait_env
    {
        ::beman::execution26::run_loop* loop;

        auto query(::beman::execution26::get_scheduler_t) const noexcept
        {
            return this->loop->get_scheduler();
        }
        auto query(::beman::execution26::get_delegation_scheduler_t) const noexcept
        {
            return this->loop->get_scheduler();
        }
    };

    template <typename Sender>
    struct sync_wait_state
    {
        be::run_loop loop{};
        ::std::exception_ptr error{};
        ::std::optional<int> result{};
    };

    template <typename Sender>
    struct sync_wait_receiver
    {
        using receiver_concept = be::receiver_t;

        bed::sync_wait_state<Sender>* state{};

        auto set_error(auto&&) && noexcept -> void
        {
            /*-dk:TODO set state->error */
            this->state->loop.finish();
        }
        auto set_stopped() && noexcept -> void
        {
            this->state->loop.finish();
        }
        auto set_value(auto&&...) && noexcept -> void
        {
            /*-dk:TODO set state->result */
            this->state->loop.finish();
        }

    };

    struct sync_wait_t
    {
        template <typename Sender>
        auto apply_sender(Sender&& sender)
        {
            bed::sync_wait_state<Sender> state;
            auto op{be::connect(::std::forward<Sender>(sender),
                                bed::sync_wait_receiver<Sender>{&state})};
            be::start(op);

            state.loop.run();
            if (state.error)
            {
                ::std::rethrow_exception(state.error);
            }
            return ::std::move(state.result);
        }

        template <::beman::execution26::sender_in<
            ::beman::execution26::detail::sync_wait_env> Sender>
        auto operator()(Sender&& sender) const
        {
            auto domain{::beman::execution26::detail::get_domain_early(sender)};
            return ::beman::execution26::apply_sender(domain, *this, ::std::forward<Sender>(sender));
        }
    };
}

namespace beman::execution26
{
    using sync_wait_t = ::beman::execution26::detail::sync_wait_t;
    inline constexpr ::beman::execution26::sync_wait_t sync_wait{};
}

// ----------------------------------------------------------------------------

#endif
