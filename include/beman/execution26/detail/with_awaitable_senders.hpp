// include/beman/execution26/detail/with_awaitable_senders.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_WITH_AWAITABLE_SENDERS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_WITH_AWAITABLE_SENDERS

#include <beman/execution26/detail/as_awaitable.hpp>
#include <beman/execution26/detail/class_type.hpp>
#include <beman/execution26/detail/call_result_t.hpp>

#include <coroutine>

namespace beman::execution26 {
template <::beman::execution26::detail::class_type Promise>
struct with_awaitable_senders {
    template <class OtherPromise>
        requires(!::std::same_as<OtherPromise, void>)
    void set_continuation(::std::coroutine_handle<OtherPromise> h) noexcept {
        contination_ = h;
        if constexpr (requires(OtherPromise& other) { other.unhandled_stopped(); }) {
            stopped_handler_ = [](void* p) noexcept -> ::std::coroutine_handle<> {
                return ::std::coroutine_handle<OtherPromise>::from_address(p).promise().unhandled_stopped();
            };
        } else {
            stopped_handler_ = &default_unhandled_stopped;
        }
    }

    ::std::coroutine_handle<> continuation() const noexcept { return contination_; }

    ::std::coroutine_handle<> unhandled_stopped() noexcept { return stopped_handler_(contination_.address()); }

    template <class Value>
    auto await_transform(Value&& value)
        -> ::beman::execution26::detail::call_result_t<::beman::execution26::as_awaitable_t, Value, Promise&> {
        return ::beman::execution26::as_awaitable(::std::forward<Value>(value), static_cast<Promise&>(*this));
    }

  private:
    friend Promise;
    with_awaitable_senders() = default;

    [[noreturn]] static auto default_unhandled_stopped(void*) noexcept -> ::std::coroutine_handle<> {
        ::std::terminate();
    }

    ::std::coroutine_handle<> contination_{};
    ::std::coroutine_handle<> (*stopped_handler_)(void*) noexcept = &default_unhandled_stopped;
};

} // namespace beman::execution26

#endif