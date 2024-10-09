// include/beman/execution26/detail/sched_env.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHED_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SCHED_ENV

#include <beman/execution26/detail/get_domain.hpp>
#include <beman/execution26/detail/get_scheduler.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Scheduler>
    class sched_env
    {
    private:
        Scheduler sched;
    
    public:
        template <typename S>
        explicit sched_env(S&& sch): sched(::std::forward<S>(sch)) {}

        auto query(::beman::execution26::get_scheduler_t const&) const noexcept
        {
            return this->sched;
        }
        auto query(::beman::execution26::get_domain_t const& q) const noexcept
        {
            if constexpr (requires{ this-> sched.query(q); })
                return this->sched.query(q);
            else
                return ::beman::execution26::default_domain{};
        }
    };

    template <typename Scheduler>
    sched_env(Scheduler&&) -> sched_env<::std::remove_cvref_t<Scheduler>>;
}

// ----------------------------------------------------------------------------

#endif
