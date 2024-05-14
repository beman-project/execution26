// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace exec = beman::cpp26;

template <typename Token>
auto active(Token token, auto& outlock) -> void
{
    auto i{0ull};
    while (not token.stop_requested())
        // do work
        ++i;
    std::lock_guard guard(outlock);
    ::std::cout << "active done: i=" << i << "\n";
}

template <typename Token>
auto inactive(Token token, auto& outlock) -> void
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
    std::lock_guard oguard(outlock);
    std::cout << "inactive done\n";
}

auto main() -> int
{
    exec::stop_source source;
    std::mutex        outlock;
    ::std::thread act([&]{ active(source.get_token(), outlock); });
    ::std::thread inact([&]{ inactive(source.get_token(), outlock); });


    { std::lock_guard guard(outlock); ::std::cout << "threads started\n"; }

    source.request_stop();

    { std::lock_guard guard(outlock); ::std::cout << "threads cancelled\n"; }
    act.join();
    inact.join();
    { std::lock_guard guard(outlock); ::std::cout << "done\n"; }
}