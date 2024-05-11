// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include "test/execution.hpp"

namespace test
{
    struct NonToken
    {
    };
    struct TokenNonCtorCallback
    {
        template <typename>
        struct callback_type
        {
        };
    };
    struct TokenWithOddCallback
    {
        template <typename>
        struct callback_type
        {
            template <typename Initializer>
            callback_type(TokenWithOddCallback const&, Initializer) {}
            template <typename Initializer>
            callback_type(TokenWithOddCallback&, Initializer) = delete;
        };
    };
    struct TokenWithValueCallback
    {
        template <typename>
        struct callback_type
        {
            template <typename Initializer>
            callback_type(TokenWithValueCallback const&, Initializer) {}
            template <typename Initializer>
            callback_type(TokenWithValueCallback&&, Initializer) = delete;
        };
    };
    struct Token
    {
        template <typename Fun>
        struct callback_type
        {
            template <typename Initializer>
            callback_type(Token const&, Initializer) {}
        };
    };
}

auto test_detail_stopppable_callback_for()
{
    struct Callback
    {
        struct Tag {};
        Callback() = default;
        explicit Callback(Tag) {}
        auto operator()() -> void {}
    };

    static_assert((not test_detail::stoppable_callback_for<int, test::Token>));
    static_assert((test_detail::stoppable_callback_for<Callback, test::Token>));

    static_assert((not test_detail::stoppable_callback_for<Callback, test::Token, int>));
    static_assert((test_detail::stoppable_callback_for<Callback, test::Token, Callback::Tag>));

    static_assert((not test_detail::stoppable_callback_for<Callback, test::NonToken>));
    static_assert((not test_detail::stoppable_callback_for<Callback, test::TokenNonCtorCallback>));
    static_assert((not test_detail::stoppable_callback_for<Callback, test::TokenWithValueCallback>));
    static_assert((not test_detail::stoppable_callback_for<Callback, test::TokenWithOddCallback>));
    static_assert((test_detail::stoppable_callback_for<Callback, test::Token>));
    static_assert((test_detail::stoppable_callback_for<Callback, test::Token, Callback::Tag>));
}

auto main() -> int
{
    test_detail_stopppable_callback_for();
}
