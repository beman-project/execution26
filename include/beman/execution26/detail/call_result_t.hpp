// include/Beman/Execution26/detail/call_result_t.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CALL_RESULT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CALL_RESULT

#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Fun, typename... Args>
    using call_result_t = decltype(::std::declval<Fun>()(std::declval<Args>()...));
}

// ----------------------------------------------------------------------------

#endif
