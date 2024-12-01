// include/beman/execution26/detail/bounded_queue.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_BOUNDED_QUEUE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_BOUNDED_QUEUE

#include <beman/execution26/detail/immovable.hpp>
#include <beman/execution26/detail/conqueue_errc.hpp>
#include <beman/execution26/detail/conqueue_error.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/just.hpp> //-dk:TODO remove
#include <concepts>
#include <memory>
#include <mutex>
#include <optional>
#include <condition_variable>
#include <utility>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
template <typename T, typename = ::std::allocator<T>>
class bounded_queue;
}

// ----------------------------------------------------------------------------

template <typename T, typename Allocator>
class beman::execution26::bounded_queue : ::beman::execution26::detail::immovable {
  public:
    using value_type     = T;
    using allocator_type = Allocator;

    static_assert(::std::same_as<value_type, typename Allocator::value_type>);

    explicit bounded_queue(::std::size_t max, Allocator allocator = {})
        : allocator(allocator),
          array_allocator(allocator),
          max(max),
          elements(array_allocator_traits::allocate(this->array_allocator, this->max)) {}
    ~bounded_queue() {
        for (; this->tail != this->head; ++this->tail) {
            this->destroy(this->get(this->tail));
        }
        array_allocator_traits::deallocate(this->array_allocator, this->elements, this->max);
    }

    auto is_closed() const noexcept -> bool {
        std::lock_guard cerberos(this->mutex);
        return this->closed;
    }
    auto close() noexcept -> void {
        std::lock_guard cerberos(this->mutex);
        this->closed = true;
        this->push_condition.notify_all();
        this->pop_condition.notify_all();
    }

    auto push(const T& value) -> void { this->internal_push(value); }
    auto push(T&& value) -> void { this->internal_push(::std::move(value)); }
    auto push(const T& value, ::std::error_code& ec) -> bool { return this->internal_push(value, ec); }
    auto push(T&& value, ::std::error_code& ec) -> bool { return this->internal_push(::std::move(value), ec); }
    auto try_push(const T& value, ::std::error_code& ec) -> bool { return this->internal_try_push(value, ec); }
    auto try_push(T&& value, ::std::error_code& ec) -> bool { return this->internal_try_push(::std::move(value), ec); }
    auto async_push(const T& value) -> sender auto { return internal_async_push(value); }
    auto async_push(T&& value) -> sender auto { return internal_async_push(::std::move(value)); }

    auto pop() -> T {
        ::std::unique_lock cerberus(this->mutex);
        this->pop_condition.wait(cerberus, [this] { return this->closed || this->head != this->tail; });
        if (this->closed) {
            throw ::beman::execution26::conqueue_error(::beman::execution26::conqueue_errc::closed);
        }
        element_t* element{this->get(this->tail)};
        T          rc{::std::move(element->value)};
        this->destroy(element);
        ++this->tail;
        cerberus.unlock();
        this->push_condition.notify_one();
        return rc;
    }
    auto pop(::std::error_code& ec) -> ::std::optional<T> {
        ::std::unique_lock cerberus(this->mutex);
        this->pop_condition.wait(cerberus, [this] { return this->closed || this->head != this->tail; });
        if (this->closed) {
            ec = make_error_code(::beman::execution26::conqueue_errc::closed);
            return {};
        }
        element_t*         element{this->get(this->tail)};
        ::std::optional<T> rc{::std::move(element->value)};
        this->destroy(element);
        ++this->tail;
        cerberus.unlock();
        this->push_condition.notify_one();
        return rc;
    }
    auto try_pop(::std::error_code& ec) -> ::std::optional<T> {
        ::std::unique_lock cerberus(this->mutex);
        if (this->closed) {
            ec = make_error_code(::beman::execution26::conqueue_errc::closed);
            return {};
        }
        if (this->head == this->tail) {
            ec = make_error_code(::beman::execution26::conqueue_errc::empty);
            return {};
        }
        element_t*         element{this->get(this->tail)};
        ::std::optional<T> rc{::std::move(element->value)};
        this->destroy(element);
        ++this->tail;
        cerberus.unlock();
        this->push_condition.notify_one();
        return rc;
    }
    auto async_pop() -> sender auto { return ::beman::execution26::just(T()); }

  private:
    using allocator_traits = ::std::allocator_traits<Allocator>;
    union element_t {
        element_t() {}
        template <typename... Args>
        element_t(allocator_type& alloc, Args&&... args) {
            allocator_traits::construct(alloc, &value, ::std::forward<Args>(args)...);
        }
        ~element_t() {}
        T value;
    };
    using array_allocator_type   = allocator_traits::template rebind_alloc<element_t>;
    using array_allocator_traits = allocator_traits::template rebind_traits<element_t>;

    template <typename... Args>
    auto construct(element_t* element, Args&&... args) {
        array_allocator_traits::construct(
            this->array_allocator, this->get(this->head), this->allocator, ::std::forward<Args>(args)...);
    }
    auto destroy(element_t* element) {
        allocator_traits::destroy(this->allocator, &element->value);
        array_allocator_traits::destroy(this->array_allocator, element);
    }
    auto get(::std::uint64_t index) noexcept -> element_t* { return this->elements + (index % this->max); }

    template <typename Arg>
    auto internal_push(Arg&& arg) -> void {
        ::beman::execution26::conqueue_errc error{};
        if (not this->internal_push(::std::forward<Arg>(arg), error)) {
            throw ::beman::execution26::conqueue_error(error);
        }
    }
    template <typename Arg>
    auto internal_push(Arg&& arg, ::std::error_code& ec) -> bool {
        ::beman::execution26::conqueue_errc error{};
        bool                                rc{this->internal_push(::std::forward<Arg>(arg), error)};
        if (not rc) {
            ec = make_error_code(error);
        }
        return rc;
    }
    template <typename Arg>
    auto internal_push(Arg&& arg, ::beman::execution26::conqueue_errc& error) -> bool {
        {
            ::std::unique_lock cerberus(this->mutex);
            this->push_condition.wait(cerberus,
                                      [this] { return this->closed || this->head - this->tail < this->max; });
            if (this->closed) {
                error = ::beman::execution26::conqueue_errc::closed;
                return false;
            }
            this->construct(this->get(this->head), ::std::forward<Arg>(arg));
            ++this->head;
        }
        this->pop_condition.notify_one();
        return true;
    }
    template <typename Arg>
    auto internal_try_push(Arg&& arg, ::std::error_code& ec) -> bool {
        {
            ::std::unique_lock cerberus(this->mutex);
            if (this->closed) {
                ec = make_error_code(::beman::execution26::conqueue_errc::closed);
                return false;
            }
            if (this->head - this->tail == this->max) {
                ec = make_error_code(::beman::execution26::conqueue_errc::full);
                return false;
            }
            this->construct(this->get(this->head), ::std::forward<Arg>(arg));
            ++this->head;
        }
        this->pop_condition.notify_one();
        return true;
    }
    auto internal_async_push(T&& value) -> sender auto { return ::beman::execution26::just(); }

    Allocator                 allocator;
    array_allocator_type      array_allocator;
    mutable ::std::mutex      mutex;
    ::std::condition_variable push_condition;
    ::std::condition_variable pop_condition;
    ::std::size_t             max;
    element_t*                elements;
    ::std::uint64_t           head{}; // the next element to push to
    ::std::uint64_t           tail{}; // the next element to push from
    bool                      closed{};
};

// ----------------------------------------------------------------------------

#endif
