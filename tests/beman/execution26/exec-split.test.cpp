// src/beman/execution26/tests/split.test.cpp                         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/split.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>
#include <chrono>
#include <future>
#include <list>

// ----------------------------------------------------------------------------

struct timed_scheduler_t : beman::execution26::scheduler_t {};

class some_thread_pool {
  public:
    some_thread_pool()                                       = default;
    some_thread_pool(const some_thread_pool&)                = delete;
    some_thread_pool& operator=(const some_thread_pool&)     = delete;
    some_thread_pool(some_thread_pool&&) noexcept            = delete;
    some_thread_pool& operator=(some_thread_pool&&) noexcept = delete;

    ~some_thread_pool() {
        std::lock_guard lock(mutex_);
        for (auto& future : futures_) {
            future.get();
        }
    }

    template <class Fn>
    auto schedule(Fn&& fn) -> void {
        std::lock_guard lock(mutex_);
        futures_.emplace_back(std::async(std::launch::async, std::forward<Fn>(fn)));
    }

  private:
    std::mutex                   mutex_;
    std::list<std::future<void>> futures_;
};

struct some_thread_pool_scheduler {
    using scheduler_concept = timed_scheduler_t;

    template <class Receiver>
    struct timed_operation {
        using operation_state_concept = beman::execution26::operation_state_t;

        Receiver                              receiver_;
        some_thread_pool&                     pool_;
        std::chrono::steady_clock::time_point deadline_;

        void start() noexcept try {
            pool_.schedule([this] {
                std::this_thread::sleep_until(deadline_);
                beman::execution26::set_value(std::move(receiver_));
            });
        } catch (...) {
            beman::execution26::set_error(std::move(receiver_), std::current_exception());
        }
    };

    struct timed_sender {
        using sender_concept = beman::execution26::sender_t;

        using completion_signatures =
            beman::execution26::completion_signatures<beman::execution26::set_value_t(),
                                                      beman::execution26::set_error_t(std::exception_ptr)>;

        some_thread_pool*                     pool_;
        std::chrono::steady_clock::time_point deadline_;

        struct env {
            some_thread_pool* pool_;

            auto query(beman::execution26::get_completion_scheduler_t<beman::execution26::set_value_t>) const noexcept
                -> some_thread_pool_scheduler {
                return some_thread_pool_scheduler{*pool_};
            }
        };

        auto get_env() const noexcept { return env{pool_}; }

        template <beman::execution26::receiver_of<completion_signatures> Receiver>
        auto connect(Receiver receiver) const noexcept -> timed_operation<Receiver> {
            return timed_operation<Receiver>{std::move(receiver), *pool_, deadline_};
        }
    };

    explicit some_thread_pool_scheduler(some_thread_pool& pool) : pool_(&pool) {}

    auto now() const noexcept -> std::chrono::steady_clock::time_point { return std::chrono::steady_clock::now(); }

    auto schedule_at(std::chrono::steady_clock::time_point deadline) const noexcept -> timed_sender {
        return {pool_, deadline};
    }

    auto schedule_after(std::chrono::nanoseconds duration) const noexcept {
        return beman::execution26::let_value(beman::execution26::just(),
                                             [this, duration] { return schedule_at(now() + duration); });
    }

    auto schedule() const noexcept -> timed_sender { return schedule_at(now()); }

    friend bool operator==(const some_thread_pool_scheduler&, const some_thread_pool_scheduler&) noexcept = default;

    some_thread_pool* pool_;
};

struct NonCopyable {
    NonCopyable()                                  = default;
    NonCopyable(NonCopyable&&) noexcept            = default;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
    ~NonCopyable()                                 = default;
    NonCopyable(const NonCopyable&)                = delete;
    NonCopyable& operator=(const NonCopyable&)     = delete;
};

struct empty_env {};

void test_destroy_unused_split() {
    auto just               = beman::execution26::just(NonCopyable{});
    auto split              = beman::execution26::split(std::move(just));
    using split_sender_type = decltype(split);
    static_assert(beman::execution26::sender_in<split_sender_type, empty_env>);
}

void test_destroy_two_unused_split() {
    auto just               = beman::execution26::just(NonCopyable{});
    auto split              = beman::execution26::split(std::move(just));
    auto copy               = split;
    using split_sender_type = decltype(copy);
    static_assert(beman::execution26::sender_in<split_sender_type, empty_env>);
}

using beman::execution26::detail::type_list;
using beman::execution26::detail::meta::combine;

template <class... Args>
using to_set_value_t = type_list<beman::execution26::set_value_t(Args...)>;

void test_completion_sigs_and_sync_wait_on_split() {
    auto just          = beman::execution26::just(NonCopyable{});
    auto split         = beman::execution26::split(std::move(just));
    using split_sender = std::decay_t<decltype(split)>;
    struct empty_env {};
    using expected_value_completions = type_list<beman::execution26::set_value_t(const NonCopyable&)>;
    using value_completions = beman::execution26::value_types_of_t<split_sender, empty_env, to_set_value_t, combine>;
    static_assert(std::same_as<value_completions, expected_value_completions>);

    auto eat_completion = beman::execution26::then(split, [&](const NonCopyable&) {});
    ASSERT(beman::execution26::sync_wait(eat_completion));
}

void test_two_sync_waits_on_one_split() {
    auto        just    = beman::execution26::just(NonCopyable{});
    auto        split   = beman::execution26::split(std::move(just));
    const void* pointer = nullptr;
    auto        save_pointer =
        beman::execution26::then(split, [&](const NonCopyable& obj) { pointer = static_cast<const void*>(&obj); });
    auto assert_pointer = beman::execution26::then(
        split, [&](const NonCopyable& obj) { ASSERT(pointer == static_cast<const void*>(&obj)); });
    ASSERT(beman::execution26::sync_wait(save_pointer));
    ASSERT(beman::execution26::sync_wait(assert_pointer));
}

void test_completion_from_another_thread() {
    using namespace std::chrono_literals;
    auto context   = some_thread_pool{};
    auto scheduler = some_thread_pool_scheduler{context};
    auto split     = beman::execution26::split(scheduler.schedule_after(1ms));
    auto return_42 = beman::execution26::then(split, [] { return 42; });
    auto result    = beman::execution26::sync_wait(return_42);
    ASSERT(result.has_value());
    if (result.has_value()) {
        auto [val] = *result;
        ASSERT(val == 42);
    }
}

TEST(exec_split) {
    test_destroy_unused_split();
    test_destroy_two_unused_split();
    test_completion_sigs_and_sync_wait_on_split();
    test_two_sync_waits_on_one_split();
    test_completion_from_another_thread();
}
