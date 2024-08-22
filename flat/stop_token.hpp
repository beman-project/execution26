// include/Beman/Execution26/stop_token.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_STOP_TOKEN

#include <utility>
#include <concepts>
#include <type_traits>
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <cstddef>
#include <cassert>
#include <exception>
#include <system_error>
// include/Beman/Execution26/detail/check_type_alias_exist.hpp        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CHECK_TYPE_ALIAS_EXIST
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CHECK_TYPE_ALIAS_EXIST

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <template<typename> class> struct check_type_alias_exist;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/inplace_stop_source.hpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_INPLACE_STOP_SOURCE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_INPLACE_STOP_SOURCE


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    class inplace_stop_token;
    class inplace_stop_source;
    template<typename CallbackFun> class inplace_stop_callback;
    template <typename CallbackFun>
    inplace_stop_callback(::Beman::Execution26::inplace_stop_token, CallbackFun)
        -> inplace_stop_callback<CallbackFun>;
}

// ----------------------------------------------------------------------------

class Beman::Execution26::inplace_stop_token
{
public:
    template <typename CallbackFun>
    using callback_type = ::Beman::Execution26::inplace_stop_callback<CallbackFun>;

    inplace_stop_token() = default;

    constexpr auto stop_requested() const noexcept  -> bool;
    constexpr auto stop_possible() const noexcept  -> bool;
    auto operator== (inplace_stop_token const&) const -> bool = default;

    auto swap(inplace_stop_token&) noexcept -> void;

private:
    friend class ::Beman::Execution26::inplace_stop_source;
    template <typename CallbackFun>
    friend class ::Beman::Execution26::inplace_stop_callback;
    inplace_stop_token(::Beman::Execution26::inplace_stop_source* source): source(source) {}

    ::Beman::Execution26::inplace_stop_source* source{};
};

// ----------------------------------------------------------------------------

class Beman::Execution26::inplace_stop_source
{
    struct callback_base
    {
        callback_base* next{};
        virtual auto call() -> void = 0;
    };

public:
    auto stop_requested() const noexcept -> bool;
    static constexpr auto stop_possible() noexcept -> bool;
    auto get_token() const -> ::Beman::Execution26::inplace_stop_token;

    auto request_stop() -> bool;

private:
    template <typename CallbackFun>
    friend class ::Beman::Execution26::inplace_stop_callback;
    ::std::atomic<bool>           stopped{};
    ::std::atomic<callback_base*> running{};
    ::std::thread::id             id{};
    ::std::mutex                  lock;
    callback_base*                callbacks{};

    auto add(callback_base* cb) -> void;
    auto deregister(callback_base* cb) -> void;
};

// ----------------------------------------------------------------------------

template <typename CallbackFun>
class Beman::Execution26::inplace_stop_callback final
    : public ::Beman::Execution26::inplace_stop_source::callback_base 
{
public:
    using callback_type = CallbackFun;

    template <typename Init>
    inplace_stop_callback(::Beman::Execution26::inplace_stop_token, Init&&);
    inplace_stop_callback(inplace_stop_callback&&) = delete;
    ~inplace_stop_callback()
    {
        if (this->source)
        {
            this->source->deregister(this);
        }
    }

private:
    auto call() -> void override;

    CallbackFun                          fun;
    ::Beman::Execution26::inplace_stop_source* source;
};

// ----------------------------------------------------------------------------

inline constexpr auto Beman::Execution26::inplace_stop_token::stop_requested() const noexcept  -> bool
{
    return this->source && this->source->stop_requested();
}

inline constexpr auto Beman::Execution26::inplace_stop_token::stop_possible() const noexcept  -> bool
{
    return this->source;
}

inline auto Beman::Execution26::inplace_stop_token::swap(inplace_stop_token& other) noexcept -> void
{
    ::std::swap(this->source, other.source);
}

inline auto Beman::Execution26::inplace_stop_source::stop_requested() const noexcept -> bool
{
    return this->stopped;
}

inline constexpr auto Beman::Execution26::inplace_stop_source::stop_possible() noexcept -> bool
{
    return true;
}

inline auto Beman::Execution26::inplace_stop_source::get_token() const -> ::Beman::Execution26::inplace_stop_token
{
    return ::Beman::Execution26::inplace_stop_token(const_cast<::Beman::Execution26::inplace_stop_source*>(this));
}

inline auto Beman::Execution26::inplace_stop_source::request_stop() -> bool
{
    using relock = ::std::unique_ptr<::std::unique_lock<::std::mutex>, decltype([](auto p){ p->lock(); })>;
    if (false == this->stopped.exchange(true))
    {
        ::std::unique_lock guard(this->lock);
        for (auto it = this->callbacks; it != nullptr; it = this->callbacks)
        {
            this->running   = it;
            this->id        = ::std::this_thread::get_id();
            this->callbacks = it->next;
            {
                relock r(&guard);
                guard.unlock();
                it->call();
            }
            this->running = nullptr;
        }
        return true;
    }
    return false;
}

inline auto Beman::Execution26::inplace_stop_source::add(callback_base* cb) -> void
{
    if (this->stopped)
    {
        cb->call();
    }
    else
    {
        ::std::lock_guard guard(this->lock);
        cb->next = ::std::exchange(this->callbacks, cb);
    }
}

inline auto Beman::Execution26::inplace_stop_source::deregister(callback_base* cb) -> void
{
    ::std::unique_lock guard(this->lock);
    if (this->running == cb)
    {
        if (this->id == ::std::this_thread::get_id())
        {
            return;
        }
        guard.unlock();
        while (this->running == cb)
        {
        }
        return;
    }

    for (callback_base** it{&this->callbacks}; *it; it = &(*it)->next)
    {
        if (*it == cb)
        {
            *it = cb->next;
            break;
        }
    }
}

template <typename CallbackFun>
    template <typename Init>
inline Beman::Execution26::inplace_stop_callback<CallbackFun>::inplace_stop_callback(
    ::Beman::Execution26::inplace_stop_token token,
    Init&& init)
    : fun(::std::forward<Init>(init))
    , source(token.source)
{
    if (this->source)
    {
        this->source->add(this);
    }
}

template <typename CallbackFun>
inline auto Beman::Execution26::inplace_stop_callback<CallbackFun>::call() -> void 
{
    this->fun();
}


// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/never_stop_token.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NEVER_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NEVER_STOP_TOKEN

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    class never_stop_token;
}

// ----------------------------------------------------------------------------

class Beman::Execution26::never_stop_token
{
    struct private_callback_type
    {
        explicit private_callback_type(never_stop_token, auto&&) noexcept {}
    };
public:
    template <typename>
    using callback_type = private_callback_type;

    static constexpr auto stop_requested() noexcept  -> bool { return {}; }
    static constexpr auto stop_possible() noexcept  -> bool { return {}; }
    auto operator== (never_stop_token const&) const -> bool = default;
};

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/nostopstate.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NOSTOPSTATE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NOSTOPSTATE

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct nostopstate_t {
        explicit nostopstate_t() = default;
    };

    inline constexpr nostopstate_t nostopstate{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/stop_callback_for_t.hpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOP_CALLBACK_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOP_CALLBACK_FOR


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template<class Token, class CallbackFun>
    using stop_callback_for_t = Token::template callback_type<CallbackFun>;
}

namespace Beman::Execution26::Detail
{
    template<typename CallbackFun, typename Token, typename Initializer = CallbackFun>
    concept stoppable_callback_for
        =  ::std::invocable<CallbackFun>
        && ::std::constructible_from<CallbackFun, Initializer>
        && requires { typename ::Beman::Execution26::stop_callback_for_t<Token, CallbackFun>; }
        && ::std::constructible_from<::Beman::Execution26::stop_callback_for_t<Token, CallbackFun>,
                                     Token,
                                     Initializer>
        && ::std::constructible_from<::Beman::Execution26::stop_callback_for_t<Token, CallbackFun>,
                                     Token&,
                                     Initializer>
        && ::std::constructible_from<::Beman::Execution26::stop_callback_for_t<Token, CallbackFun>,
                                     Token const&,
                                     Initializer>
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/stop_source.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOP_SOURCE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOP_SOURCE


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    class stop_token;
    class stop_source;
    template<typename CallbackFun> class stop_callback;
    template <typename CallbackFun>
    stop_callback(::Beman::Execution26::stop_token, CallbackFun) -> stop_callback<CallbackFun>;
}

namespace Beman::Execution26::Detail
{
    struct stop_state;
    struct stop_callback_base;
}
// ----------------------------------------------------------------------------

struct Beman::Execution26::Detail::stop_state
{
    ::std::atomic<bool>                             stop_requested{};
    ::std::atomic<::std::size_t>                    sources{};
    ::std::mutex                                    lock{};
    Beman::Execution26::Detail::stop_callback_base* callbacks{};
    ::std::atomic<bool>                             executing{};

    auto stop_possible() const -> bool { return this->sources != 0 || this->stop_requested; }
};

// ----------------------------------------------------------------------------

struct Beman::Execution26::Detail::stop_callback_base
{
private:
    using stop_state = ::Beman::Execution26::Detail::stop_state;
    ::std::shared_ptr<stop_state> state;

    virtual auto do_call() -> void = 0;

protected:
    stop_callback_base(::Beman::Execution26::stop_token const&);
    ~stop_callback_base();

public:
    auto call() -> void;
    auto setup() -> void;
    auto deregister() -> void;

    stop_callback_base* next{};
    ::std::thread::id   id{};
};

// ----------------------------------------------------------------------------

class Beman::Execution26::stop_source
{
private:
    using stop_state = ::Beman::Execution26::Detail::stop_state;

    ::std::shared_ptr<stop_state> state{::std::make_shared<stop_state>()};

public:
    using stop_token = ::Beman::Execution26::stop_token;

    stop_source();
    explicit stop_source(::Beman::Execution26::nostopstate_t) noexcept;
    stop_source(stop_source const&);
    auto operator= (stop_source const&) -> stop_source&;
    ~stop_source();

    auto swap(stop_source&) noexcept -> void;
    auto get_token() const -> stop_token;
    auto stop_requested() const noexcept -> bool;
    auto stop_possible() const noexcept -> bool;
    auto request_stop() noexcept -> bool;
};

// ----------------------------------------------------------------------------

class Beman::Execution26::stop_token
{
private:
    friend ::Beman::Execution26::stop_source;
    friend ::Beman::Execution26::Detail::stop_callback_base;
    ::std::shared_ptr<::Beman::Execution26::Detail::stop_state> state;

    stop_token(::std::shared_ptr<::Beman::Execution26::Detail::stop_state>);

public:
    template <typename Fun>
    using callback_type = ::Beman::Execution26::stop_callback<Fun>;

    stop_token() = default;

    auto swap(stop_token& other) noexcept -> void;
    [[nodiscard]] auto stop_requested() const noexcept -> bool;
    [[nodiscard]] auto stop_possible() const noexcept -> bool;

    [[nodiscard]] auto operator== (stop_token const&) const noexcept -> bool = default;
};

// ----------------------------------------------------------------------------

template<typename CallbackFun>
class Beman::Execution26::stop_callback final
    : private CallbackFun
    , Beman::Execution26::Detail::stop_callback_base
{
private:
    static_assert(::std::invocable<CallbackFun>);
    static_assert(::std::destructible<CallbackFun>);

    using stop_token = ::Beman::Execution26::stop_token;

    auto do_call() -> void override
    {
        (*this)();
    }

public:
    using callback_type = CallbackFun;

    template <typename Initializer>
    stop_callback(stop_token const& token, Initializer&& init)
        noexcept(::std::is_nothrow_constructible_v<CallbackFun, Initializer>)
        requires(::std::constructible_from<CallbackFun, Initializer>)
        : CallbackFun(::std::forward<Initializer>(init))
        , stop_callback_base(token)
    {
        this->setup();
    }
    template <typename Initializer>
    stop_callback(stop_token&& token, Initializer&& init)
        noexcept(::std::is_nothrow_constructible_v<CallbackFun, Initializer>)
        requires(::std::is_constructible_v<CallbackFun, Initializer>)
        : CallbackFun(::std::forward<Initializer>(init))
        , stop_callback_base(::std::move(token))
    {
        this->setup();
    }
    ~stop_callback()
    {
        this->deregister();
    }
    stop_callback(stop_callback&&) = delete;
};

// ----------------------------------------------------------------------------

inline Beman::Execution26::Detail::stop_callback_base::stop_callback_base(
    ::Beman::Execution26::stop_token const& token)
    : state(token.state)
{
}

inline Beman::Execution26::Detail::stop_callback_base::~stop_callback_base()
{
}

inline auto Beman::Execution26::Detail::stop_callback_base::setup() -> void
{
    if (this->state)
    {
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
}

inline auto Beman::Execution26::Detail::stop_callback_base::deregister() -> void
{
    if (this->state)
    {
        ::std::unique_lock guard(this->state->lock);
        if (this->state->executing && this->id != ::std::this_thread::get_id())
        {
            using lock_again = decltype([](auto p){ p->lock(); });
            ::std::unique_ptr<decltype(guard), lock_again> relock(&guard);
            relock->unlock();
            while (this->state->executing)
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
}

inline auto Beman::Execution26::Detail::stop_callback_base::call() -> void
{
    this->do_call();
}

inline Beman::Execution26::stop_token::stop_token(::std::shared_ptr<::Beman::Execution26::Detail::stop_state> state)
    : state(::std::move(state))
{
}

inline auto Beman::Execution26::stop_token::swap(stop_token& other) noexcept -> void
{
    this->state.swap(other.state);
}

inline auto Beman::Execution26::stop_token::stop_requested() const noexcept -> bool
{
    return this->state && this->state->stop_requested;
}

inline auto Beman::Execution26::stop_token::stop_possible() const noexcept -> bool
{
    return this->state && this->state->stop_possible();
}

// ----------------------------------------------------------------------------

inline Beman::Execution26::stop_source::stop_source()
{
    ++this->state->sources;
}

inline Beman::Execution26::stop_source::stop_source(::Beman::Execution26::nostopstate_t) noexcept
    : state()
{
}

inline Beman::Execution26::stop_source::stop_source(stop_source const& other)
    : state(other.state)
{
    ++this->state->sources;
}

inline auto Beman::Execution26::stop_source::operator= (stop_source const& other) -> stop_source&
{
    --this->state->sources;
    this->state = other.state;
    ++this->state->sources;
    return *this;
}

inline Beman::Execution26::stop_source::~stop_source()
{
    this->state && --this->state->sources;
}

inline auto Beman::Execution26::stop_source::swap(::Beman::Execution26::stop_source& other) noexcept -> void
{
    this->state.swap(other.state);
}

inline auto Beman::Execution26::stop_source::get_token() const -> stop_token
{
    return stop_token{this->state};
}

inline auto Beman::Execution26::stop_source::stop_requested() const noexcept -> bool
{
    return this->state && this->state->stop_requested;
}

inline auto Beman::Execution26::stop_source::stop_possible() const noexcept -> bool
{
    return true && this->state;
}

inline auto Beman::Execution26::stop_source::request_stop() noexcept -> bool
{
    using release = decltype([](auto p){ *p = false; });
    using lock_again = decltype([](auto p){ p->lock(); });

    if (this->state && not this->state->stop_requested.exchange(true))
    {
        ::std::unique_lock guard(this->state->lock);
        while (this->state->callbacks)
        {
            auto front = ::std::exchange(this->state->callbacks, this->state->callbacks->next);
            ::std::unique_ptr<::std::atomic<bool>, release> reset(&state->executing);
            *reset = true;
            front->id = ::std::this_thread::get_id();
            ::std::unique_ptr<decltype(guard), lock_again> relock(&guard);
            relock->unlock();
            front->call();
        }
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/stoppable_token.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_TOKEN


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template<typename Token>
    concept stoppable_token
        =   requires(Token const& token)
            {
                typename ::Beman::Execution26::Detail::check_type_alias_exist<Token::template callback_type>;
                { token.stop_requested() } noexcept -> ::std::same_as<bool>;
                { token.stop_possible() } noexcept -> ::std::same_as<bool>;
                { Token(token) } noexcept;
            }
        &&  ::std::copyable<Token>
        &&  ::std::equality_comparable<Token>
        &&  ::std::swappable<Token>
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/stoppable_source.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_SOURCE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_SOURCE


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template<typename Source>
    concept stoppable_source
        =   requires(Source& source, Source const& csource)
            {
                { csource.get_token() } -> ::Beman::Execution26::stoppable_token;
                { csource.stop_possible() } noexcept -> ::std::same_as<bool>;
                { csource.stop_requested() } noexcept -> ::std::same_as<bool>;
                { source.request_stop() } -> ::std::same_as<bool>;
            }
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/unstoppable_token.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_UNSTOPPABLE_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_UNSTOPPABLE_TOKEN


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template<typename Token>
    concept unstoppable_token
        =  ::Beman::Execution26::stoppable_token<Token>
        && requires()
            {
                requires ::std::bool_constant<not Token::stop_possible()>::value;
            }
        ;
}

// ----------------------------------------------------------------------------

#endif



// ----------------------------------------------------------------------------

#endif
