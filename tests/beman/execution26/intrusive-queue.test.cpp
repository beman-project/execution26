// tests/beman/execution26/intrusive-queue.test.cpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/intrusive_queue.hpp>
#include <array>
#include <test/execution.hpp>

// ----------------------------------------------------------------------------

namespace {
struct node {
    int   value;
    node* n_{};
};
} // namespace

TEST(intrusive_queue) {
    test_detail::intrusive_queue<&node::n_> queue;
    ::std::array<node, 5>                   n{node{1}, node{2}, node{3}, node{4}, node{5}};
    ASSERT(queue.empty());

    queue.push(&n[0]);
    ASSERT(not queue.empty());
    node* n0{queue.pop()};
    ASSERT(n0 == &n[0]);
    ASSERT(n0->n_ == nullptr);
    ASSERT(queue.empty());

    for (int i = 0; i != 5; ++i) {
        queue.push(&n[i]);
    }
    for (int i = 0; i != 5; ++i) {
        ASSERT(not queue.empty());
        ASSERT(queue.pop() == &n[i]);
    }
    ASSERT(queue.empty());
}
