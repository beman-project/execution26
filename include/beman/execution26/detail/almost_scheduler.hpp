// include/Beman/Execution26/detail/almost_scheduler.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ALMOST_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ALMOST_SCHEDULER

#include <Beman/Execution26/detail/queryable.hpp>
#include <Beman/Execution26/detail/schedule.hpp>
#include <Beman/Execution26/detail/sender.hpp>
#include <concepts>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct scheduler_t {};
}

namespace Beman::Execution26::Detail
{
    template <typename Scheduler>
    concept almost_scheduler
        = ::std::derived_from<typename ::std::remove_cvref_t<Scheduler>::scheduler_concept,
                              ::Beman::Execution26::scheduler_t>
        && ::Beman::Execution26::Detail::queryable<Scheduler>
        && requires(Scheduler&& sched) {
            { ::Beman::Execution26::schedule(::std::forward<Scheduler>(sched)) }
                -> ::Beman::Execution26::sender;
        }
        && ::std::equality_comparable<::std::remove_cvref_t<Scheduler>>
        && ::std::copy_constructible<::std::remove_cvref_t<Scheduler>>
        ;
}

// ----------------------------------------------------------------------------

#endif
