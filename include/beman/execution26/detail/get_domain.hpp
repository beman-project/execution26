// include/beman/execution26/detail/get_domain.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN

#include <beman/execution26/detail/common.hpp>
#include <beman/execution26/detail/forwarding_query.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct get_domain_t
    {
        template <typename Object>
            requires (not requires(Object&& object, get_domain_t const& tag)
            {
                ::std::forward<Object>(object).query(tag);
            })
            && (not requires(Object&& object, get_domain_t const& tag)
            {
                ::std::as_const(object).query(tag);
            })
        auto operator()(Object&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("object neeeds a query(get_domain_t) overload");
        template <typename Object>
            requires (not requires(Object&& object, get_domain_t const& tag)
            {
                ::std::as_const(object).query(tag);
            })
        auto operator()(Object&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("query(get_domain_t) overload needs to be const");
        template <typename Object>
            requires (not requires(Object&& object, get_domain_t const& tag)
            {
                { ::std::as_const(object).query(tag) } noexcept;
            })
        auto operator()(Object&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("query(get_domain_t) overload needs to be noexcept");

        template <typename Object>
        constexpr auto operator()(Object&& object) const noexcept
        {
            return ::std::as_const(object).query(*this);
        }
        constexpr auto query(::beman::execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_domain_t get_domain{};
}

// ----------------------------------------------------------------------------

#endif
