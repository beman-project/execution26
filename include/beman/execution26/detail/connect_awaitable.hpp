// include/beman/execution26/detail/connect_awaitable.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_AWAITABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CONNECT_AWAITABLE

#include <beman/execution26/detail/await_result_type.hpp>
#include <beman/execution26/detail/completion_signatures.hpp>
#include <beman/execution26/detail/operation_state_task.hpp>
#include <beman/execution26/detail/receiver.hpp>
#include <beman/execution26/detail/receiver_of.hpp>
#include <beman/execution26/detail/suspend_complete.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>

#include <concepts>
#include <exception>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename T>
struct awaiter_set_value {
    using type = ::beman::execution26::set_value_t(T);
};
template <>
struct awaiter_set_value<void> {
    using type = ::beman::execution26::set_value_t();
};

template <typename Awaiter, typename Receiver>
using awaiter_completion_signatures = ::beman::execution26::completion_signatures<
    typename ::beman::execution26::detail::awaiter_set_value< ::beman::execution26::detail::await_result_type<
        Awaiter,
        ::beman::execution26::detail::connect_awaitable_promise<Receiver> > >::type,
    ::beman::execution26::set_error_t(::std::exception_ptr),
    ::beman::execution26::set_stopped_t()>;

template <typename Awaiter, ::beman::execution26::receiver Receiver>
auto connect_awaitable(Awaiter awaiter, Receiver receiver)
    -> ::beman::execution26::detail::operation_state_task<Receiver>
    requires ::beman::execution26::
        receiver_of<Receiver, ::beman::execution26::detail::awaiter_completion_signatures<Awaiter, Receiver> >
{
    // NOTE: suspened_complete(...) is co_await to make sure that the
    //    coroutine is suspended at the point when set_*(...) is called.
    using result_type = ::beman::execution26::detail::
        await_result_type<Awaiter, ::beman::execution26::detail::connect_awaitable_promise<Receiver> >;

    ::std::exception_ptr ep;
    try {
        if constexpr (std::same_as<void, result_type>) {
            co_await ::std::move(awaiter);
            co_await ::beman::execution26::detail::suspend_complete(::beman::execution26::set_value,
                                                                    ::std::move(receiver));
        } else {
            co_await ::beman::execution26::detail::suspend_complete(
                ::beman::execution26::set_value, ::std::move(receiver), co_await ::std::move(awaiter));
        }
    } catch (...) {
        ep = ::std::current_exception();
    }
    co_await ::beman::execution26::detail::suspend_complete(
        ::beman::execution26::set_error, ::std::move(receiver), ep);
}
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
