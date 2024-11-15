// include/beman/execution26/detail/finite_inplace_stop_source.hpp    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_INCLUDE_BEMAN_EXECUTION26_DETAIL_FINITE_INPLACE_STOP_SOURCE
#define INCLUDED_INCLUDE_BEMAN_EXECUTION26_DETAIL_FINITE_INPLACE_STOP_SOURCE

#include <algorithm>
#include <array>
#include <atomic>
#include <exception>
#include <thread>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
    struct finite_inplace_stop_callback_base {
        virtual auto notify() -> void = 0;
    };

    struct finite_inplace_stop_marker_t
        : ::beman::execution26::detail::finite_inplace_stop_callback_base
    {
        auto notify() -> void override {}
    };
    inline constinit finite_inplace_stop_marker_t finite_inplace_stop_stopping{};
    inline constinit finite_inplace_stop_marker_t finite_inplace_stop_stopped{};
}

namespace beman::execution26 {
    template <::std::size_t>
       class finite_inplace_stop_source;
    template <::std::size_t N, ::std::size_t I>
       requires(I < N)
       class finite_inplace_stop_token;
    template <::std::size_t N, ::std::size_t I, typename>
       requires(I < N)
       class finite_inplace_stop_callback;

    template <::std::size_t N, ::std::size_t I, typename Callback>
    finite_inplace_stop_callback(finite_inplace_stop_token<N, I>, Callback)
        -> finite_inplace_stop_callback<N, I, Callback>;
}

// ----------------------------------------------------------------------------

template <::std::size_t N, ::std::size_t I>
    requires(I < N)
class beman::execution26::finite_inplace_stop_token
{
public:
    template <typename Callback>
    using callback_type = ::beman::execution26::finite_inplace_stop_callback<N, I, Callback>;

    constexpr finite_inplace_stop_token() noexcept = default;

    auto operator== (finite_inplace_stop_token const&) const noexcept -> bool = default;
    auto stop_possible() const noexcept -> bool { return this->source != nullptr; }
    auto stop_requested() const noexcept -> bool;
    auto swap(finite_inplace_stop_token&) noexcept -> void;

private:
    friend class ::beman::execution26::finite_inplace_stop_source<N>;
    template <std::size_t NN, std::size_t II, typename>
        requires(II < NN)
    friend class ::beman::execution26::finite_inplace_stop_callback;
    
    finite_inplace_stop_token(::beman::execution26::finite_inplace_stop_source<N>* source)
        : source(source)
    {
    }
    ::beman::execution26::finite_inplace_stop_source<N>* source{};
};

// ----------------------------------------------------------------------------

template <std::size_t N>
class beman::execution26::finite_inplace_stop_source
{
public:
    constexpr finite_inplace_stop_source() noexcept;
    finite_inplace_stop_source(finite_inplace_stop_source&&) = delete;
    finite_inplace_stop_source(finite_inplace_stop_source const&) = delete;
    ~finite_inplace_stop_source() = default;
    auto operator=(finite_inplace_stop_source&&) -> finite_inplace_stop_source& = delete;
    auto operator=(finite_inplace_stop_source const&) -> finite_inplace_stop_source& = delete;

    template <std::size_t I = N == 1? 0u: ~0u>
        requires (I < N)
    constexpr auto get_token() const noexcept
        -> beman::execution26::finite_inplace_stop_token<N, I>
    {
        return {const_cast<finite_inplace_stop_source*>(this)};
    }
    static constexpr auto stop_possible() noexcept -> bool { return true; }
    auto stop_requested() const noexcept -> bool;
    auto request_stop() noexcept -> bool;

private:
    template <::std::size_t NN, std::size_t II, typename>
        requires(II < NN)
    friend class ::beman::execution26::finite_inplace_stop_callback;

    ::std::atomic<bool> requested{false};
    ::std::atomic<::std::thread::id> id;
    ::std::array<::std::atomic<::beman::execution26::detail::finite_inplace_stop_callback_base*>, N> callbacks{};
};

// ----------------------------------------------------------------------------

template <std::size_t N, std::size_t I, typename Callback>
    requires(I < N)
class beman::execution26::finite_inplace_stop_callback
    : beman::execution26::detail::finite_inplace_stop_callback_base
{
public:
    template <typename Initializer>
    finite_inplace_stop_callback(::beman::execution26::finite_inplace_stop_token<N, I> token, Initializer&& init)
        : callback(init)
        , source(token.source)
    {
        ::beman::execution26::detail::finite_inplace_stop_callback_base* expected{};

        if (this->source && not this->source->callbacks[I].compare_exchange_strong(expected, this)) {
            if (expected == &::beman::execution26::detail::finite_inplace_stop_stopping
                || expected == &::beman::execution26::detail::finite_inplace_stop_stopped) {
                this->source = nullptr;
                this->callback();
            }
            else {
                ::std::terminate();
            }
        }
    }
    
    finite_inplace_stop_callback(finite_inplace_stop_callback&&) = delete;
    finite_inplace_stop_callback(finite_inplace_stop_callback const&) = delete;
    ~finite_inplace_stop_callback()
    {
        auto* const source{this->source};
        if (source) {
            auto result{source->callbacks[I].exchange(nullptr)};
            if (result == &::beman::execution26::detail::finite_inplace_stop_stopping
                && source->id != ::std::this_thread::get_id()) {
                source->callbacks[I].wait(&::beman::execution26::detail::finite_inplace_stop_stopping);
            }
        }
    }
    auto operator=(finite_inplace_stop_callback&&) -> finite_inplace_stop_callback = delete;
    auto operator=(finite_inplace_stop_callback const&) -> finite_inplace_stop_callback = delete;

private:
    auto notify() -> void override {
        this->callback();
    }

    Callback callback;
    ::beman::execution26::finite_inplace_stop_source<N>* source;
};

// ----------------------------------------------------------------------------

template <::std::size_t N, ::std::size_t I>
    requires(I < N)
auto beman::execution26::finite_inplace_stop_token<N, I>::stop_requested() const noexcept -> bool
{
    return this->source && this->source->stop_requested();
}

template <::std::size_t N, ::std::size_t I>
    requires(I < N)
auto beman::execution26::finite_inplace_stop_token<N, I>::swap(finite_inplace_stop_token& other) noexcept -> void
{
    ::std::swap(this->source, other.source);
}
// ----------------------------------------------------------------------------

template <::std::size_t N>
constexpr beman::execution26::finite_inplace_stop_source<N>::finite_inplace_stop_source() noexcept
{
}

template <::std::size_t N>
auto beman::execution26::finite_inplace_stop_source<N>::stop_requested() const noexcept -> bool
{
    return this->requested;
}

template <::std::size_t N>
auto beman::execution26::finite_inplace_stop_source<N>::request_stop() noexcept -> bool
{
    if (this->requested.exchange(true))
        return false;
    this->id = ::std::this_thread::get_id();
    for (auto& cb: this->callbacks) {
        auto current{cb.exchange(&::beman::execution26::detail::finite_inplace_stop_stopping)};
        if (current != nullptr) {
            current->notify();
        }
        cb = &::beman::execution26::detail::finite_inplace_stop_stopped;
        cb.notify_one();
    }
    return true;
}

// ----------------------------------------------------------------------------


#endif
