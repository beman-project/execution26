// examples/intro-2-hello-async.cpp                                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include "intro-timer.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

namespace ex = ::beman::execution26;
using namespace std::string_literals;
using namespace std::chrono_literals;

// ----------------------------------------------------------------------------
// Please see the explanation in docs/intro-examples.md for an explanation.

int main() {
    std::cout << std::unitbuf;
    intro::timer timer;

    // clang-format off
    auto [result] = ex::sync_wait(
        ex::when_all(
            timer.run(),
            ex::when_all(
                timer.resume_after(3s)
                    | ex::then([] { std::cout << "h\n"; return std::string("hello"); }),
                timer.resume_after(1s)
                    | ex::then([] { std::cout << ",\n"; return std::string(", "); }),
                timer.resume_after(2s)
                    | ex::then([] { std::cout << "w\n"; return std::string("world"); })
            ) | ex::then([](auto s1, auto s2, auto s3) { return s1 + s2 + s3; })
        )
    ).value_or(std::tuple(std::string("")));
    // clang-format on

    std::cout << result << "\n";
}