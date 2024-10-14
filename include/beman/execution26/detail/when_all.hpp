// include/beman/execution26/detail/when_all.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_WHEN_ALL
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_WHEN_ALL

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/error_types_of_t.hpp>
#include <beman/execution26/detail/value_types_of_t.hpp>
#include <beman/execution26/detail/meta_size.hpp>
#include <beman/execution26/detail/meta_combine.hpp>
#include <beman/execution26/detail/meta_unique.hpp>
#include <beman/execution26/detail/meta_to.hpp>
#include <beman/execution26/detail/type_list.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/transform_sender.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/completion_signatures_of_t.hpp>
#include <beman/execution26/detail/type_list.hpp>
#include <beman/execution26/detail/set_value.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct when_all_t
    {
        template <::beman::execution26::sender... Sender>
            requires (0u != sizeof...(Sender))
                && ((::beman::execution26::detail::meta::size_v<
                        ::beman::execution26::value_types_of_t<
                            Sender,
                            ::beman::execution26::empty_env,
                            ::std::tuple,
                            ::beman::execution26::detail::type_list
                        >
                    > == 1u) && ...)
                && requires(Sender&&... s) {
                    typename ::std::common_type_t<
                        decltype(::beman::execution26::detail::get_domain_early(s))...
                    >;
                }
        auto operator()(Sender&&... sender) const
        {
            using common_t = typename ::std::common_type_t<
                 decltype(::beman::execution26::detail::get_domain_early(sender))...
            >;
           return ::beman::execution26::transform_sender(
                common_t(),
                ::beman::execution26::detail::make_sender(*this, {}, ::std::forward<Sender>(sender)...)
            );
        }
    };

    template <typename> struct when_all_value_types;
    template <typename... T>
    struct when_all_value_types<::beman::execution26::detail::type_list<T...>>
    {
        using type = ::beman::execution26::completion_signatures<
            ::beman::execution26::set_value_t(T...)
        >;
    };

    template <typename Data, typename Env, typename... Sender>
    struct completion_signatures_for_impl<
        ::beman::execution26::detail::basic_sender<
            ::beman::execution26::detail::when_all_t,
            Data,
            Sender...
            >,
        Env
        >
    {
        using value_types = typename ::beman::execution26::detail::when_all_value_types<
            ::beman::execution26::detail::meta::combine<
                    ::beman::execution26::value_types_of_t<Sender, Env, 
                    ::beman::execution26::detail::type_list,
                    ::std::type_identity_t
                >...
            >
        >::type;
        using error_types = ::beman::execution26::detail::meta::unique<
            ::beman::execution26::detail::meta::combine<
                ::beman::execution26::error_types_of_t<Sender, Env, ::beman::execution26::completion_signatures>...
            >
        >;
        using type = ::beman::execution26::detail::meta::combine<
            value_types,
            error_types
        >;
    };
}

namespace beman::execution26
{
    using ::beman::execution26::detail::when_all_t;
    inline constexpr ::beman::execution26::when_all_t when_all{};
}

// ----------------------------------------------------------------------------

#endif
