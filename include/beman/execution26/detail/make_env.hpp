// include/beman/execution26/detail/make_env.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_MAKE_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_MAKE_ENV

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Query, typename Value>
    class make_env
    {
    private:
        Value value;
    
    public:
        template <typename V>
        make_env(Query const&, V&& value): value(::std::forward<V>(value)) {}
        constexpr auto query(Query const&) const noexcept -> Value const& { return this->value; }
        constexpr auto query(Query const&) noexcept -> Value & { return this->value; }
    };
    template <typename Query, typename Value>
    make_env(Query&&, Value&& value)
        -> make_env<::std::remove_cvref_t<Query>, ::std::remove_cvref_t<Value>>;
}

// ----------------------------------------------------------------------------

#endif
