// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/stop_token.hpp>
#include "test/execution.hpp"

auto test_stopsource_swap() -> void
{
    // Plan:
    //  - Given two engaged stop sources.
    //  - When two corresponding tokens are obtained.
    //  - Then after swapping the source the tokens compare suitably
    //    equal to the sources's get_token()
    // Reference: [stopsource.mem] p1

    ::test_std::stop_source source1;
    ::test_std::stop_source source2;

    auto token1{source1.get_token()};
    auto token2{source2.get_token()};
    assert(token1 == source1.get_token());
    assert(token1 != source2.get_token());
    assert(token2 != source1.get_token());
    assert(token2 == source2.get_token());

    source1.swap(source2);

    assert(token1 != source1.get_token());
    assert(token1 == source2.get_token());
    assert(token2 == source1.get_token());
    assert(token2 != source2.get_token());
}

auto test_stopsource_get_token() -> void
{
    // Plan:
    //  - Given an engaged and a disengaged stop sources.
    //  - When two corresponding tokens are obtained.
    //  - Then get_token() for the engaged stop source returns
    //    a non-default constructed token while the disengaged
    //    source returns a token comparing equal to a default
    //    constructed token.
    // Reference: [stopsource.mem] p2

    ::test_std::stop_source engaged;
    ::test_std::stop_source disengaged(::test_std::nostopstate);

    assert(engaged.get_token() == engaged.get_token());
    assert(engaged.get_token() != ::test_std::stop_token());
    assert(engaged.get_token() != disengaged.get_token());
    assert(disengaged.get_token() == ::test_std::stop_token());
}

auto test_stopsource_stop_possible() -> void
{
    // Plan:
    //  - Given an engaged and a disengaged stop sources.
    //  - When using stop_possible()
    //  - Then the result is true for the engaged stop source (both
    //    before request_stop() was calle and after) while
    //    the result is false for the disengaged stop source.
    // Reference: [stopsource.mem] p3

    ::test_std::stop_source engaged;
    ::test_std::stop_source disengaged(::test_std::nostopstate);

    assert(engaged.stop_possible());
    engaged.request_stop();
    assert(engaged.stop_possible());

    assert(not disengaged.stop_possible());
    disengaged.request_stop();
    assert(not disengaged.stop_possible());
}

auto test_stopsource_stop_requested() -> void
{
    // Plan:
    //  - Given an engaged and a disengaged stop sources.
    //  - When using stop_requested()
    //  - Then the result is true for the engaged stop source
    //    after a call to request_stop() but false before.
    //    The result is false for the disengaged stop source.
    // Reference: [stopsource.mem] p4

    ::test_std::stop_source engaged;
    ::test_std::stop_source disengaged(::test_std::nostopstate);

    assert(not engaged.stop_requested());
    engaged.request_stop();
    assert(engaged.stop_requested());

    assert(not disengaged.stop_requested());
    disengaged.request_stop();
    assert(not disengaged.stop_requested());
}

auto test_stopsource_request_stop() -> void
{
    // Plan:
    //  - Given an engaged and a disengaged stop sources.
    //  - When using request_stop()
    //  - Then the result is true for the first call of the engaged
    //    stop source. All subsequent calls on the engaged stop source
    //    and all calls on the disengaged stop source return false.
    // Reference: [stopsource.mem] p5 dnd [stopsource.concepts] p8

    ::test_std::stop_source engaged;
    ::test_std::stop_source disengaged(::test_std::nostopstate);

    assert(not engaged.stop_requested());
    auto res1{engaged.request_stop()};
    assert(res1);
    assert(engaged.stop_requested());
    auto res2{engaged.request_stop()};
    assert(res2 == false);
    assert(engaged.stop_requested());

    assert(not disengaged.stop_requested());
    auto res3{disengaged.request_stop()};
    assert(res3 == false);
    assert(not disengaged.stop_requested());
    auto res4{disengaged.request_stop()};
    assert(res4 == false);
    assert(not disengaged.stop_requested());
}

auto main() -> int
{
    test_stopsource_swap();
    test_stopsource_get_token();
    test_stopsource_stop_possible();
    test_stopsource_stop_requested();
    test_stopsource_request_stop();
}
