// include/Beman/Execution26/detail/scheduler.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHEDULER

#include <Beman/Execution26/detail/almost_scheduler.hpp>
#include <Beman/Execution26/detail/get_completion_scheduler.hpp>
#include <Beman/Execution26/detail/get_env.hpp>
#include <Beman/Execution26/detail/schedule.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename T0, typename T1>
    concept decayed_same_as
        = ::std::same_as<::std::remove_cvref_t<T0>, ::std::remove_cvref_t<T1>>
        ;
}
namespace Beman::Execution26
{
    template <typename Scheduler>
    concept scheduler
        = ::Beman::Execution26::Detail::almost_scheduler<Scheduler>
        && requires(Scheduler&& sched) {
            {
                ::Beman::Execution26::get_completion_scheduler<::Beman::Execution26::set_value_t>(
                    ::Beman::Execution26::get_env(
                        ::Beman::Execution26::schedule(
                            ::std::forward<Scheduler>(sched)
                        )
                    )
                )
            } -> ::Beman::Execution26::Detail::decayed_same_as<Scheduler>;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
