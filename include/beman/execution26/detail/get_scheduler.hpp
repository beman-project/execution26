// include/beman/execution26/detail/get_scheduler.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_SCHEDULER

#include <beman/execution26/detail/forwarding_query.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct get_scheduler_t
        : ::beman::execution26::forwarding_query_t
    {
        template <typename Env>
            requires requires(get_scheduler_t const& self, Env&& env)
            {
                ::std::as_const(env).query(self); 

            }
        auto operator()(Env&& env) const noexcept
        {
            static_assert(noexcept(::std::as_const(env).query(*this)));
            //-dk:TODO mandate that the result is a scheduler
            // static_assert(::beman::execution26::scheduler<
            //     decltype(::std::as_const(env).query(*this))
            // >)
            return ::std::as_const(env).query(*this); 
        }
    };

    inline constexpr get_scheduler_t get_scheduler{};
}

// ----------------------------------------------------------------------------

#endif
