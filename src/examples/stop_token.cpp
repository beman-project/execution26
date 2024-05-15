// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include <condition_variable>
#include <mutex>
#include <print>
#include <thread>

namespace exec = beman::cpp26;

// The stop token classes are used to cancel work. Each
// stop token may be connected to a stop source on which
// cancellation can be requested using source.request_stop().
// The stop token reflects whether stop was requested on any
// source.
//
// There are two different needs on how the cancallation is
// indicated:
// - Active work, e.g., doing a lengthy computation,
//   would occassionally check the results are still needed
//   and abort the computation if it is not. To do so, the
//   function would use token.stop_requested() which yields
//   true once stopping was requested.
// - Inactive work, e.g., something waiting for input from
//   a user or a network connection is asleep and can't
//   reasonably check whether stopping was requested. Instead,
//   a callback gets registered which gets triggered when
//   stopping is requested. This callback can then cancel
//   the work. The callback is registered by creating an
//   object using the token's callback_type and deregistered
//   when this object is destroyed.
//
// The two use-cases are shown in the functions active() and
// inactive() respectively: they are run as threads from main()
// using a token associated with a stop source. Once both
// threads are started stopping is requested and the threads
// are joined.

template <typename Token>
auto active(Token token) -> void
{
    auto i{0ull};
    while (not token.stop_requested())
    {
        // do work
        ++i;
    }
    ::std::print("active thread done: {}\n" , i);
}

template <typename Token>
auto inactive(Token token) -> void
{
    ::std::condition_variable cond;
    struct Callback
    {
        ::std::condition_variable* cond;
        Callback(::std::condition_variable* cond): cond(cond) {}
        auto operator()() -> void
        {
            cond->notify_one();
        }
    };
    exec::stop_callback_for_t<Token, Callback> cb(token, &cond);
    ::std::mutex lock;

    ::std::unique_lock guard(lock);
    cond.wait(guard, [token]{ return token.stop_requested(); });
    ::std::print("inactive thread done\n");
}

auto main() -> int
{
    exec::stop_source source;
    ::std::thread act([&]{ active(source.get_token()); });
    ::std::thread inact([&]{ inactive(source.get_token()); });

    ::std::print("threads started\n");
    source.request_stop();
    ::std::print("threads cancelled\n");

    act.join();
    inact.join();
    ::std::print("done\n");
}