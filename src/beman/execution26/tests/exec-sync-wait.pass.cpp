// src/beman/execution26/tests/exec-sync-wait.pass.cpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/sync_wait.hpp>

#include <beman/execution26/detail/completion_signatures.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_in.hpp>
#include <test/execution.hpp>

#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct error { int value{}; };
    struct sender
    {
        using sender_concept = test_std::sender_t;
    };

    struct sender_in
    {
        using sender_concept = test_std::sender_t;
        using completion_signatures = test_std::completion_signatures<
                test_std::set_value_t(bool, int),
                test_std::set_error_t(error),
                test_std::set_stopped_t()
            >;
        
        template <typename Receiver>
        struct state
        {
            using operation_state_concept = test_std::operation_state_t;

            std::remove_cvref_t<Receiver> receiver;

            auto start() & noexcept -> void {}
        };
        
        template <typename Receiver>
        auto connect(Receiver&& receiver) noexcept -> state<Receiver>
        {
            return {::std::forward<Receiver>(receiver)};
        }
    };

    template <bool Expect>
    auto test_has_sync_wait(auto&& sender) -> void
    {
        static_assert(Expect == requires{ test_std::sync_wait(sender); });
    }
}

auto main() -> int
{
    static_assert(std::same_as<
        test_std::sync_wait_t const,
        decltype(test_std::sync_wait)
    >);

    test_has_sync_wait<false>(sender{});
    test_has_sync_wait<true>(sender_in{});
}