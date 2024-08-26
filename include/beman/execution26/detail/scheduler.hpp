// include/beman/execution26/detail/scheduler.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULER

#include <beman/execution26/detail/almost_scheduler.hpp>
#include <beman/execution26/detail/decayed_same_as.hpp>
#include <beman/execution26/detail/get_completion_scheduler.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/schedule.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template <typename Scheduler>
    concept scheduler
        = ::beman::execution26::detail::almost_scheduler<Scheduler>
        && requires(Scheduler&& sched) {
            {
                ::beman::execution26::get_completion_scheduler<::beman::execution26::set_value_t>(
                    ::beman::execution26::get_env(
                        ::beman::execution26::schedule(
                            ::std::forward<Scheduler>(sched)
                        )
                    )
                )
            } -> ::beman::execution26::detail::decayed_same_as<Scheduler>;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
