// tests/beman/execution26/stopcallback-finite-general.test.cpp       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/finite_inplace_stop_source.hpp>
#include "test/execution.hpp"
#include <concepts>
#include <type_traits>

namespace {
    template <bool Result, std::size_t N, std::size_t I>
    auto test_i_less_than_n() -> void {
        struct callback {
            callback(int*) {}
            auto operator()() noexcept -> void {}
        };

        static_assert(Result == requires {
            typename test_std::finite_inplace_stop_callback<N, I, callback>;
        });
    }
}

TEST(stopsource_finite_general) {
    struct callback {
        callback(int*) {}
        auto operator()() noexcept -> void {}
    };
    using cb_type = test_std::finite_inplace_stop_callback<1, 0, callback>;

    int i{};
    test_std::finite_inplace_stop_token<1, 0> token;
    cb_type cb1(token, &i);

    static_assert(not std::move_constructible<cb_type>);
    static_assert(not std::copy_constructible<cb_type>);
    static_assert(not std::is_move_assignable_v<cb_type>);
    static_assert(not std::is_copy_assignable_v<cb_type>);

    test_i_less_than_n<true, 1, 0>();
    test_i_less_than_n<false, 1, 1>();
    test_i_less_than_n<true, 2, 0>();
    test_i_less_than_n<true, 2, 1>();
    test_i_less_than_n<false, 2, 2>();

    test_std::finite_inplace_stop_callback cb2(token, callback{&i});
    static_assert(std::same_as<cb_type, decltype(cb2)>);
}