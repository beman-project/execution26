// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include <cassert>

namespace
{
    template <typename> void type_exists() {}

    struct test_token
    {
        template <typename CallbackFn>
        using callback_type = beman::stop_callback<CallbackFn>;
    };
}

int main()
{
    assert((not beman::detail::stoppable_callback_for<int, int, int>));
    assert((not beman::detail::stoppable_source<int>));

    assert((not beman::stoppable_token<int>));
    assert((not beman::unstoppable_token<int>));

    type_exists<beman::stop_token>();
    type_exists<beman::stop_source>();

    type_exists<beman::nostopstate_t>();
    (void)beman::nostopstate_t();
    beman::nostopstate_t const& nostopstate = beman::nostopstate;

    type_exists<beman::stop_callback<decltype([]{})>>();

    type_exists<beman::never_stop_token>();

    type_exists<beman::inplace_stop_token>();
    type_exists<beman::inplace_stop_source>();
    type_exists<beman::inplace_stop_callback<decltype([]{})>>();

    type_exists<beman::stop_callback_for_t<test_token, decltype([]{})>>();
}
