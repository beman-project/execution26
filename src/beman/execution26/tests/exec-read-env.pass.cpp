// src/beman/execution26/tests/exec-read-env.pass.cpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/read_env.hpp>
#include <beman/execution26/detail/get_domain.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_in.hpp>
#include <beman/execution26/detail/receiver.hpp>
#include <beman/execution26/detail/connect.hpp>
#include <beman/execution26/detail/start.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct domain
    {
        int value{};
        auto operator== (domain const&) const -> bool = default;
    };

    struct env
    {
        int value{};
        auto query(test_std::get_domain_t) const noexcept -> domain
        {
            return {this->value};
        }
    };

    struct receiver
    {
        using receiver_concept = test_std::receiver_t;

        int   value{};
        bool* called{};

        auto set_value(domain d) && noexcept -> void
        {
            assert(d == domain{this->value});
            *this->called = true;
        }
        auto set_error(auto&&) && noexcept -> void
        {
            assert(nullptr == "error function was incorrectly called");
        }
        auto get_env() const noexcept -> env { return {this->value}; }
    };

    auto test_read_env() -> void
    {
        static_assert(test_std::receiver<receiver>);
        assert(domain{17} == test_std::get_domain(env{17}));
        assert(domain{17} == test_std::get_domain(test_std::get_env(receiver{17})));
        auto sender{test_std::read_env(test_std::get_domain)};
        static_assert(test_std::sender<decltype(sender)>);
        static_assert(test_std::sender_in<decltype(sender), env>);
        static_assert(std::same_as<
            test_std::completion_signatures<
                test_std::set_value_t(domain),
                test_std::set_error_t(std::exception_ptr)
            >,
            decltype(test_std::get_completion_signatures(sender, env{}))
            >);

        bool called{};
        auto op{test_std::connect(
            test_std::read_env(test_std::get_domain),
            receiver{17, &called}
            )};
        test::use(op);
        assert(not called);
        test_std::start(op);
        assert(called);
    }
}

auto main() -> int
{
    static_assert(std::same_as<test_std::read_env_t const, decltype(test_std::read_env)>);
    test_read_env();
}