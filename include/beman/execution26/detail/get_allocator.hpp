// include/beman/execution26/detail/get_allocator.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ALLOCATOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ALLOCATOR

#include <beman/execution26/detail/common.hpp>
#include <beman/execution26/detail/forwarding_query.hpp>
#include <beman/execution26/detail/simple_allocator.hpp>
#include <concepts>
#include <utility>

#include <beman/execution26/detail/suppress_push.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
struct get_allocator_t {
    template <typename Object>
        requires(not requires(Object&& object, const get_allocator_t& tag) { ::std::as_const(object).query(tag); })
    auto operator()(Object&&) const =
        BEMAN_EXECUTION26_DELETE("the object requires a const query(get_allocator_t) overload");
    template <typename Object>
        requires(
                    not requires(const Object& object, const get_allocator_t& tag) { object.query(tag); } &&
                    not requires(Object&& object, const get_allocator_t& tag) { ::std::as_const(object).query(tag); })
    auto
    operator()(Object&&) const = BEMAN_EXECUTION26_DELETE("the object requires a query(get_allocator_t) overload");
    template <typename Object>
        requires(not requires(const Object& object, const get_allocator_t& tag) {
                    { object.query(tag) } noexcept;
                })
    auto
    operator()(Object&&) const = BEMAN_EXECUTION26_DELETE("the query(get_allocator_t) overload needs to be noexcept");

    template <typename Object>
        requires(not requires(const Object& object, const get_allocator_t& tag) {
                    { object.query(tag) } noexcept -> ::beman::execution26::detail::simple_allocator<>;
                })
    auto
    operator()(Object&&) const = BEMAN_EXECUTION26_DELETE("the query(get_allocator_t) overload needs to be noexcept");

    template <typename Object>
        requires(requires(const Object& object, const get_allocator_t& tag) {
            { object.query(tag) } noexcept -> ::beman::execution26::detail::simple_allocator<>;
        })
    auto operator()(Object&& object) const noexcept {
        return ::std::as_const(object).query(*this);
    }

    constexpr auto query(const ::beman::execution26::forwarding_query_t&) const noexcept -> bool { return true; }
};

inline constexpr get_allocator_t get_allocator{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#include <beman/execution26/detail/suppress_pop.hpp>

#endif
