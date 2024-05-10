// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include "test-execution.hpp"
#include <utility>

namespace
{
    struct test_source
    {
        struct callback_base
        {
            virtual auto call() -> void = 0;
        };
        struct token 
        {
            template <typename Fun>
            struct callback_type
                : callback_base
            {
                Fun d_fun;
                callback_type(token t, Fun fun)
                    : d_fun(fun)
                {
                    t.d_source->d_callback = this;
                }
                auto call() -> void override
                {
                    this->d_fun();
                }
            };

            test_source* d_source;
            auto stop_requested() const { return this->d_source->d_value; }
        };

        bool d_value{};
        callback_base* d_callback{};

        auto get_token() -> token { return {this}; }
        auto request_stop() -> void
        {
            if (not this->d_value)
            {
                this->d_value = true;
                auto cb = ::std::exchange(this->d_callback, nullptr);
                if (cb)
                {
                    cb->call();
                }
            }
        }
    };
}

int main()
{
    assert(::test_detail::stoppable_source<test_source>);
    
    {
        ::test_source source;
        ::test::test_stop_visible(source);
    }
    {
        ::test_source source;
        ::test::test_stop_callback(source);
    }
}
