// include/Beman/Execution26/detail/transform_sender.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_TRANSFORM_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_TRANSFORM_SENDER

#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/default_domain.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (sizeof...(Env) < 2)
        && requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        }
        && (::std::same_as<::std::remove_cvref_t<Sender>,
            std::remove_cvref_t<decltype(::std::declval<Domain>().transform_sender(::std::declval<Sender>()))>>)
    constexpr auto transform_sender(Domain, Sender&& sender, Env const&...)
        noexcept
        -> ::Beman::Execution26::sender auto
    {
        return ::std::forward<Sender>(sender);
    }

    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (sizeof...(Env) < 2)
        && requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        }
        && (not ::std::same_as<::std::remove_cvref_t<Sender>,
            std::remove_cvref_t<decltype(::std::declval<Domain>().transform_sender(::std::declval<Sender>()))>>)
    constexpr auto transform_sender(Domain dom, Sender&& sender, Env const&... env)
        noexcept
        -> ::Beman::Execution26::sender decltype(auto)
    {
        return ::Beman::Execution26::Detail::transform_sender(
            dom,
            dom.transform_sender(::std::forward<Sender>(sender), env...),
            env...);
    }

    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (not requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        })
        && ::std::same_as<::std::remove_cvref_t<Sender>,
            ::std::remove_cvref_t<decltype(
                ::Beman::Execution26::default_domain{}.transform_sender(::std::declval<Sender>(), ::std::declval<Env>()...)
            )>>
    constexpr auto transform_sender(Domain, Sender&& sender, Env const&...)
        noexcept(noexcept(::std::forward<Sender>(sender)))
        -> ::Beman::Execution26::sender auto
    {
        return sender;
    }

    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (not requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        })
        && (not ::std::same_as<::std::remove_cvref_t<Sender>,
            ::std::remove_cvref_t<decltype(
                ::Beman::Execution26::default_domain{}.transform_sender(::std::declval<Sender>(), ::std::declval<Env>()...)
            )>>)
    constexpr auto transform_sender(Domain dom, Sender&& sender, Env const&... env)
        noexcept(noexcept(
            ::Beman::Execution26::default_domain{}.transform_sender(::std::declval<Sender>(), ::std::declval<Env>()...)
        ))
        -> ::Beman::Execution26::sender decltype(auto)
    {
        (void)dom;
        return ::Beman::Execution26::Detail::transform_sender(
            dom,
            ::Beman::Execution26::default_domain{}.transform_sender(::std::forward<Sender>(sender), env...),
            env...);
    }
}

namespace Beman::Execution26
{
    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (sizeof...(Env) < 2)
        && requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        }
        && (::std::same_as<::std::remove_cvref_t<Sender>,
            std::remove_cvref_t<decltype(::std::declval<Domain>().transform_sender(::std::declval<Sender>()))>>)
    constexpr auto transform_sender(Domain, Sender&& sender, Env const&...)
        noexcept
        -> ::Beman::Execution26::sender decltype(auto)
    {
        return ::std::forward<Sender>(sender);
    }

    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (sizeof...(Env) < 2)
        && requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        }
        && (not ::std::same_as<::std::remove_cvref_t<Sender>,
            std::remove_cvref_t<decltype(::std::declval<Domain>().transform_sender(::std::declval<Sender>()))>>)
    constexpr auto transform_sender(Domain dom, Sender&& sender, Env const&... env)
        noexcept
        -> ::Beman::Execution26::sender auto
    {
        return ::Beman::Execution26::Detail::transform_sender(
            dom,
            dom.transform_sender(::std::forward<Sender>(sender), env...),
            env...);
    }

    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (sizeof...(Env) < 2)
        && (not requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        })
        && ::std::same_as<::std::remove_cvref_t<Sender>,
            ::std::remove_cvref_t<decltype(
                ::Beman::Execution26::default_domain{}.transform_sender(::std::declval<Sender>(), ::std::declval<Env>()...)
            )>>
    constexpr auto transform_sender(Domain, Sender&& sender, Env const&...)
        noexcept(noexcept(::std::forward<Sender>(sender)))
        -> ::Beman::Execution26::sender decltype(auto)
    {
        return ::std::forward<Sender>(sender);
    }

    template <typename Domain, ::Beman::Execution26::sender Sender, typename... Env> 
        requires (sizeof...(Env) < 2)
        && (not requires(Domain dom, Sender&& sender, Env const& ... env)
        {
            dom.transform_sender(::std::forward<Sender>(sender), env...);
        })
        && (not ::std::same_as<::std::remove_cvref_t<Sender>,
            ::std::remove_cvref_t<decltype(
                ::Beman::Execution26::default_domain{}.transform_sender(::std::declval<Sender>(), ::std::declval<Env>()...)
            )>>)
    constexpr auto transform_sender(Domain dom, Sender&& sender, Env const&... env)
        noexcept(noexcept(
            ::Beman::Execution26::Detail::transform_sender(
                dom,
                ::Beman::Execution26::default_domain{}.transform_sender(::std::declval<Sender>(), ::std::declval<Env>()...),
                env...
            )
        ))
        -> ::Beman::Execution26::sender decltype(auto)
    {
        return ::Beman::Execution26::Detail::transform_sender(
            dom,
            ::Beman::Execution26::default_domain{}.transform_sender(::std::forward<Sender>(sender), env...),
            env...);
    }
}

// ----------------------------------------------------------------------------

#endif
