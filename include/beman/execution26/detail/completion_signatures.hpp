// include/Beman/Execution26/detail/completion_signatures.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES

#include <Beman/Execution26/detail/completion_signature.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <::Beman::Execution26::Detail::completion_signature...>
    struct completion_signatures
    {
    };
}

// ----------------------------------------------------------------------------

#endif
