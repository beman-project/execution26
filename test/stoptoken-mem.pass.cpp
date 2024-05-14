// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <stop_token>
#include <memory>
#include "test/execution.hpp"
#include "test/stop_token.hpp"

void test_stop_token_swap()
{
    // Plan:
    // - Given two pairs of stop_tokens compare equal within the pair
    //   but are different between the pairse.
    // - When swapping the one element of each pair.
    // - Then the tokens compare equal to the respective other member.
    // Reference: [stoptoken.mem] p1

    ::test_std::stop_source s0, s1;
    ::test_std::stop_token pair0[] = { s0.get_token(), s0.get_token() };
    ::test_std::stop_token pair1[] = { s1.get_token(), s1.get_token() };

    assert(pair0[0] == pair0[1]);
    assert(pair1[0] == pair1[1]);
    assert(pair0[0] != pair1[1]);
    assert(pair0[1] != pair1[0]);

    pair0[1].swap(pair1[1]);

    assert(pair0[0] != pair0[1]);
    assert(pair1[0] != pair1[1]);
    assert(pair0[0] == pair1[1]);
    assert(pair0[1] == pair1[0]);
}

void test_stop_token_stop_requested()
{
    // Plan:
    // - Given a stop source and its stop token with stop_requested() == false.
    //   Also, a disengaged stop_token needs to have stop_requested() == false.
    // - When requesting stop on the source.
    // - Then the token's stop_requested() should be true. The disengaged stop
    //   token's stop_requested() remains false.
    // Reference: [stoptoken.mem] p2

    ::test_std::stop_source source;
    ::test_std::stop_token  engaged{source.get_token()};
    ::test_std::stop_token  disengaged;
    assert(engaged.stop_requested() == false);
    assert(disengaged.stop_requested() == false);

    source.request_stop();

    assert(engaged.stop_requested() == true);
    assert(disengaged.stop_requested() == false);
}

void test_stop_token_stop_possible()
{
    // Plan:
    // - Given a three stop sources:
    //     - remain_source    - which sticks around
    //     - unstopped_source - which is destroyed before stopped
    //     - stopped_source   - which is destroyed after stopped
    // - When getting a stop token for each of them (prior to destruction)
    // - Then the token's stop_possible() should be:
    //     - false for a disengaged token
    //     - true for the remain_source
    //     - false for the unstopped source
    //     - true for the stopped source
    // Reference: [stoptoken.mem] p3

    auto remain_source(::std::make_unique<::test_std::stop_source>());
    auto unstopped_source(::std::make_unique<::test_std::stop_source>());
    auto stopped_source(::std::make_unique<::test_std::stop_source>());

    auto disengaged_token{::test_std::stop_token()};
    auto remain_token{remain_source->get_token()};

    auto unstopped_token{unstopped_source->get_token()};
    unstopped_source.reset();

    auto stopped_token{stopped_source->get_token()};
    stopped_source->request_stop();
    stopped_source.reset();

    assert(disengaged_token.stop_possible() == false);
    assert(remain_token.stop_possible() == true);
    assert(unstopped_token.stop_possible() == false);
    assert(stopped_token.stop_possible() == true);
}

int main()
{
    static_assert(::test_std::stoppable_token<::test_std::stop_token>);

    test_stop_token_swap();
    test_stop_token_stop_requested();
    test_stop_token_stop_possible();
}
