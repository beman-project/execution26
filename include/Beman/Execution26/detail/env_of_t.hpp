// include/Beman/Execution26/detail/env_of_t.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ENV_OF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ENV_OF

#include <Beman/Execution26/detail/get_env.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <typename T>
    using env_of_t = decltype(::Beman::Execution26::get_env(::std::declval<T>()));
}

// ----------------------------------------------------------------------------

#endif
