// include/Beman/Execution26/detail/valid_completion_for.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_COMPLETION_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_COMPLETION_FOR

#include <Beman/Execution26/functional.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <Beman/Execution26/detail/set_error.hpp>
#include <Beman/Execution26/detail/set_stopped.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename, typename> struct valid_completion_for_aux;

    template <typename Rcvr, typename Tag, typename... Args>
    struct valid_completion_for_aux<Rcvr, Tag(*)(Args...)>
    {
        static auto test(Tag(*)(Args...)) -> void
            requires ::Beman::Execution26::Detail::callable<Tag, ::std::remove_cvref_t<Rcvr>, Args...>
        {
        }
    };

    template <typename Signature, typename Rcvr>
    concept valid_completion_for
        = requires(Signature* signature)
        {
            #if 1
            valid_completion_for_aux<Rcvr, Signature*>::test(signature);
            #else
            // This definition crashes some versions of clang.
            []<typename Tag, typename... Args>(Tag(*)(Args...))
                requires ::Beman::Execution26::Detail::callable<Tag, ::std::remove_cvref_t<Rcvr>, Args...>
            {
            }(signature);
            #endif
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
