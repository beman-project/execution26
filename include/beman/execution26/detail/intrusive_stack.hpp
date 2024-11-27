// include/beman/execution26/detail/intrusive_queue.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_INTRUSIVE_QUEUE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_INTRUSIVE_QUEUE

#include <cassert>
#include <utility>

namespace beman::execution26::detail {

template <auto Next>
class atomic_intrusive_stack;

template <auto Next>
class intrusive_stack;

//! @brief  This data structure is an intrusive queue that is not thread-safe.
template <class Item, Item* Item::*Next>
class intrusive_stack<Next> {
  public:
    //! @brief  Pushes an item to the queue.
    auto push(Item* item) noexcept -> void { item->*Next = std::exchange(head_, item); }

    //! @brief  Pops one item from the queue.
    //!
    //! @return  The item that was popped from the queue, or nullptr if the queue is empty.
    auto pop() noexcept -> Item* {
        if (head_) {
            auto item = head_;
            head_     = std::exchange(item->*Next, nullptr);
            return item;
        }
        return nullptr;
    }

    //! @brief  Tests if the queue is empty.
    auto empty() const noexcept -> bool { return !head_; }

  private:
    friend class atomic_intrusive_stack<Next>;
    Item* head_{nullptr};
};

} // namespace beman::execution26::detail

#endif