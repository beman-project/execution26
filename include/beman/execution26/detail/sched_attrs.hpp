// include/Beman/Execution26/detail/sched_attrs.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHED_ATTRS
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHED_ATTRS

#include <Beman/Execution26/detail/get_domain.hpp>
#include <Beman/Execution26/detail/get_completion_scheduler.hpp>
#include <Beman/Execution26/detail/set_error.hpp>
#include <Beman/Execution26/detail/set_stopped.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <concepts>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Scheduler>
    class sched_attrs
    {
    private:
        Scheduler sched;
    
    public:
        template <typename S>
        sched_attrs(S&& sched): sched(::std::forward<S>(sched)) {}

        template <typename Tag>
        auto query(::Beman::Execution26::get_completion_scheduler_t<Tag> const&) const noexcept
        {
            return this->sched;
        }
        auto query(::Beman::Execution26::get_domain_t const& q) const noexcept
        {
            return this->sched.query(q);
        }
    };

    template <typename Scheduler>
    sched_attrs(Scheduler&&) -> sched_attrs<::std::remove_cvref_t<Scheduler>>;
}

// ----------------------------------------------------------------------------

#endif
