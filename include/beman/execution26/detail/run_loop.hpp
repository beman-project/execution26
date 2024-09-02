// include/beman/execution26/detail/run_loop.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_RUN_LOOP
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_RUN_LOOP

#include <beman/execution26/detail/completion_signatures.hpp>
#include <beman/execution26/detail/get_completion_scheduler.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/get_stop_token.hpp>
#include <beman/execution26/detail/operation_state.hpp>
#include <beman/execution26/detail/scheduler.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>

#include <exception>
#include <mutex>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    class run_loop
    {
    private:
        struct scheduler;

        struct env
        {
            run_loop* loop;

            template <typename Completion>
            auto query(::beman::execution26::get_completion_scheduler_t<Completion> const&) const
                noexcept
                -> scheduler
            {
                return {this->loop};
            }
        };
        struct opstate_base
        {
            opstate_base* next;
            virtual auto execute() noexcept -> void = 0;
        };
        template <typename Receiver>
        struct opstate
            : opstate_base
        {
            using operation_state_concept = ::beman::execution26::operation_state_t;

            run_loop* loop;
            Receiver   receiver;

            template <typename R>
            opstate(run_loop* loop, R&& receiver)
                : loop(loop)
                , receiver(::std::forward<Receiver>(receiver))
            {
            }
            opstate(opstate&&) = delete;
            auto start() & noexcept -> void
            {
                try
                {
                    this->loop->push_back(this);
                }
                catch(...)
                {
                    ::beman::execution26::set_error(
                        ::std::move(this->receiver),
                        ::std::current_exception());
                }
                
            }
            auto execute() noexcept -> void override
            {
                if (::beman::execution26::get_stop_token(
                        ::beman::execution26::get_env(this->receiver)
                    ).stop_requested())
                    ::beman::execution26::set_stopped(::std::move(this->receiver));
                else
                    ::beman::execution26::set_value(::std::move(this->receiver));
            }
        };
        struct sender
        {
            using sender_concept = ::beman::execution26::sender_t;
            using completion_signatures = ::beman::execution26::completion_signatures<
                    ::beman::execution26::set_value_t(),
                    ::beman::execution26::set_error_t(::std::exception_ptr),
                    ::beman::execution26::set_stopped_t()
                >;
            
            run_loop* loop;

            auto get_env() const noexcept -> env { return {this->loop}; }
            template <typename Receiver>
            auto connect(Receiver&& receiver) noexcept
                -> opstate<::std::decay_t<Receiver>>
            {
                return {this->loop, ::std::forward<Receiver>(receiver)};
            }
        };
        struct scheduler
        {
            using scheduler_concept = ::beman::execution26::scheduler_t;

            run_loop* loop;

            auto schedule() noexcept -> sender { return {this->loop}; }
            auto operator== (scheduler const&) const -> bool = default;
        };

        enum class state { starting, running, finishing };

        state                     current_state{state::starting};
        ::std::mutex              mutex{};
        ::std::condition_variable condition{};
        opstate_base*             front{};
        opstate_base*             back{};

        auto push_back(opstate_base* item) -> void
        {
            ::std::lock_guard guard(this->mutex);
            if (auto previous_back{::std::exchange(this->back, item)})
            {
                previous_back->next = item;
            }
            else
            {
                this->front = item;
                this->condition.notify_one();
            }
        }
        auto pop_front() -> opstate_base*
        {
            ::std::unique_lock guard(this->mutex);
            this->condition.wait(guard,
                [this]{ return this->front || this->current_state == state::finishing; });
            return this->front? ::std::exchange(this->front, this->front->next): nullptr;
        }

    public:
        run_loop() noexcept = default;
        run_loop(run_loop&&) = delete;
        ~run_loop()
        {
            ::std::lock_guard guard(this->mutex);
            {
                if (this->front != nullptr || this->current_state == state::running)
                    ::std::terminate();
            }
        }

        auto get_scheduler() -> scheduler { return {this}; }

        auto run() -> void
        {
            {
                ::std::lock_guard guard(this->mutex);
                auto current{::std::exchange(this->current_state, state::running)};
                if (state::starting != current)
                {
                    ::std::terminate();
                }
            }

            while (auto* op{this->pop_front()})
            {
                op->execute();
            }
        }
        auto finish() -> void
        {
            {
                ::std::lock_guard guard(this->mutex);
                this->current_state = state::finishing;
            }
            this->condition.notify_one();
        }
    };
}

// ----------------------------------------------------------------------------

#endif