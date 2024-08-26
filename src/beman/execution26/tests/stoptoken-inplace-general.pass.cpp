// src/beman/execution26/tests/stoptoken-inplace-general.pass.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"
#include <type_traits>
#include <cassert>

auto main() -> int
{
    // Reference [stoptoken.inplace.general]
    static_assert(::test_std::stoppable_token<::test_std::inplace_stop_token>);
    static_assert(::std::is_swappable_v<::test_std::inplace_stop_token>);

    ::test_std::inplace_stop_token token, token2;
    assert(false == token.stop_requested());
    assert(false == token.stop_possible());
    token.swap(token2);
    assert(token == token2);

    struct Callback
    {
        explicit Callback(int*) {}
        auto operator()() const -> void {}
    };
    ::test_std::stop_callback_for_t<::test_std::inplace_stop_token, Callback> cb(token, nullptr);
}
