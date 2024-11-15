// tests/beman/execution26/stopcallback-finite-cons.test.cpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include "test/execution.hpp"

namespace {
    enum class action { none, ctor, call };
    struct callback {
        action& what;
        explicit callback(action& what): what(what) { what = action::ctor; }
        auto operator()() noexcept -> void { this->what = action::call; }
    };

    auto test_disengaged_ctor() -> void {
        action what{action::none};
        test_std::finite_inplace_stop_token<1, 0> token{};
        ASSERT(what == action::none);
        test_std::finite_inplace_stop_callback    cb(token, callback{what});
        ASSERT(what != action::call);
    }
    auto test_engaged_ctor() -> void {
        test_std::finite_inplace_stop_source<3> source;
        auto token0{source.get_token<0>()};
        auto token2{source.get_token<2>()};
        action what0{action::none};
        action what2{action::none};

        test_std::finite_inplace_stop_callback cb0(token0, callback{what0});
        test_std::finite_inplace_stop_callback<3, 2, callback> cb2(token2, what2);
        ASSERT(what0 == action::ctor);
        ASSERT(what2 == action::ctor);

        source.request_stop();
        ASSERT(what0 == action::call);
        ASSERT(what2 == action::call);
    }

    auto test_engaged_dtor() -> void {
        test_std::finite_inplace_stop_source<3> source;
        auto token0{source.get_token<0>()};
        auto token2{source.get_token<2>()};
        action what0{action::none};
        action what2{action::none};

        {
            test_std::finite_inplace_stop_callback cb0(token0, callback{what0});
            test_std::finite_inplace_stop_callback<3, 2, callback> cb2(token2, what2);
            ASSERT(what0 == action::ctor);
            ASSERT(what2 == action::ctor);
        }
        {
            test_std::finite_inplace_stop_callback cb0(token0, callback{what0});
            test_std::finite_inplace_stop_callback<3, 2, callback> cb2(token2, what2);
            ASSERT(what0 == action::ctor);
            ASSERT(what2 == action::ctor);
        }

        source.request_stop();
        ASSERT(what0 == action::ctor);
        ASSERT(what2 == action::ctor);
    }
}

TEST(stopsource_finite_cons) {
    test_disengaged_ctor();
    test_engaged_ctor();
    test_engaged_dtor();
}