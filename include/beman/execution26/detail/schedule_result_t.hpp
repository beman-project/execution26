// include/beman/execution26/detail/schedule_result_t.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE_RESULT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE_RESULT

#include <beman/execution26/detail/scheduler.hpp>
#include <beman/execution26/detail/schedule.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template <::beman::execution26::scheduler Scheduler>
    using schedule_result_t
        = decltype(::beman::execution26::schedule(::std::declval<Scheduler>()));
}

// ----------------------------------------------------------------------------

#endif
