// include/Beman/Execution26/detail/completion_domain.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_DOMAIN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_DOMAIN

#include <Beman/Execution26/detail/default_domain.hpp>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Default = ::Beman::Execution26::default_domain, typename Sender>
    constexpr auto completion_domain(Sender const&) noexcept
    {
        //-dk:TODO
    }
}

// ----------------------------------------------------------------------------

#endif
