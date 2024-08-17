// include/Beman/Execution26/detail/get_completion_scheduler.hpp      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SCHEDULER

#include <Beman/Execution26/detail/forwarding_query.hpp>
#include <Beman/Execution26/detail/set_error.hpp>
#include <Beman/Execution26/detail/set_stopped.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <concepts>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <typename Tag>
    struct get_completion_scheduler_t
        : ::Beman::Execution26::forwarding_query_t
    {
        template <typename Env>
        auto operator()(Env&& env) const noexcept
        {
            static_assert(noexcept(::std::as_const(env).query(*this)));
            //-dk:TODO mandate result is a scheduler:
            // static_assert(::Beman::Execution26::scheduler<
            //               decltype(::std::as_const(env).query(*this))
            //               >);
            return ::std::as_const(env).query(*this); 
        }
    };

    template <typename Tag>
        requires(
            ::std::same_as<Tag, ::Beman::Execution26::set_error_t>
            || ::std::same_as<Tag, ::Beman::Execution26::set_stopped_t>
            || ::std::same_as<Tag, ::Beman::Execution26::set_value_t>
        )
    inline constexpr get_completion_scheduler_t<Tag> get_completion_scheduler{};
}

// ----------------------------------------------------------------------------

#endif
