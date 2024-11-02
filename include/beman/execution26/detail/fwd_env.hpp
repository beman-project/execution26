// include/beman/execution26/detail/fwd_env.hpp                       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_FWD_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_FWD_ENV

#include <beman/execution26/detail/common.hpp>
#include <beman/execution26/detail/forwarding_query.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Env>
class fwd_env {
  private:
    Env env;

  public:
    fwd_env(Env&& env) : env(::std::forward<Env>(env)) {}

    template <typename Query, typename... Args>
        requires(not::beman::execution26::forwarding_query(::std::remove_cvref_t<Query>()))
    constexpr auto query(Query&& q,
                         Args&&... args) const = BEMAN_EXECUTION26_DELETE("the used query is not forwardable");

    template <typename Query, typename... Args>
        requires(::beman::execution26::forwarding_query(::std::remove_cvref_t<Query>())) &&
                requires(const Env& env, Query&& q, Args&&... args) { env.query(q, ::std::forward<Args>(args)...); }
    constexpr auto query(Query&& q, Args&&... args) const
        noexcept(noexcept(env.query(q, ::std::forward<Args>(args)...))) {
        return env.query(q, ::std::forward<Args>(args)...);
    }
};
template <typename Env>
fwd_env(Env&&) -> fwd_env<Env>;
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
