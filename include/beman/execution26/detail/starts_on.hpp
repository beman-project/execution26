// include/beman/execution26/detail/starts_on.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STARTS_ON
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STARTS_ON

#include <beman/execution26/detail/scheduler.hpp>
#include <beman/execution26/detail/sender_for.hpp>
#include <beman/execution26/detail/transform_sender.hpp>
#include <beman/execution26/detail/query_with_default.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/get_domain.hpp>
#include <beman/execution26/detail/default_domain.hpp>
#include <beman/execution26/detail/join_env.hpp>
#include <beman/execution26/detail/sched_env.hpp>
#include <beman/execution26/detail/fwd_env.hpp>
#include <beman/execution26/detail/forward_like.hpp>
#include <beman/execution26/detail/let.hpp>
#include <beman/execution26/detail/schedule.hpp>
#include <concepts>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
struct starts_on_t {
    template <::beman::execution26::detail::sender_for<::beman::execution26::detail::starts_on_t> Sender, typename Env>
    auto transform_env(Sender&& sender, Env&& env) const noexcept {
        auto&& scheduler{sender.template get<1>()};
        return ::beman::execution26::detail::join_env(::beman::execution26::detail::sched_env(scheduler),
                                                      ::beman::execution26::detail::fwd_env(env));
    }
    template <::beman::execution26::detail::sender_for<::beman::execution26::detail::starts_on_t> Sender,
              typename... Env>
    auto transform_sender(Sender&& sender, Env&&...) const noexcept {
        auto&& scheduler{sender.template get<1>()};
        auto&& new_sender{sender.template get<2>()};
        return ::beman::execution26::let_value(
            ::beman::execution26::schedule(scheduler),
            [new_sender = ::beman::execution26::detail::forward_like<Sender>(new_sender)]() mutable noexcept(
                ::std::is_nothrow_constructible_v<::std::decay_t<Sender>>) { return ::std::move(new_sender); });
    }

    template <::beman::execution26::scheduler Scheduler, ::beman::execution26::sender Sender>
    auto operator()(Scheduler&& scheduler, Sender&& sender) const {
        auto domain{::beman::execution26::detail::query_with_default(::beman::execution26::get_domain,
                                                                     ::std::forward<Scheduler>(scheduler),
                                                                     ::beman::execution26::default_domain{})};
        return ::beman::execution26::transform_sender(
            domain,
            ::beman::execution26::detail::make_sender(
                *this, ::std::forward<Scheduler>(scheduler), ::std::forward<Sender>(sender)));
    }
};
} // namespace beman::execution26::detail

namespace beman::execution26 {
using ::beman::execution26::detail::starts_on_t;
inline constexpr ::beman::execution26::detail::starts_on_t starts_on{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
