// include/Beman/Execution26/detail/fwd_env.hpp                       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_FWD_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_FWD_ENV

#include <Beman/Execution26/detail/forwarding_query.hpp>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Env>
    struct fwd_env
    {
        Env env;
        fwd_env(Env env): env(::std::forward<Env>(env)) {}

        template <typename Query, typename... Args>
            requires (::Beman::Execution26::forwarding_query(::std::remove_cvref_t<Query>()))
        constexpr auto query(Query&& q, Args&&... args) const
        {
            return env.query(q, ::std::forward<Args>(args)...);
        }
    };
}

// ----------------------------------------------------------------------------

#endif
