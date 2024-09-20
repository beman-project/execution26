// include/beman/execution26/detail/into_variant.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_INTO_VARIANT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_INTO_VARIANT

#include <beman/execution26/detail/child_type.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/default_impls.hpp>
#include <beman/execution26/detail/decayed_tuple.hpp>
#include <beman/execution26/detail/env_of_t.hpp>
#include <beman/execution26/detail/error_types_of_t.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/meta_combine.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sends_stopped.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <beman/execution26/detail/transform_sender.hpp>
#include <beman/execution26/detail/value_types_of_t.hpp>

#include <concepts>
#include <exception>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct into_variant_t
    {
        template <::beman::execution26::sender Sender>
        auto operator()(Sender&& sender) const
        {
            auto domain{::beman::execution26::detail::get_domain_early(sender)};
            (void)domain;
            return 
                ::beman::execution26::detail::make_sender(*this, {}, ::std::forward<Sender>(sender))
                ;
            //return ::beman::execution26::transform_sender(
            //    ::std::move(domain),
            //    ::beman::execution26::detail::make_sender(*this, {}, ::std::forward<Sender>(sender))
            //);
        }
    };

    template<>
    struct impls_for<::beman::execution26::detail::into_variant_t>
        : ::beman::execution26::detail::default_impls
    {
        static constexpr auto get_state
            = []<typename Sender, typename Receiver>(Sender&&, Receiver&&) noexcept
                -> ::std::type_identity<
                        ::beman::execution26::value_types_of_t<
                            ::beman::execution26::detail::child_type<Sender>,
                            ::beman::execution26::env_of_t<Receiver>
                        >
                    >
            {
                return {};
            };
        static constexpr auto complete
            = []<typename State, typename Tag, typename... Args>
              (auto, State, auto& receiver, Tag, Args&&... args) noexcept
                -> void
            {
                if constexpr (::std::same_as<Tag, ::beman::execution26::set_value_t>)
                {
                    using variant_type = typename State::type;
                    using tuple_type = ::beman::execution26::detail::decayed_tuple<Args...>;
                    try
                    {
                        if constexpr (sizeof...(Args) == 0u)
                            ::beman::execution26::set_value(
                                ::std::move(receiver)
                            );
                        else
                            ::beman::execution26::set_value(
                                ::std::move(receiver),
                                variant_type(tuple_type{::std::forward<Args>(args)...})
                            );
                    }
                    catch(...)
                    {
                        ::beman::execution26::set_error(
                            ::std::move(receiver),
                            ::std::current_exception()
                            );
                    }
                    
                }
                else
                {
                    Tag()(::std::move(receiver), ::std::forward<Args>(args)...);
                }
            };
    };

    template <typename Sender, typename State, typename Env>
    struct completion_signatures_for_impl<
        ::beman::execution26::detail::basic_sender<
            ::beman::execution26::detail::into_variant_t,
            State,
            Sender
            >,
        Env
        >
    {
        using variant_type = ::beman::execution26::value_types_of_t<Sender, Env>;
        using value_types = ::std::conditional_t<
                ::std::same_as<variant_type, ::beman::execution26::detail::empty_variant>,
                ::beman::execution26::completion_signatures<>,
                ::beman::execution26::completion_signatures<
                    ::beman::execution26::set_value_t(variant_type)
                >
            >;
        template <typename... E>
        using make_error_types
            = ::beman::execution26::completion_signatures<::beman::execution26::set_error_t(E)...>;

        using error_types
        = ::beman::execution26::error_types_of_t<
            Sender,
            Env,
            make_error_types
            >;
        using stopped_types
            = ::std::conditional_t< 
                ::beman::execution26::sends_stopped<Sender, Env>,
                ::beman::execution26::completion_signatures<::beman::execution26::set_stopped_t()>,
                ::beman::execution26::completion_signatures<>
                >;
        using type = ::beman::execution26::detail::meta::combine<
            value_types,
            ::beman::execution26::detail::meta::combine<error_types, stopped_types>
            >;
    };
}

namespace beman::execution26
{
    using into_variant_t = ::beman::execution26::detail::into_variant_t;
    inline constexpr into_variant_t into_variant{};
}

// ----------------------------------------------------------------------------

#endif
