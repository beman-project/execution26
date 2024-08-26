// src/beman/execution26/tests/stopsource-inplace-mem.pass.cpp
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/stop_token.hpp>
#include "test/execution.hpp"

auto test_inplace_stop_source_get_token() -> void
{
    // Plan:
    // - Given two inplace_stop_sources.
    // - When getting a stop_token each using get_token().
    // - Then these token compare unequal to each other but equal to
    //   other tokens get from the respective source.
    // Reference: [stopsource.inplace.mem] p1

    ::test_std::inplace_stop_source source1;
    ::test_std::inplace_stop_source source2;

    auto token1{source1.get_token()};
    auto token2{source2.get_token()};

    assert(token1 != token2);
    assert(token1 == source1.get_token());
    assert(token2 == source2.get_token());
}

auto test_inplace_stop_source_stop_requested() -> void
{
    // Plan:
    // - Given an inplace_stop_source.
    // - When getting using stop_requested() it yields false.
    // - Then after calling request_stop() stop_requested()
    //   yields true.
    // Reference: [stopsource.inplace.mem] p2

    ::test_std::inplace_stop_source source;

    assert(source.stop_requested() == false);
    source.request_stop();
    assert(source.stop_requested() == true);
}

auto test_inplace_stop_source_request_stop() -> void
{
    // Plan:
    // - Given an inplace_stop_source.
    // - When getting using stop_requested() it yields false
    //   and registered callbacks are executed..
    // - Then after calling request_stop() stop_requested()
    //   yields true.
    // Reference: [stopsource.inplace.mem] p4

    bool flag1{false};
    bool flag2{false};

    ::test_std::inplace_stop_source source;
    auto callback1 = [&flag1]{ flag1 = true; };
    auto callback2 = [&flag2]{ flag2 = true; };
    ::test_std::stop_callback_for_t<::test_std::inplace_stop_token, decltype(callback1)> cb1(source.get_token(), callback1);
    ::test_std::stop_callback_for_t<::test_std::inplace_stop_token, decltype(callback2)> cb2(source.get_token(), callback2);

    assert(source.stop_requested() == false);
    assert(flag1 == false);
    assert(flag2 == false);

    source.request_stop();

    assert(source.stop_requested() == true);
    assert(flag1 == true);
    assert(flag2 == true);
}


auto main() -> int
{
    test_inplace_stop_source_get_token();
    test_inplace_stop_source_stop_requested();
    test_inplace_stop_source_request_stop();
}
