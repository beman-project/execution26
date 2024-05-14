// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_STOP_TOKEN
#define INCLUDED_STOP_TOKEN

#include <atomic>
#include <concepts>
#include <cstddef>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <iostream> //-dk:TODO

namespace beman::inline cpp26
{
    template<class Token, class CallbackFun>
    using stop_callback_for_t = Token::template callback_type<CallbackFun>;

    namespace detail
    {
        template<typename CallbackFun, typename Token, typename Initializer = CallbackFun>
        concept stoppable_callback_for
            =  ::std::invocable<CallbackFun>
            && ::std::constructible_from<CallbackFun, Initializer>
            && requires { typename ::beman::cpp26::stop_callback_for_t<Token, CallbackFun>; }
            && ::std::constructible_from<::beman::cpp26::stop_callback_for_t<Token, CallbackFun>,
                                         Token,
                                         Initializer>
            && ::std::constructible_from<::beman::cpp26::stop_callback_for_t<Token, CallbackFun>,
                                         Token&,
                                         Initializer>
            && ::std::constructible_from<::beman::cpp26::stop_callback_for_t<Token, CallbackFun>,
                                         Token const&,
                                         Initializer>
            ;

        template <template<typename> class> struct check_type_alias_exist;

        struct stop_state;
        struct stop_callback_base;
    }

    template<typename Token>
    concept stoppable_token
        =   requires(Token const& token)
            {
                typename ::beman::cpp26::detail::check_type_alias_exist<Token::template callback_type>;
                { token.stop_requested() } noexcept -> ::std::same_as<bool>;
                { token.stop_possible() } noexcept -> ::std::same_as<bool>;
                { Token(token) } noexcept;
            }
        &&  ::std::copyable<Token>
        &&  ::std::equality_comparable<Token>
        &&  ::std::swappable<Token>
        ;

    template<typename Token>
    concept unstoppable_token
        =  ::beman::cpp26::stoppable_token<Token>
        && requires()
            {
                requires ::std::bool_constant<not Token::stop_possible()>::value;
            }
        ;

    namespace detail
    {
        template<typename Source>
        concept stoppable_source
            =   requires(Source& source, Source const& csource)
                {
                    { csource.get_token() } -> ::beman::cpp26::stoppable_token;
                    { csource.stop_possible() } noexcept -> ::std::same_as<bool>;
                    { csource.stop_requested() } noexcept -> ::std::same_as<bool>;
                    { source.request_stop() } -> ::std::same_as<bool>;
                }
            ;
    }

    class never_stop_token;

    class stop_token;
    class stop_source;
    template<typename CallbackFun> class stop_callback;

    struct nostopstate_t {
        explicit nostopstate_t() = default;
    };
    inline constexpr nostopstate_t nostopstate{};

    class inplace_stop_token;
    class inplace_stop_source;
    template<typename CallbackFun> class inplace_stop_callback;
}

// ----------------------------------------------------------------------------

struct beman::cpp26::detail::stop_state
{
    ::std::atomic<bool>                       stop_requested{};
    ::std::atomic<::std::size_t>              sources{};
    ::std::mutex                              lock{};
    beman::cpp26::detail::stop_callback_base* callbacks{};

    auto stop_possible() const -> bool { return this->sources != 0 || this->stop_requested; }
};

// ----------------------------------------------------------------------------

struct beman::cpp26::detail::stop_callback_base
{
private:
    using stop_state = ::beman::cpp26::detail::stop_state;
    ::std::shared_ptr<stop_state> state;

    virtual auto do_call() -> void = 0;

protected:
    stop_callback_base(::beman::cpp26::stop_token const&);
    ~stop_callback_base();

public:
    auto call() -> void;
    auto setup() -> void;

    stop_callback_base*              next{};
    ::std::atomic<bool>              executing{};
    ::std::atomic<::std::thread::id> id{};
};

// ----------------------------------------------------------------------------

class beman::cpp26::stop_source
{
private:
    using stop_state = ::beman::cpp26::detail::stop_state;

    ::std::shared_ptr<stop_state> state{::std::make_shared<stop_state>()};

public:
    using stop_token = ::beman::cpp26::stop_token;

    stop_source();
    stop_source(stop_source const&);
    auto operator= (stop_source const&) -> stop_source&;
    ~stop_source();

    auto get_token() const -> stop_token;
    auto stop_requested() const noexcept -> bool;
    auto stop_possible() const noexcept -> bool;
    auto request_stop() -> bool;
};

// ----------------------------------------------------------------------------

class beman::cpp26::stop_token
{
private:
    friend ::beman::cpp26::stop_source;
    friend ::beman::cpp26::detail::stop_callback_base;
    ::std::shared_ptr<::beman::cpp26::detail::stop_state> state;

    stop_token(::std::shared_ptr<::beman::cpp26::detail::stop_state>);

public:
    template <typename Fun>
    using callback_type = ::beman::cpp26::stop_callback<Fun>;

    stop_token() = default;

    auto swap(stop_token& other) noexcept -> void;
    [[nodiscard]] auto stop_requested() const noexcept -> bool;
    [[nodiscard]] auto stop_possible() const noexcept -> bool;

    [[nodiscard]] auto operator== (stop_token const&) const noexcept -> bool = default;
};

// ----------------------------------------------------------------------------

template<typename CallbackFun>
class beman::cpp26::stop_callback final
    : private CallbackFun
    , beman::cpp26::detail::stop_callback_base
{
private:
    auto do_call() -> void override
    {
        (*this)();
    }

public:
    using stop_token = ::beman::cpp26::stop_token;

    stop_callback(stop_token const& token, auto init)
        : CallbackFun(::std::move(init))
        , stop_callback_base(token)
    {
        this->setup();
    }
};

// ----------------------------------------------------------------------------

inline beman::cpp26::detail::stop_callback_base::stop_callback_base(
    ::beman::cpp26::stop_token const& token)
    : state(token.state)
{
}

inline auto beman::cpp26::detail::stop_callback_base::setup() -> void
{
    ::std::cout << "registering callback: " << this->state.get() << "\n";
    {
        ::std::lock_guard guard(this->state->lock);
        if (!this->state->stop_requested)
        {
            this->next = ::std::exchange(this->state->callbacks, this);
            return;
        }
    }
    this->call();
}

inline beman::cpp26::detail::stop_callback_base::~stop_callback_base()
{
    ::std::cout << "unregistering callback: " << this->state.get() << "\n";
    ::std::unique_lock guard(this->state->lock);
    if (this->executing && this->id != ::std::this_thread::get_id())
    {
        using lock_again = decltype([](auto p){ p->lock(); });
        ::std::unique_ptr<decltype(guard), lock_again> relock(&guard);
        relock->unlock();
        while (this->executing)
            ;
    }
    for (auto next = &this->state->callbacks; *next; next = &this->next)
    {
        if (*next == this)
        {
            *next = this->next;
            break;
        }
    }
}

inline auto beman::cpp26::detail::stop_callback_base::call() -> void
{
    this->do_call();
}

inline beman::cpp26::stop_token::stop_token(::std::shared_ptr<::beman::cpp26::detail::stop_state> state)
    : state(::std::move(state))
{
}

inline auto beman::cpp26::stop_token::swap(stop_token& other) noexcept -> void
{
    this->state.swap(other.state);
}

inline auto beman::cpp26::stop_token::stop_requested() const noexcept -> bool
{
    return this->state && this->state->stop_requested;
}

inline auto beman::cpp26::stop_token::stop_possible() const noexcept -> bool
{
    return this->state && this->state->stop_possible();
}

// ----------------------------------------------------------------------------

inline beman::cpp26::stop_source::stop_source()
{
    ++this->state->sources;
}

inline beman::cpp26::stop_source::stop_source(stop_source const& other)
    : state(other.state)
{
    ++this->state->sources;
}

inline auto beman::cpp26::stop_source::operator= (stop_source const& other) -> stop_source&
{
    --this->state->sources;
    this->state = other.state;
    ++this->state->sources;
    return *this;
}

inline beman::cpp26::stop_source::~stop_source()
{
    --this->state->sources;
}

inline auto beman::cpp26::stop_source::get_token() const -> stop_token
{
    return stop_token{this->state};
}

inline auto beman::cpp26::stop_source::stop_requested() const noexcept -> bool
{
    return this->state->stop_requested;
}

inline auto beman::cpp26::stop_source::stop_possible() const noexcept -> bool
{
    return true;
}

inline auto beman::cpp26::stop_source::request_stop() -> bool
{
    using release = decltype([](auto p){ *p = false; });
    using lock_again = decltype([](auto p){ p->lock(); });
    ::std::cout << "request_stop: " << this->state.get() << "\n";

    if (not this->state->stop_requested.exchange(true))
    {
        ::std::cout << "actually stopping\n";
        ::std::unique_lock guard(this->state->lock);
        while (this->state->callbacks)
        {
            ::std::cout << "has callback\n";
            auto front = ::std::exchange(this->state->callbacks, this->state->callbacks->next);
            ::std::unique_ptr<::std::atomic<bool>, release> reset(&front->executing);
            *reset = true;
            front->id = ::std::this_thread::get_id();
            ::std::unique_ptr<decltype(guard), lock_again> relock(&guard);
            relock->unlock();
            front->call();
        }
    }
    ::std::cout << "request_stop done\n";
    return true;
}

// ----------------------------------------------------------------------------

#endif // INCLUDED_STOP_TOKEN
