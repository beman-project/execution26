// src/Beman/Execution26/tests/stopcallback-inplace-cons.pass.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/stop_token.hpp>
#include "test/execution.hpp"

auto test_inplace_stopcallback_ctor() -> void
{
    // Plan:
    // - Given an inplace_stop_source.
    // - When constructing an inplace_stop_callback with a corresponding
    //   token and callback.
    // - Then the callback gets called when stop is requested and the
    //   callback of an inplace_stop_callback constructed afterwards gets
    //   call immediately.
    // Reference: [stopcallback.inplace.cons] p2

    bool flag1{};
    bool flag2{};

    ::test_std::inplace_stop_source source;
    ::test_std::inplace_stop_callback cb1(source.get_token(), [&flag1]{ flag1 = true; });

    assert(flag1 == false);
    assert(flag2 == false);

    source.request_stop();

    assert(flag1 == true);
    assert(flag2 == false);

    ::test_std::inplace_stop_callback cb2(source.get_token(), [&flag2]{ flag2 = true; });

    assert(flag1 == true);
    assert(flag2 == true);
}

auto test_inplace_stopcallback_dtor() -> void
{
    // Plan:
    // - Given an inplace_stop_source.
    // - When constructing two inplace_stop_callbacks with a corresponding
    //   token and callbacks and destroying on of the callbacks.
    // - Then only one callback called when stop is requested.
    // Reference: [stopcallback.inplace.cons] p3

    bool flag1{};
    bool flag2{};

    ::test_std::inplace_stop_source source;
    ::test_std::inplace_stop_callback callback1(source.get_token(), [&flag1]{ flag1 = true; });
    {
        ::test_std::inplace_stop_callback callback2(source.get_token(), [&flag2]{ flag2 = true; });

    }

    assert(flag1 == false);
    assert(flag2 == false);

    source.request_stop();

    assert(flag1 == true);
    assert(flag2 == false);
}

auto main() -> int
{
    test_inplace_stopcallback_ctor();
    test_inplace_stopcallback_dtor();
}
