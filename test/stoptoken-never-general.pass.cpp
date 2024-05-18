// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include "test/execution.hpp"

auto main() -> int
{
    // Reference: [stoptoken.never.general]
    static_assert(::test_std::unstoppable_token<::test_std::never_stop_token>);

    ::test_std::never_stop_token token;
    static_assert(false == token.stop_requested());
    static_assert(false == token.stop_possible());
    static_assert(token == token);

    struct Callback
    {
        explicit Callback(int*) {}
        auto operator()() -> void {}
    };
    ::test_std::stop_callback_for_t<::test_std::never_stop_token, Callback> CB(token, nullptr);
}
