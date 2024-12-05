// examples/doc-just_stopped.cpp                                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/execution.hpp>
#include <system_error>
#include <cassert>
#include <iostream> //-dk:TODO remove
namespace ex = beman::execution26;

int main() {
    bool stopped{false};

    auto result = ex::sync_wait(ex::just_stopped() | ex::upon_stopped([&] { stopped = true; }));
    assert(result);
    assert(stopped);
}