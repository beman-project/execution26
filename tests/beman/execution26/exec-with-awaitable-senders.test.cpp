// src/beman/execution26/tests/exec-with-awaitable-senders.test.cpp   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/with_awaitable_senders.hpp>

#include <beman/execution26/execution.hpp>

#include <test/execution.hpp>

#include <coroutine>

// This examples shows how to await on senders
// but also how to use awaitables with sender algorithms

namespace exec = beman::execution26;

struct promise;

struct awaitable {
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) { h.resume(); }
    int  await_resume() { return 1; }
};

struct void_awaitable {
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) { h.resume(); }
    void await_resume() {}
};

struct coroutine : std::coroutine_handle<promise> {
    using promise_type = ::promise;
};

struct promise : exec::with_awaitable_senders<promise> {
    coroutine           get_return_object() { return {coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_never  final_suspend() noexcept { return {}; }
    void                return_void() {}
    void                unhandled_exception() {}
};

coroutine test_await_tuple() {
    auto [just1, just2] = co_await exec::when_all(exec::just(42), exec::just(42));
    ASSERT(just1 == 42);
    ASSERT(just2 == 42);
}

coroutine test_await_void() { co_await exec::just(); }

void test_sync_wait_awaitable() {
    try {
        auto [v] = exec::sync_wait(awaitable{}).value();
        ASSERT(v == 1);
    } catch (...) {
        ASSERT(false);
    }
}

void test_sync_wait_void_awaitable() {
    try {
        ASSERT(exec::sync_wait(void_awaitable{}));
    } catch (...) {
        ASSERT(false);
    }
}

coroutine test_mix_awaitable_and_sender() {
    auto [just, value] = co_await exec::when_all(exec::just(0), awaitable{});
    ASSERT(just == 0);
    ASSERT(value == 1);
}

TEST(exec_with_awaitable_senders) {
    test_await_tuple().resume();
    test_await_void().resume();
    test_sync_wait_awaitable();
    test_sync_wait_void_awaitable();
    test_mix_awaitable_and_sender().resume();
}