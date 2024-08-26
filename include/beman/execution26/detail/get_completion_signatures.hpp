// include/beman/execution26/detail/get_completion_signatures.hpp     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SIGNATURES

#include <beman/execution26/detail/completion_signatures.hpp> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct get_completion_signatures_t
    {
        template <typename Sender, typename Env>
        auto operator()(Sender&&, Env&&) const noexcept
        {
            return ::beman::execution26::completion_signatures<>{};
            //-dk:TODO implement!
        }
    };
    inline constexpr get_completion_signatures_t get_completion_signatures{};
}

// ----------------------------------------------------------------------------

#endif