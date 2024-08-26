// src/beman/execution26/tests/exec-connect.pass.cpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/connect.hpp>
#include <beman/execution26/detail/receiver.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    enum class kind { plain, domain };
    template <kind, test_std::receiver Receiver>
    struct state
    {
        using operation_state_concept = test_std::operation_state_t;
        int                           value{};
        std::remove_cvref_t<Receiver> receiver;

        template <typename R>
        state(int value, R&& r)
            : value(value)
            , receiver(std::forward<R>(r))
        {
        }
        state(state&&) = delete;
        auto start() noexcept -> void {}
    };

    struct sender
    {
        using sender_concept = test_std::sender_t;
        int value{};

        template <typename Receiver>
        auto connect(Receiver&& receiver) -> state<kind::plain, Receiver>
        {
            return state<kind::plain, Receiver>(this->value, std::forward<Receiver>(receiver));
        }
    };

    struct rvalue_sender
    {
        using sender_concept = test_std::sender_t;
        int value{};

        rvalue_sender(int value): value(value) {}
        rvalue_sender(rvalue_sender&&) = default;

        template <typename Receiver>
        auto connect(Receiver&& receiver) && -> state<kind::plain, Receiver>
        {
            return state<kind::plain, Receiver>(this->value, std::forward<Receiver>(receiver));
        }
    };

    struct receiver
    {
        using receiver_concept = test_std::receiver_t;
        int value{};

        receiver(int value): value(value) {}
        receiver(receiver&&) = default;
        auto operator== (receiver const&) const -> bool = default;
    };

    struct domain_sender
    {
        using sender_concept = test_std::sender_t;
        int value{};

        template <typename Receiver>
        auto connect(Receiver&& receiver) && -> state<kind::domain, Receiver>
        {
            return state<kind::domain, Receiver>(this->value, std::forward<Receiver>(receiver));
        }
    };

    struct domain
    {
        auto transform_sender(auto&& sender, auto&&...) const noexcept -> domain_sender
        {
            return domain_sender{sender.value};
        }
    };

    struct domain_env
    {
        auto query(test_std::get_domain_t const&) const noexcept -> domain { return {}; }
    };

    struct domain_receiver
    {
        using receiver_concept = test_std::receiver_t;
        int value{};

        domain_receiver(int value): value(value) {}
        domain_receiver(domain_receiver&&) = default;
        auto operator== (domain_receiver const&) const -> bool = default;

        auto get_env() const noexcept -> domain_env { return {}; }
    };
}

auto main() -> int
{
    static_assert(std::same_as<test_std::connect_t const, decltype(test_std::connect)>);

    static_assert(test_std::operation_state<state<kind::plain, receiver>>);
    static_assert(test_std::sender<sender>);
    static_assert(test_std::sender<rvalue_sender>);
    static_assert(test_std::receiver<receiver>);

    {
        static_assert(std::same_as<state<kind::plain, receiver>,
            decltype(test_std::connect(sender{43}, receiver(17)))>);
        auto op{test_std::connect(sender{43}, receiver(17))};
        assert(op.value == 43);
        assert(op.receiver == receiver(17));
    }

    {
        static_assert(std::same_as<state<kind::plain, receiver>,
            decltype(test_std::connect(rvalue_sender(42), receiver(17)))>);
        auto op{test_std::connect(rvalue_sender(42), receiver(17))};
        assert(op.value == 42);
        assert(op.receiver == receiver(17));
    }

    {
        static_assert(std::same_as<domain, decltype(test_std::get_domain(domain_env()))>);
        static_assert(std::same_as<domain_env, decltype(test_std::get_env(domain_receiver(17)))>);
        static_assert(std::same_as<domain_sender,
            decltype(domain().transform_sender(sender{42}, domain_receiver(17)))>);
        static_assert(std::same_as<domain_sender,
            decltype(test_std::transform_sender(domain(), sender{42}, domain_env()))>);

        static_assert(std::same_as<state<kind::domain, domain_receiver>,
            decltype(test_std::connect(sender{42}, domain_receiver(17)))>);
        auto op{test_std::connect(sender{42}, domain_receiver(17))};
        assert(op.value == 42);
        assert(op.receiver == domain_receiver(17));
    }
}