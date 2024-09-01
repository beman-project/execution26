// include/beman/execution26/detail/has_completions.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_HAS_COMPLETIONS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_HAS_COMPLETIONS

#include <beman/execution26/detail/valid_completion_for.hpp>
#include <beman/execution26/detail/completion_signatures.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
#if not defined(__clang__)
    // The version of clang current (2024-09-01) installed on my Mac crashes
    // with this code - thus, there is a work-around.
    template <typename Receiver, typename Completions>
    concept has_completions
        = requires(Completions* completions)
        {
            []<::beman::execution26::detail::valid_completion_for<Receiver>... Signatures>
            (::beman::execution26::completion_signatures<Signatures...>*)
            {
            }(completions);
        }
        ;
#else
    template <typename, typename> struct has_completions_aux;
    template <typename Receiver, typename... Signature>
    struct has_completions_aux<
            Receiver,
            ::beman::execution26::completion_signatures<Signature...>
        >
    {
        static constexpr bool value
            = (::beman::execution26::detail::valid_completion_for<Signature, Receiver> && ... && true)
            ;
    };

    template <typename Receiver, typename Completions>
    concept has_completions
        = has_completions_aux<Receiver, Completions>::value
        ;
#endif
}

// ----------------------------------------------------------------------------

#endif
