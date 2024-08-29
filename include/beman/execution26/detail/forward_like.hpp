// include/beman/execution26/detail/forward_like.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_FORWARD_LIKE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_FORWARD_LIKE

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------
// std::forward_like() doesn't work on some compilers, yet. This header
// provides a work-around.

namespace beman::execution26::detail
{
    template <typename> struct forward_like_helper;

    template <typename T>
    struct forward_like_helper
    {
        template <typename U>
        static auto forward(U&& u) -> ::std::remove_reference_t<U>&&
        {
            return ::std::move(u);
        }
    };
    template <typename T>
    struct forward_like_helper<T&&>
    {
        template <typename U>
        static auto forward(U&& u) -> ::std::remove_cvref_t<U>&&
        {
            return ::std::move(u);
        }
    };
    template <typename T>
    struct forward_like_helper<T&>
    {
        template <typename U>
        static auto forward(U&& u) -> ::std::remove_cvref_t<U>&
        {
            return ::std::forward<U&&>(u);
        }
    };
    template <typename T>
    struct forward_like_helper<T const&&>
    {
        template <typename U>
        static auto forward(U&& u) -> ::std::remove_cvref_t<U> const&&
        {
            return ::std::move(u);
        }
    };
    template <typename T>
    struct forward_like_helper<T const&>
    {
        template <typename U>
        static auto forward(U&& u) -> ::std::remove_cvref_t<U> const&
        {
            return ::std::forward<U&&>(u);
        }
    };

    // The overload own_forward_like is used for testing on systems
    // which actually do provide an implementation.
    template <typename T, typename U>
    auto own_forward_like(U&& u) noexcept -> decltype(auto)
    {
        return ::beman::execution26::detail::forward_like_helper<T>::forward(::std::forward<U>(u));
    }

    template <typename T, typename U>
    auto forward_like(U&& u) noexcept -> decltype(auto)
    {
#if 202207 <= __cpp_lib_forward_like
        return ::std::forward_like<T>(::std::forward<U>(u));
#else
        return ::beman::execution26::detail::forward_like_helper<T>::forward(::std::forward<U>(u));
#endif
    }

}

// ----------------------------------------------------------------------------

#endif
