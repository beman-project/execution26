// include/Beman/Execution26/detail/sched_env.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHED_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHED_ENV

#include <Beman/Execution26/detail/get_domain.hpp>
#include <Beman/Execution26/detail/get_scheduler.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Scheduler>
    class sched_env
    {
    private:
        Scheduler sched;
    
    public:
        template <typename S>
        sched_env(S&& sched): sched(::std::forward<S>(sched)) {}

        auto query(::Beman::Execution26::get_scheduler_t const&) const noexcept
        {
            return this->sched;
        }
        auto query(::Beman::Execution26::get_domain_t const& q) const noexcept
        {
            return this->sched.query(q);
        }
    };

    template <typename Scheduler>
    sched_env(Scheduler&&) -> sched_env<::std::remove_cvref_t<Scheduler>>;
}

// ----------------------------------------------------------------------------

#endif