// include/beman/execution26/detail/just.hpp                          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_JUST
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_JUST

#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/default_impls.hpp>
#include <concepts>
#include <memory>
#include <utility>

// ----------------------------------------------------------------------------

#include <beman/execution26/detail/suppress_push.hpp>

namespace beman::execution26::detail {
template <typename Completion, typename... T>
concept just_size = (not::std::same_as<Completion, ::beman::execution26::set_error_t> or 1u == sizeof...(T)) &&
                    (not::std::same_as<Completion, ::beman::execution26::set_stopped_t> or 0u == sizeof...(T));
template <typename Completion>
struct just_t {
    template <typename... T>
        requires ::beman::execution26::detail::just_size<Completion, T...> &&
                 (::std::movable<::std::decay_t<T>> && ...)
    auto operator()(T&&... arg) const {
        return ::beman::execution26::detail::make_sender(
            *this, ::beman::execution26::detail::product_type{::std::forward<T>(arg)...});
    }
};

template <typename Completion, typename... T, typename Env>
struct completion_signatures_for_impl<
    ::beman::execution26::detail::basic_sender<just_t<Completion>, ::beman::execution26::detail::product_type<T...>>,
    Env> {
    using type = ::beman::execution26::completion_signatures<Completion(T...)>;
};

template <typename Completion>
struct impls_for<just_t<Completion>> : ::beman::execution26::detail::default_impls {
    static constexpr auto start = []<typename State>(State& state, auto& receiver) noexcept -> void {
        [&state, &receiver]<::std::size_t... I>(::std::index_sequence<I...>) {
            Completion()(::std::move(receiver), ::std::move(state.template get<I>())...);
        }(::std::make_index_sequence<State::size()>{});
    };
};
} // namespace beman::execution26::detail

#include <beman/execution26/detail/suppress_pop.hpp>

namespace beman::execution26 {
using just_t         = ::beman::execution26::detail::just_t<::beman::execution26::set_value_t>;
using just_error_t   = ::beman::execution26::detail::just_t<::beman::execution26::set_error_t>;
using just_stopped_t = ::beman::execution26::detail::just_t<::beman::execution26::set_stopped_t>;

inline constexpr ::beman::execution26::just_t         just{};
inline constexpr ::beman::execution26::just_error_t   just_error{};
inline constexpr ::beman::execution26::just_stopped_t just_stopped{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
