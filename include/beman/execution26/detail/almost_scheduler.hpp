// include/beman/execution26/detail/almost_scheduler.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ALMOST_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ALMOST_SCHEDULER

#include <beman/execution26/detail/queryable.hpp>
#include <beman/execution26/detail/schedule.hpp>
#include <beman/execution26/detail/scheduler_t.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <concepts>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
/*!
 * \brief Auxiliary concept used to break cycle for scheduler concept.
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 * \internal
 * \concept almost_scheduler
 */
template <typename Scheduler>
concept almost_scheduler = ::std::derived_from<typename ::std::remove_cvref_t<Scheduler>::scheduler_concept,
                                               ::beman::execution26::scheduler_t> &&
                           ::beman::execution26::detail::queryable<Scheduler> &&
                           requires(Scheduler&& sched) {
                               {
                                   ::beman::execution26::schedule(::std::forward<Scheduler>(sched))
                               } -> ::beman::execution26::sender;
                           } && ::std::equality_comparable<::std::remove_cvref_t<Scheduler>> &&
                           ::std::copy_constructible<::std::remove_cvref_t<Scheduler>>;
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
