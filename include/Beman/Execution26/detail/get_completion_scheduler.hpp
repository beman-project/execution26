// include/Beman/Execution26/detail/get_completion_scheduler.hpp      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SCHEDULER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SCHEDULER

#include <Beman/Execution26/detail/common.hpp>
#include <Beman/Execution26/detail/decayed_same_as.hpp>
#include <Beman/Execution26/detail/forwarding_query.hpp>
#include <Beman/Execution26/detail/get_env.hpp>
#include <Beman/Execution26/detail/schedule.hpp>
#include <Beman/Execution26/detail/set_error.hpp>
#include <Beman/Execution26/detail/set_stopped.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <Beman/Execution26/detail/almost_scheduler.hpp>
#include <concepts>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <typename Tag>
    struct get_completion_scheduler_t;

    template <typename Tag>
    struct get_completion_scheduler_t
        : ::Beman::Execution26::forwarding_query_t
    {
        template <typename Env>
            requires (not requires(get_completion_scheduler_t const& self,
                                   ::std::remove_cvref_t<Env> const& env) {
                env.query(self);
            })
        auto operator()(Env&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("The environment needs a query(get_completion_scheduler_t) member");

        template <typename Env>
            requires (requires(get_completion_scheduler_t const& self,
                                   ::std::remove_cvref_t<Env> const& env) {
                env.query(self);
            }
            && (not requires(get_completion_scheduler_t const& self,
                                   ::std::remove_cvref_t<Env> const& env) {
                { env.query(self) } noexcept;
            }))
        auto operator()(Env&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("The environment's query(get_completion_scheduler_t) has to be noexcept");

        template <typename Env>
            requires (requires(get_completion_scheduler_t const& self,
                                   ::std::remove_cvref_t<Env> const& env) {
                env.query(self);
            }
            && requires(get_completion_scheduler_t const& self,
                                   ::std::remove_cvref_t<Env> const& env) {
                { env.query(self) } noexcept;
            }
            && (not requires(get_completion_scheduler_t const& self,
                                   get_completion_scheduler_t<::Beman::Execution26::set_value_t> const& value_self,
                                   ::std::remove_cvref_t<Env> const& env) {
                {
                    env.query(self)
                } noexcept -> ::Beman::Execution26::Detail::almost_scheduler;
                {
                    ::Beman::Execution26::get_env(
                        ::Beman::Execution26::schedule(env.query(self))
                    ).query(value_self)
                } -> ::Beman::Execution26::Detail::decayed_same_as<decltype(env.query(self))>;
            }))
        auto operator()(Env&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("The environment's query(get_completion_scheduler_t) has to return a scheduler");

        template <typename Env>
            requires requires(get_completion_scheduler_t const& self,
                              get_completion_scheduler_t<::Beman::Execution26::set_value_t> const& value_self,
                              ::std::remove_cvref_t<Env> const& env) {
                {
                    env.query(self)
                } noexcept -> ::Beman::Execution26::Detail::almost_scheduler;
                {
                    ::Beman::Execution26::get_env(
                        ::Beman::Execution26::schedule(env.query(self))
                    ).query(value_self)
                } -> ::Beman::Execution26::Detail::decayed_same_as<decltype(env.query(self))>;
            }
        auto operator()(Env&& env) const noexcept
        {
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
