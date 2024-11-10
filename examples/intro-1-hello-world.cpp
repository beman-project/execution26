// examples/intro-1-hello-world.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <iostream>
#include <string>
#include <tuple>

namespace ex = ::beman::execution26;
using namespace std::string_literals;

// ----------------------------------------------------------------------------
// Please see the explanation in docs/intro-examples.md for an explanation.

int main() {
    // clang-format off
    auto [result] = ex::sync_wait(
        ex::when_all(
            ex::just("hello, "s),
            ex::just("world"s)
        ) | ex::then([](auto s1, auto s2) { return s1 + s2; })
        ).value_or(std::tuple(""s)
    );
    // clang-format on

    std::cout << result << '\n';
}