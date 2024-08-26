// include/beman/execution26/detail/default_impls.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_IMPLS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_IMPLS

#include <beman/execution26/detail/fwd_env.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/start.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/callable.hpp>
#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
#if __cpp_lib_forward_like < 202207
    template <typename T>
    struct forward_like_helper
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
#endif

    template <typename T, typename U>
    auto forward_like(U&& u) noexcept -> decltype(auto)
    {
#if 202207 <= __cpp_lib_forward_like
        return ::std::forward_like<T>(::std::forward<U>(u));
#else
        return ::beman::execution26::detail::forward_like_helper<T>::forward(::std::forward<U>(u));
#endif
    }

    struct default_impls
    {
        static constexpr auto get_attrs
            = [](auto const&, auto const&... child) noexcept -> decltype(auto)
            {
                if constexpr (1 == sizeof...(child))
                    return (::beman::execution26::detail::fwd_env(
                        ::beman::execution26::get_env(child)
                    ), ...);
                else
                    return ::beman::execution26::empty_env{};
            };
        static constexpr auto get_env
            = [](auto, auto&, auto const& receiver) noexcept -> decltype(auto)
            {
                return ::beman::execution26::detail::fwd_env(
                    ::beman::execution26::get_env(receiver)
                );
            };
        static constexpr auto get_state
            = []<typename Sender, typename Receiver>(Sender&& sender, Receiver&) noexcept -> decltype(auto)
            {
                auto&& decompose = ::beman::execution26::detail::get_sender_data(::std::forward<Sender>(sender));
                return ::beman::execution26::detail::forward_like<Sender>(decompose.data);
            };
        static constexpr auto start
            = [](auto&, auto&, auto&... ops) noexcept -> void
            {
                (::beman::execution26::start(ops), ...);
            };
        static constexpr auto complete
            = []<typename Index, typename Receiver, typename Tag, typename... Args>(
                Index, auto&, Receiver& receiver, Tag, Args&&... args) noexcept -> void
                requires ::beman::execution26::detail::callable<Tag, Receiver, Args...>
            {
                static_assert(Index::value == 0);
                Tag()(::std::move(receiver), ::std::forward<Args>(args)...);
            };
    };
}

// ----------------------------------------------------------------------------

#endif
