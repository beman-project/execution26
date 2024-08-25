// include/Beman/Execution26/detail/default_impls.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_IMPLS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_IMPLS

#include <Beman/Execution26/detail/fwd_env.hpp>
#include <Beman/Execution26/detail/get_env.hpp>
#include <Beman/Execution26/detail/empty_env.hpp>
#include <Beman/Execution26/detail/start.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <Beman/Execution26/detail/callable.hpp>
#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
#if __cpp_lib_forward_like < 202207
    template <typename T>
    struct forward_like_helper
    {
        template <typename U>
        static auto forward(U&& u) -> ::std::remove_cvref_t<U>&&
        {
            return ::std::forward<U&&>(u);
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
            return ::std::forward<U&&>(u);
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
        return ::Beman::Execution26::Detail::forward_like_helper<T>::forward(::std::forward<U>(u));
#endif
    }

    struct default_impls
    {
        static constexpr auto get_attrs
            = [](auto const&, auto const&... child) noexcept -> decltype(auto)
            {
                if constexpr (1 == sizeof...(child))
                    return (::Beman::Execution26::Detail::fwd_env(
                        ::Beman::Execution26::get_env(child)
                    ), ...);
                else
                    return ::Beman::Execution26::empty_env{};
            };
        static constexpr auto get_env
            = [](auto, auto&, auto const& receiver) noexcept -> decltype(auto)
            {
                return ::Beman::Execution26::Detail::fwd_env(
                    ::Beman::Execution26::get_env(receiver)
                );
            };
        static constexpr auto get_state
            = []<typename Sender, typename Receiver>(Sender&& sender, Receiver&) noexcept -> decltype(auto)
            {
                auto&& decompose = ::Beman::Execution26::Detail::get_sender_data(::std::forward<Sender>(sender));
                return ::std::forward_like<Sender>(decompose.data);
            };
        static constexpr auto start
            = [](auto&, auto&, auto&... ops) noexcept -> void
            {
                (::Beman::Execution26::start(ops), ...);
            };
        static constexpr auto complete
            = []<typename Index, typename Receiver, typename Tag, typename... Args>(
                Index, auto&, Receiver& receiver, Tag, Args&&... args) noexcept -> void
                requires ::Beman::Execution26::Detail::callable<Tag, Receiver, Args...>
            {
                static_assert(Index::value == 0);
                Tag()(::std::move(receiver), ::std::forward<Args>(args)...);
            };
    };
}

// ----------------------------------------------------------------------------

#endif
