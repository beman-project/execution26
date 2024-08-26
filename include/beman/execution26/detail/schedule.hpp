// include/Beman/Execution26/detail/schedule.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULE

#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/detail/common.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct schedule_t
    {
        template <typename Scheduler>
            requires (not requires(Scheduler&& sched)
            {
                { ::std::forward<Scheduler>(sched).schedule() }
                    -> ::Beman::Execution26::sender;
            })
        auto operator()(Scheduler&& sched) const
            = BEMAN_EXECUTION26_DELETE("the scheduler needs a schedule() member returning a sender");

        template <typename Scheduler>
            requires requires(Scheduler&& sched)
            {
                { ::std::forward<Scheduler>(sched).schedule() }
                    -> ::Beman::Execution26::sender;
            }
        auto operator()(Scheduler&& sched) const
            noexcept(noexcept(std::forward<Scheduler>(sched).schedule()))
        {
            return std::forward<Scheduler>(sched).schedule();
        }
    };

    inline constexpr ::Beman::Execution26::schedule_t schedule{};
}

// ----------------------------------------------------------------------------

#endif
