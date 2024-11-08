// include/beman/execution26/detail/schedule.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE

#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/common.hpp>
#include <utility>

#include <beman/execution26/detail/suppress_push.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
struct schedule_t {
    template <typename Scheduler>
        requires(not requires(Scheduler&& sched) {
                    { ::std::forward<Scheduler>(sched).schedule() } -> ::beman::execution26::sender;
                })
    auto operator()(Scheduler&& sched) const =
        BEMAN_EXECUTION26_DELETE("the scheduler needs a schedule() member returning a sender");

    template <typename Scheduler>
        requires requires(Scheduler&& sched) {
            { ::std::forward<Scheduler>(sched).schedule() } -> ::beman::execution26::sender;
        }
    auto operator()(Scheduler&& sched) const noexcept(noexcept(std::forward<Scheduler>(sched).schedule())) {
        return std::forward<Scheduler>(sched).schedule();
    }
};

inline constexpr ::beman::execution26::schedule_t schedule{};
} // namespace beman::execution26

// ----------------------------------------------------------------------------

#include <beman/execution26/detail/suppress_pop.hpp>

#endif
