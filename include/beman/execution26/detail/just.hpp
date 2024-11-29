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

/*!
 * \brief <code>just(_arg_...)</code>` yields a sender completing with <code>set_value_t(_Arg_...)</code>
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 *
 * \details
 * `just` is a callable object of type `just_t`. Invoking <code>just(_arg_...)</code> yields a sender which stores its
 * arguments and produces a value completion with these arguments when started. This sender completes synchronously
 * when started.
 *
 * <h4>Usage</h4>
 * <pre>
 * just(<i>arg</i>...)
 * </pre>
 *
 * Above <code>_Arg_...</code> is a pack of the types of <code>_arg_...</code>
 * after removing top-level `const` and reference qualifications
 * (<code>std::remove_cvref_t&lt;decltype(_arg_)&gt;...</code>).
 *
 * <h4>Completions Signatures</h4>
 * <pre>
 * completion_signatures<
 *     set_value_t(<i>Arg</i>...)
 * >;
 * </pre>
 *
 * <h4>Example</h4>
 *
 * The normal use of <code>just(_args_...)</code> is as the starting
 * point of a work graph. Various other examples will use `just` as
 * their starting. The example below create a sender yielding three
 * values and awaits the completion using <code>sync_wait(_sender_)</code>:
 * for a value completion of <code>_sender_</code> it will yield an
 * <code>std::optional&lt;std::tuple&lt;_Args_...&gt;&gt;` with the
 * `tuple` containing the value copied/moved from the original arguments
 * (an `optional` is returned to indicate cancellation).
 *
 * <pre example="doc-just.cpp">
 * #include <beman/execution26/execution.hpp>
 * #include <cassert>
 * #include <string>
 * using namespace std::string_literals;
 *
 * int main() {
 *     auto result = ex::sync_wait(ex::just(17, "hello"s, true));
 *     assert(result);
 *     assert(*result == std::tuple(17, "hello"s, true));
 * }
 * </pre>
 */
inline constexpr ::beman::execution26::just_t         just{};

/*!
 * \brief <code>just_error(_error_)</code> yields a sender completing with <code>set_error_t(_Error_)</code>
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 *
 * \details
 * `just_error` is a callable object of type `just_error_t`. Invoking <code>just_error(_error_)</code> yields a sender which
 * stores its argument and produces an error completion with this error when started. This sender completes
 * synchronously when started.
 *
 * <h4>Usage</h4>
 * <pre>
 * just_error(<i>error</i>)
 * </pre>
 *
 * The type <code>_Error_</code> used above is the type of <code>_error_</code>
 * after removing top-level `const` and reference qualifications
 * (<code>std::remove_cvref_t&lt;decltype(error)&gt;</code>).
 *
 * <h4>Completions Signatures</h4>
 * <pre>
 * completion_signatures<
 *     set_error_t(<i>Error</i>)
 * >;
 * </pre>
 *
 * <h4>Example</h4>
 *
 * The normal use of <code>just_error(_error_)</code> is to report an
 * error as the result of some work in a work graph. It would, e.g., be
 * used as the completion produced by `let_value`.
 * The example below creates a sender yielding an `std::error_code` on the error
 * channel and
 * uses that as the input for `upon_error` consuming the error and producing
 * a value completion: using <code>sync_wait(just_error(_error_))</code>
 * directly doesn't work because `sync_wait` requires exactly one value completion
 * from its argument and `set_error` only has an error completion. The function used with `upon_error` verifies that the
 * expected code was produced and also sets the flag `had_error` indicating it
 * was called at all. This flag is checked after waiting for the result
 * in `sync_wait`.
 *
 * <pre example="doc-just_error.cpp">
 * #include <beman/execution26/execution.hpp>
 * #include <system_error>
 * #include <cassert>
 * namespace ex = beman::execution26;
 *
 * int main() {
 *     bool had_error{false};
 *     auto result = ex::sync_wait(ex::just_error(std::error_code(17, std::system_category())) |
 *                                 ex::upon_error([&](std::error_code ec) {
 *                                     assert(ec.value() == 17);
 *                                     had_error = true;
 *                                 }));
 *     assert(had_error);
 * }
 * </pre>
 */
inline constexpr ::beman::execution26::just_error_t   just_error{};

/*!
 * \brief <code>just_stopped(_)</code> yields a sender completing with <code>set_stopped_t()</code>
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 *
 * \details
 * `just_stopped` is a callable object of type `just_stopped_t`. Invoking <code>just_stopped()</code> yields a sender which
 * produces a cancellation completion when started. This sender completes
 * synchronously when started.
 *
 * <h4>Usage</h4>
 * <pre>
 * just_stopped()
 * </pre>
 *
 * <h4>Completions Signatures</h4>
 * <pre>
 * completion_signatures<
 *     set_stopped_t()
 * >;
 * </pre>
 *
 * <h4>Example</h4>
 *
 * The normal use of <code>just_stopped()</code> is to report a
 * cancellation as the result of some work in a work graph. It would, e.g., be
 * used as the completion produced by `let_value`.
 * The example below creates a sender yielding a completion on the cancellation
 * channel and
 * uses that as the input for `upon_stopped` consuming the cancellation and producing
 * a value completion: using <code>sync_wait(just_stopped())</code>
 * directly doesn't work because `sync_wait` requires exactly one value completion
 * from its argument and `set_stopped` only has a cancellation completion. The function used with `upon_stopped` 
 * sets the flag `had_stopped` indicating it
 * was called at all. This flag is checked after waiting for the result
 * in `sync_wait`.
 *
 * <pre example="doc-just_error.cpp">
 * #include <beman/execution26/execution.hpp>
 * #include <system_error>
 * #include <cassert>
 * namespace ex = beman::execution26;
 *
 * int main() {
 *     bool had_stopped{false};
 *     auto result = ex::sync_wait(ex::just_error(std::error_code(17, std::system_category())) |
 *                                 ex::upon_error([&](std::error_code ec) {
 *                                     assert(ec.value() == 17);
 *                                     had_stopped = true;
 *                                 }));
 *     assert(had_stopped);
 * }
 * </pre>
 */
inline constexpr ::beman::execution26::just_stopped_t just_stopped{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
