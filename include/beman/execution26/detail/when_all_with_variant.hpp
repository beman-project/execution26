// include/beman/execution26/detail/when_all_with_variant.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_WHEN_ALL_WITH_VARIANT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_WHEN_ALL_WITH_VARIANT

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/transform_sender.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/sender_for.hpp>
#include <beman/execution26/detail/forward_like.hpp>
#include <beman/execution26/detail/when_all.hpp>
#include <beman/execution26/detail/into_variant.hpp>

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct when_all_with_variant_t
    {
        // template <::beman::execution26::detail::sender_for<when_all_with_variant_t> Sender>
        template <::beman::execution26::sender Sender>
        auto transform_sender(Sender&& sender, auto&& ...) const noexcept
        {
            return ::std::forward<Sender>(sender).apply(
                [](auto&&, auto&&, auto&&... child){
                    return ::beman::execution26::when_all(
                        ::beman::execution26::into_variant(
                            ::beman::execution26::detail::forward_like<Sender>(child)
                        )...
                    );
                }
            );
        }

        template <::beman::execution26::sender... Sender>
        auto operator()(Sender&&... sender) const
        {
            using domain_t = typename ::std::common_type_t<
                 decltype(::beman::execution26::detail::get_domain_early(sender))...
            >;
            return ::beman::execution26::transform_sender(
                domain_t{},
                ::beman::execution26::detail::make_sender(
                    *this,
                    {},
                    ::std::forward<Sender>(sender)...
                )
            );
        }
    };
}

namespace beman::execution26
{
    using ::beman::execution26::detail::when_all_with_variant_t;
    inline constexpr ::beman::execution26::when_all_with_variant_t when_all_with_variant{};
}


// ----------------------------------------------------------------------------

#endif
