// examples/when_all-cancel.cpp                                       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <beman/execution26/stop_token.hpp>
#include <iostream>
#include <optional>
#include <type_traits>
#include <utility>

namespace ex = beman::execution26;

// ----------------------------------------------------------------------------

namespace
{
    struct env
    {
        ex::inplace_stop_source* source;
        auto query(ex::get_stop_token_t const&) const noexcept
            -> ex::inplace_stop_token
        {
            std::cout << "query\n";
            return this->source->get_token();
        }
    };

    struct receiver
    {
        using receiver_concept = ex::receiver_t;
        ex::inplace_stop_source* source;

        auto set_value() && noexcept -> void { std::cout << "set_value\n"; }
        auto set_error(auto&&) && noexcept -> void { std::cout << "set_error\n"; }
        auto set_stopped() && noexcept -> void { std::cout << "set_stopped\n"; }

        auto get_env() const noexcept { return env{this->source}; }
    };
    static_assert(ex::receiver<receiver>);

    struct await_stop
    {
        using sender_concept = ex::sender_t;
        using completion_signatures = ex::completion_signatures<
            ex::set_value_t(),
            ex::set_stopped_t()
        >;

        template <ex::receiver Receiver>
        struct state
        {
            struct stop_t
            {
                state* st;
                auto operator()() const noexcept -> void
                {
                    auto local_st{this->st};
                    local_st->callback.reset();
                    ex::set_stopped(std::move(local_st->receiver));
                }
            };
            using operation_state_concept = ex::operation_state_t;
            using token_t = decltype(ex::get_stop_token(ex::get_env(std::declval<Receiver>())));
            using callback_t = ex::stop_callback_for_t<token_t, stop_t>;

            Receiver                   receiver;
            std::optional<callback_t>  callback{};

            auto start() & noexcept -> void
            {
                callback.emplace(ex::get_stop_token(ex::get_env(this->receiver)), stop_t(this));
            }
        };

        template <ex::receiver Receiver>
        auto connect(Receiver&& receiver) -> state<std::remove_cvref_t<Receiver>>
        {
            return { ::std::forward<Receiver>(receiver) };
        }
    };
    static_assert(ex::sender_in<await_stop>);
    static_assert(ex::operation_state<await_stop::state<receiver>>);
}

auto main() -> int
{
    auto s{ex::when_all(await_stop{})};

    ex::inplace_stop_source source{};
    auto op{ex::connect(s, receiver{&source})};
    std::cout << "start\n";
    ex::start(op);
    std::cout << "started\n";
    source.request_stop();
    std::cout << "done\n";
}
