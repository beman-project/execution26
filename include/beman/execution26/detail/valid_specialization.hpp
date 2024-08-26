// include/beman/execution26/detail/valid_specialization.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_SPECIALIZATION
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_SPECIALIZATION

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <template <typename...> class T, typename... Args>
    concept valid_specialization
        = requires { typename T<Args...>; }
        ;
}

// ----------------------------------------------------------------------------

#endif
