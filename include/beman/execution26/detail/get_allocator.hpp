// include/Beman/Execution26/detail/get_allocator.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ALLOCATOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ALLOCATOR

#include <Beman/Execution26/detail/common.hpp>
#include <Beman/Execution26/detail/forwarding_query.hpp>
#include <Beman/Execution26/detail/simple_allocator.hpp>
#include <concepts>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct get_allocator_t
    {
        template <typename Object>
            requires (not requires(Object&& object, get_allocator_t const& tag)
            {
                ::std::as_const(object).query(tag);
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the object requires a const query(get_allocator_t) overload");
        template <typename Object>
            requires (not requires(Object const& object, get_allocator_t const& tag)
            {
                object.query(tag);
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the object requires a query(get_allocator_t) overload");
        template <typename Object>
            requires (not requires(Object const& object, get_allocator_t const& tag)
            {
                { object.query(tag) } noexcept;
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the query(get_allocator_t) overload needs to be noexcept");
        template <typename Object>
            requires (not requires(Object const& object, get_allocator_t const& tag)
            {
                { object.query(tag) } noexcept -> ::Beman::Execution26::Detail::simple_allocator<>;
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the query(get_allocator_t) overload needs to be noexcept");

        template <typename Object>
        auto operator()(Object&& object) const
        {
            return ::std::as_const(object).query(*this);
        }

        constexpr auto query(::Beman::Execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_allocator_t get_allocator{};
}

// ----------------------------------------------------------------------------

#endif
