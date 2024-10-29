// src/beman/execution26/tests/notify.pass.cpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/notify.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

auto main() -> int
{
    static_assert(std::same_as<test_detail::notify_t const, decltype(test_detail::notify)>);

    {
        test_detail::notifier n;
        auto s{test_detail::notify(n)};
        static_assert(test_std::sender<decltype(s)>);
        static_assert(test_std::sender_in<decltype(s)>);
        n.complete();
        test_std::sync_wait(::std::move(s));
    }
    {
        struct receiver
        {
            using receiver_concept = test_std::receiver_t;
            bool& result;
            auto set_value() && noexcept -> void { this->result = true; }
        };
        bool finish1{};
        bool finish2{};
        test_detail::notifier n;
        auto op1{test_std::connect(test_detail::notify(n), receiver{finish1})};
        test_std::start(op1);
        auto op2{test_std::connect(test_detail::notify(n), receiver{finish2})};
        test_std::start(op2);
        assert(finish1 == false);
        assert(finish2 == false);
        n.complete();
        assert(finish1 == true);
        assert(finish2 == true);
    }

}