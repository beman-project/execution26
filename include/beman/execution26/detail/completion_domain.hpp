// include/beman/execution26/detail/completion_domain.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_DOMAIN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_DOMAIN

#include <beman/execution26/detail/default_domain.hpp>
#include <beman/execution26/detail/get_domain.hpp>
#include <beman/execution26/detail/get_completion_scheduler.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
struct completion_domain_undefined {};
template <typename, typename>
struct completion_domain_merge {};
template <typename T>
struct completion_domain_merge<T, T> {
    using type = T;
};
template <typename T>
struct completion_domain_merge<completion_domain_undefined, T> {
    using type = T;
};
template <typename T>
struct completion_domain_merge<T, completion_domain_undefined> {
    using type = T;
};
template <>
struct completion_domain_merge<completion_domain_undefined, completion_domain_undefined> {
    using type = completion_domain_undefined;
};

template <typename Default = ::beman::execution26::default_domain, typename Sender>
constexpr auto completion_domain(const Sender& sender) noexcept {

    static_assert(::beman::execution26::sender<Sender>);
    auto get = [&sender]<typename Tag>(Tag) {
        if constexpr (requires {
                          ::beman::execution26::get_domain(::beman::execution26::get_completion_scheduler<Tag>(
                              ::beman::execution26::get_env(sender)));
                      }) {
            return ::beman::execution26::get_domain(
                ::beman::execution26::get_completion_scheduler<Tag>(::beman::execution26::get_env(sender)));
        } else {
            return completion_domain_undefined{};
        }
    };

    using type = typename completion_domain_merge<
        typename completion_domain_merge<decltype(get(::beman::execution26::set_error)),
                                         decltype(get(::beman::execution26::set_stopped))>::type,
        decltype(get(::beman::execution26::set_value))>::type;
    return ::std::conditional_t< ::std::same_as<type, completion_domain_undefined>, Default, type>();
}
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
