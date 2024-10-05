// include/beman/execution26/detail/default_impls.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_IMPLS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_IMPLS

#include <beman/execution26/detail/callable.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/forward_like.hpp>
#include <beman/execution26/detail/fwd_env.hpp>
#include <beman/execution26/detail/get_allocator.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/sender_decompose.hpp>
#include <beman/execution26/detail/start.hpp>

#include <memory>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
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
            = []<typename Sender, typename Receiver>(Sender&& sender, Receiver& receiver) noexcept -> decltype(auto)
            {
                auto&& decompose = ::beman::execution26::detail::get_sender_data(::std::forward<Sender>(sender));
                using type = std::remove_cvref_t<decltype(decompose.data)>;

                if constexpr (requires{
                                ::beman::execution26::get_allocator(::beman::execution26::get_env(receiver));
                              }
                              //-dk:TODO only take this branch if uses_allocator is also true
                              //&& ::std::uses_allocator_v<type,
                              //  decltype(::beman::execution26::get_allocator(::beman::execution26::get_env(receiver)))>
                              )
                {
                    auto alloc = ::beman::execution26::get_allocator(::beman::execution26::get_env(receiver));
                    if constexpr (decltype(::beman::execution26::detail::is_product_type(decompose.data))())
                    {
                        return decompose.data.make_from(
                            alloc,
                            ::beman::execution26::detail::forward_like<Sender>(decompose.data)
                        );
                    }
                    else
                    {
                        return ::std::make_obj_using_allocator<type>(
                            alloc,
                            ::beman::execution26::detail::forward_like<Sender>(decompose.data)
                        );
                    }
                }
                else
                {
                    return ::beman::execution26::detail::forward_like<Sender>(decompose.data);
                }
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
