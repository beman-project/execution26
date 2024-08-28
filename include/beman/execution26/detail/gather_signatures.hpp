// include/beman/execution26/detail/gather_signatures.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GATHER_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GATHER_SIGNATURES

#include <beman/execution26/detail/meta_filter.hpp>
#include <beman/execution26/detail/indirect_meta_apply.hpp>
#include <beman/execution26/detail/valid_completion_signatures.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename...> concept always_true = true;

    template <typename, typename> struct same_tag;
    template <typename Tag, typename R, typename... A>
    struct same_tag<Tag, R(A...)>
    {
        static constexpr bool value = ::std::same_as<Tag, R>;
    };
    template <typename Tag>
    struct bound_tag
    {
        template <typename T>
        using predicate = ::beman::execution26::detail::same_tag<Tag, T>;
    };

    template <typename, template <typename...> class>
    struct gather_signatures_apply;
    template <typename R, typename... A, template <typename...> class Transform>
    struct gather_signatures_apply<R(A...), Transform>
    {
        using type = ::beman::execution26::detail::indirect_meta_apply<::beman::execution26::detail::always_true<R>>
            ::template meta_apply<Transform, A...>
        ;
    };

    template <::beman::execution26::detail::valid_completion_signatures,
              template <typename...> class,
              template <typename...> class>
    struct gather_signatures_helper;

    template <typename... Signatures,
              template <typename...> class Tuple,
              template <typename...> class Variant>
    struct gather_signatures_helper<
        ::beman::execution26::completion_signatures<Signatures...>, Tuple, Variant>
    {
        using type = ::beman::execution26::detail::indirect_meta_apply<::beman::execution26::detail::always_true<Signatures...>>
            ::template meta_apply<Variant,
                typename ::beman::execution26::detail::gather_signatures_apply<Signatures, Tuple>::type...
            >
        ;
    };

    template <typename Tag,
              ::beman::execution26::detail::valid_completion_signatures signatures,
              template <typename...> class Tuple,
              template <typename...> class Variant>
    using gather_signatures
        = ::beman::execution26::detail::gather_signatures_helper<
            ::beman::execution26::detail::meta::filter<
                ::beman::execution26::detail::bound_tag<Tag>::template predicate,
                signatures
            >,
            Tuple,
            Variant
        >::type;
}

// ----------------------------------------------------------------------------

#endif
