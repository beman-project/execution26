// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include <condition_variable>
#include <mutex>
#include <print>
#include <thread>

namespace exec = beman::cpp26;

template <typename Token>
auto active(Token token) -> void
{
    auto i{0ull};
    while (not token.stop_requested())
        // do work
        ++i;
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