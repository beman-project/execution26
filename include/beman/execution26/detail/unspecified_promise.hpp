// include/beman/execution26/detail/unspecified_promise.hpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_UNSPECIFIED_PROMISE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_UNSPECIFIED_PROMISE

#include <coroutine>

namespace beman::execution26::detail {
struct unspecified_promise {
    auto get_return_object() noexcept -> unspecified_promise;
    auto initial_suspend() noexcept -> unspecified_promise;
    auto final_suspend() noexcept -> unspecified_promise;
    void unhandled_exception() noexcept;
    void return_void() noexcept;
    auto unhandled_stopped() noexcept -> ::std::coroutine_handle<>;
};
} // namespace beman::execution26::detail

#endif