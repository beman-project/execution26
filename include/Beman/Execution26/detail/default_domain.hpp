// include/Beman/Execution26/detail/default_domain.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_DOMAIN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DEFAULT_DOMAIN

#include <Beman/Execution26/detail/queryable.hpp>
#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/sender_decompose.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct default_domain
    {
        template <::Beman::Execution26::sender Sender,
                  ::Beman::Execution26::Detail::queryable... Env>
            requires (sizeof...(Env) <= 1)
            && requires(Sender&& sender, Env&&... env)
            {
                ::Beman::Execution26::tag_of_t<Sender>().transform_sender(::std::forward<Sender>(sender), ::std::forward<Env>(env)...);
            }
        static constexpr auto transform_sender(Sender&& sender, Env&&... env)
            noexcept(noexcept(::Beman::Execution26::tag_of_t<Sender>().transform_sender(::std::forward<Sender>(sender), ::std::forward<Env>(env)...)))
            -> ::Beman::Execution26::sender decltype(auto)
        {
            return ::Beman::Execution26::tag_of_t<Sender>().transform_sender(::std::forward<Sender>(sender), ::std::forward<Env>(env)...);
        }

        template <::Beman::Execution26::sender Sender,
                  ::Beman::Execution26::Detail::queryable... Env>
            requires (sizeof...(Env) <= 1)
            && (not requires(Sender&& sender, Env&&... env)
            {
                ::Beman::Execution26::tag_of_t<Sender>().transform_sender(::std::forward<Sender>(sender), ::std::forward<Env>(env)...);
            })
        static constexpr auto transform_sender(Sender&& sender, Env&&...)
            noexcept(noexcept(::std::forward<Sender>(sender)))
            -> ::Beman::Execution26::sender decltype(auto)
        {
            return ::std::forward<Sender>(sender);
        }

        template <::Beman::Execution26::sender Sender,
                  ::Beman::Execution26::Detail::queryable Env>
            requires requires(Sender&& sender, Env&& env)
            {
                ::Beman::Execution26::tag_of_t<Sender>().transform_env(::std::forward<Sender>(sender), ::std::forward<Env>(env));
            }
        static constexpr auto transform_env(Sender&& sender, Env&& env) noexcept
            -> ::Beman::Execution26::Detail::queryable decltype(auto)
        {
            return ::Beman::Execution26::tag_of_t<Sender>().transform_env(::std::forward<Sender>(sender), ::std::forward<Env>(env));
        }

        template <::Beman::Execution26::sender Sender,
                  ::Beman::Execution26::Detail::queryable Env>
            requires (not requires(Sender&& sender, Env&& env)
            {
                ::Beman::Execution26::tag_of_t<Sender>().transform_env(::std::forward<Sender>(sender), ::std::forward<Env>(env));
            })
        static constexpr auto transform_env(Sender&&, Env&& env) noexcept
            -> ::Beman::Execution26::Detail::queryable decltype(auto)
        {
            return static_cast<Env>(::std::forward<Env>(env));
        }

        template <typename Tag, ::Beman::Execution26::sender Sender, typename... Args>
            requires requires(Sender&& sender, Args&&... args)
            {
                Tag().apply_sender(::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
            }
        static constexpr auto apply_sender(Tag, Sender&& sender, Args&&... args)
            noexcept(noexcept(Tag().apply_sender(::std::forward<Sender>(sender), ::std::forward<Args>(args)...)))
            -> decltype(auto)
        {
            return Tag().apply_sender(::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
        }
    };
}

// ----------------------------------------------------------------------------

#endif
