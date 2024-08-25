// include/Beman/Execution26/detail/indices_for.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_INDICES_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_INDICES_FOR

#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Sender>
    using indices_for = ::std::remove_reference_t<Sender>::indices_for;
}

// ----------------------------------------------------------------------------

#endif
