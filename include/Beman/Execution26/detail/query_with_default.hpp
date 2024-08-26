// include/Beman/Execution26/detail/query_with_default.hpp            -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_QUERY_WITH_DEFAULT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_QUERY_WITH_DEFAULT

#include <utility>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Tag, typename Env, typename Value>
        requires requires(Tag const& tag, Env const& env)
        {
            tag(env);
        }
    constexpr auto query_with_default(Tag, Env const& env, Value&&)
        noexcept(noexcept(Tag()(env)))
        -> decltype(auto)
    {
        return Tag()(env);
    }

    template <typename Tag, typename Env, typename Value>
    constexpr auto query_with_default(Tag, Env const&, Value&& value)
        noexcept(noexcept(static_cast<Value>(std::forward<Value>(value))))
        -> decltype(auto)
    {
        return static_cast<Value>(std::forward<Value>(value));
    }
}

// ----------------------------------------------------------------------------

#endif
