// include/Beman/Execution26/detail/schedule_result_t.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE_RESULT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE_RESULT

#include <Beman/Execution26/detail/scheduler.hpp>
#include <Beman/Execution26/detail/schedule.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <::Beman::Execution26::scheduler Scheduler>
    using schedule_result_t
        = decltype(::Beman::Execution26::schedule(::std::declval<Scheduler>()));
}

// ----------------------------------------------------------------------------

#endif
