// examples/intro-2-hello-async.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <tuple>

namespace ex = ::beman::execution26;
using namespace std::string_literals;
using namespace std::chrono_literals;

#include <queue>
#include <tuple>
#include <poll.h>

namespace demo {
struct timer {
    struct state_base {
        virtual void complete() = 0;
    };
    template <typename Receiver>
    struct state : state_base {
        using operation_state_concept = ex::operation_state_t;
        timer*                        self;
        std::remove_cvref_t<Receiver> receiver;
        std::chrono::milliseconds     ms;

        template <typename R>
        state(timer* self, R&& receiver, std::chrono::milliseconds ms)
            : state_base(), self(self), receiver(std::forward<R>(receiver)), ms(ms) {}
        void start() & noexcept { self->add(ms, this); }
        void complete() override { ex::set_value(std::move(receiver)); }
    };
    struct sender {
        using sender_concept        = ex::sender_t;
        using completion_signatures = ex::completion_signatures<ex::set_value_t()>;
        timer*                    self;
        std::chrono::milliseconds ms;
        template <typename R>
        state<R> connect(R&& r) {
            return state<R>(self, std::forward<R>(r), ms);
        }
    };
    using time_point = std::chrono::system_clock::time_point;
    using value_type = std::tuple<time_point, state_base*>;
    std::priority_queue<value_type, std::vector<value_type>, std::greater<>> outstanding;

    void add(std::chrono::milliseconds ms, state_base* base) {
        outstanding.emplace(std::chrono::system_clock::now() + ms, base);
    }
    bool run_one() {
        if (outstanding.empty())
            return false;
        auto [time, base] = outstanding.top();
        outstanding.pop();
        auto now{std::chrono::system_clock::now()};
        if (now < time) {
            auto dur{time - now};
            auto ms{std::chrono::duration_cast<std::chrono::milliseconds>(dur)};
            poll(nullptr, {}, int(ms.count()));
        }
        base->complete();
        return true;
    }

    template <typename Receiver>
    struct run_state
    {
        using operation_state_concept = ex::operation_state_t;
        using scheduler = decltype(ex::get_delegation_scheduler(ex::get_env(std::declval<Receiver&>())));

        timer* self;
        Receiver receiver;
        void start() & noexcept {
            std::cout << "run start\n";
        }
    };
    struct run_sender
    {
        using sender_concept = ex::sender_t;
        using completion_signatures = ex::completion_signatures<ex::set_value_t()>;

        timer* self;

        template <typename Receiver>
        run_state<std::remove_cvref_t<Receiver>> connect(Receiver&& receiver)
        {
            return { self, std::forward<Receiver>(receiver) };
        }
    };

    auto run() {
#if 0
        return run_sender{this};
#else
        return ex::then(ex::just(), [this] {
            while (this->run_one())
                ;
        });
#endif
    }

    template <typename T>
    auto resume_after(std::chrono::duration<T> d) {
        auto ms(std::chrono::duration_cast<std::chrono::milliseconds>(d));
        return sender{this, ms};
    }
};
} // namespace demo

// ----------------------------------------------------------------------------
// Please see the explanation in docs/intro-examples.md for an explanation.

struct receiver {
    using receiver_concept = ex::receiver_t;
    void set_value(auto&&...) noexcept {}
    void set_error(auto&&) noexcept {}
    void set_stopped() noexcept {}
};
static_assert(ex::receiver<receiver>);

int main() {
    std::cout << std::unitbuf;
    demo::timer timer;

    // clang-format off
    auto [result] = ex::sync_wait(
        ex::when_all(
            ex::when_all(
                timer.resume_after(3s)
                    | ex::then([] { std::cout << "h\n"; return std::string("hello"); }),
                timer.resume_after(1s)
                    | ex::then([] { std::cout << ",\n"; return std::string(", "); }),
                timer.resume_after(2s)
                    | ex::then([] { std::cout << "w\n"; return std::string("world"); })
            ) | ex::then([](auto s1, auto s2, auto s3) { return s1 + s2 + s3; }),
            timer.run()
        )
    ).value_or(std::tuple(std::string("")));
    // clang-format on

    std::cout << result << "\n";
}