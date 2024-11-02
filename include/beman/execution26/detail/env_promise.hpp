// include/beman/execution26/detail/env_promise.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ENV_PROMISE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ENV_PROMISE

#include <beman/execution26/detail/with_await_transform.hpp>
#include <coroutine>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Env>
struct env_promise : ::beman::execution26::detail::with_await_transform<Env> {
    auto get_return_object() noexcept -> void;
    auto initial_suspend() noexcept -> ::std::suspend_always;
    auto final_suspend() noexcept -> ::std::suspend_always;
    auto unhandled_exception() noexcept -> void;
    auto return_void() noexcept -> void;
    auto unhandled_stopped() noexcept -> ::std::coroutine_handle<>;
    auto get_env() const noexcept -> const Env&;
};
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
