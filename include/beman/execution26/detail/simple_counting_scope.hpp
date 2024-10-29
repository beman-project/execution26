// include/beman/execution26/detail/simple_counting_scope.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_COUNTING_SCOPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_COUNTING_SCOPE

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/notify.hpp>
#include <atomic>
#include <mutex>
#include <utility>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    class simple_counting_scope;
}

// ----------------------------------------------------------------------------

class beman::execution26::simple_counting_scope
{
public:
    class token;
    class assoc;

    simple_counting_scope() = default;
    simple_counting_scope(simple_counting_scope &&) = delete;
    ~simple_counting_scope() = default;

    auto get_token() noexcept -> token;
    auto close() noexcept -> void
    {
        switch (this->state)
        {
        default:
            break;
        case state_t::unused:
            this->state = state_t::unused_and_closed;
            break;
        case state_t::open:
            this->state = state_t::closed;
            break;
        case state_t::open_and_joining:
            this->state = state_t::closed_and_joining;
            break;
        }
    }
    auto join() noexcept
    {
        bool complete{false};
        {
            ::std::lock_guard kerberos(this->mutex);
            if (0u == this->count)
            {
                this->state = state_t::joined;
                complete = true;
            }
        }
        if (complete)
        {
            n.complete();
        }
        return ::beman::execution26::detail::notify(this->n);
    }

private:
    enum class state_t
    {
        unused,
        open,
        open_and_joining,
        closed,
        closed_and_joining,
        unused_and_closed,
        joined
    };
    friend class assoc;
    auto try_associate() noexcept -> simple_counting_scope*
    {
        ::std::lock_guard lock(this->mutex);
        switch (this->state)
        {
        default:
            return nullptr;
        case state_t::unused:
            this->state = state_t::open; // fall-through!
        case state_t::open:
        case state_t::open_and_joining:
            ++this->count;
            return this;
        }
    }
    auto disassociate() noexcept -> void
    {
        {
            ::std::lock_guard lock(this->mutex);
            if (0u < --this->count)
                return;
            this->state = state_t::joined;
        }
        n.complete();
    }
    ::std::mutex                           mutex;
    ::std::size_t                          count{};
    state_t                                state{state_t::unused};
    ::beman::execution26::detail::notifier n;
};

// ----------------------------------------------------------------------------

class beman::execution26::simple_counting_scope::assoc
{
public:
    assoc() = default;
    assoc(assoc const& other) noexcept: assoc(other.scope)
    {
    }
    assoc(assoc&& other) noexcept: scope(::std::exchange(other.scope, nullptr))
    {
    }
    ~assoc()
    {
         if (this->scope)
            this->scope->disassociate();
    }

    auto operator= (assoc other) noexcept -> assoc
    {
        ::std::swap(this->scope, other.scope);
        return *this;
    }

    explicit operator bool() const noexcept { return this->scope != nullptr; }

private:
    friend class beman::execution26::simple_counting_scope::token;
    explicit assoc(beman::execution26::simple_counting_scope* scope)
        : scope(scope? scope->try_associate(): nullptr)
    {
    }
    beman::execution26::simple_counting_scope* scope{};
};

// ----------------------------------------------------------------------------

class beman::execution26::simple_counting_scope::token
{
public:
    template <::beman::execution26::sender Sender>
    auto wrap(Sender&& sender) const noexcept -> Sender&&
    {
        return ::std::forward<Sender>(sender);
    }

    auto try_associate() const -> beman::execution26::simple_counting_scope::assoc
    {
        return beman::execution26::simple_counting_scope::assoc(this->scope);
    }

private:
    friend class beman::execution26::simple_counting_scope;
    explicit token(beman::execution26::simple_counting_scope* scope) noexcept
        : scope(scope)
    {
    }
    beman::execution26::simple_counting_scope* scope;
};

// ----------------------------------------------------------------------------

inline auto beman::execution26::simple_counting_scope::get_token() noexcept
    -> beman::execution26::simple_counting_scope::token
{
    return beman::execution26::simple_counting_scope::token(this);
}

// ----------------------------------------------------------------------------

#endif
