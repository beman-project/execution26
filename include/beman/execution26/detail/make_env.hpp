// include/beman/execution26/detail/make_env.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_MAKE_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_MAKE_ENV

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename Query, typename Value>
class make_env {
  private:
    Value value;

  public:
    template <typename V>
    make_env(const Query&, V&& val) : value(::std::forward<V>(val)) {}
    constexpr auto query(const Query&) const noexcept -> const Value& { return this->value; }
    constexpr auto query(const Query&) noexcept -> Value& { return this->value; }
};
template <typename Query, typename Value>
make_env(Query&&, Value&& value) -> make_env<::std::remove_cvref_t<Query>, ::std::remove_cvref_t<Value>>;
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
