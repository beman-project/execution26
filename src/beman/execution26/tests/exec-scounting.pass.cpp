// src/beman/execution26/tests/exec-scounting-ctor.pass.cpp           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/execution26/detail/simple_counting_scope.hpp>
#include <beman/execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>
#include <optional>

// ----------------------------------------------------------------------------

namespace
{
    auto test_scounting_ctor() -> void
    {
        static_assert(std::default_initializable<test_std::simple_counting_scope>);
        static_assert(noexcept(test_std::simple_counting_scope{}));
        static_assert(std::destructible<test_std::simple_counting_scope>);
        static_assert(not std::copy_constructible<test_std::simple_counting_scope>);
        static_assert(not std::move_constructible<test_std::simple_counting_scope>);
        static_assert(not std::assignable_from<
            test_std::simple_counting_scope&,
            test_std::simple_counting_scope
            >);
        static_assert(not std::assignable_from<
            test_std::simple_counting_scope&,
            test_std::simple_counting_scope const
            >);

        // a scope is created unused and can be destroyed:
        test_std::simple_counting_scope{};
        // a scope can be used and closed and can be destroyed:
        {
            test_std::simple_counting_scope scope{};
            scope.close();
        }
        // a scope can be joined and destroyed:
        {
            test_std::simple_counting_scope scope{};
            test_std::sync_wait(scope.join());
        }
        // a scope can be closed, joined, and destroyed:
        {
            test_std::simple_counting_scope scope{};
            scope.close();
            test_std::sync_wait(scope.join());
        }
        // a scope with an outstanding token can be destroyed:
        {
            std::optional<test_std::simple_counting_scope> scope_opt;
            scope_opt.emplace();
            auto token{scope_opt->get_token()};
            scope_opt.reset();
            test::use(token);
        }
    }

    auto test_scounting_assoc_ctor() -> void
    {
        static_assert(noexcept(test_std::simple_counting_scope::assoc{}));
        assert(test_std::simple_counting_scope::assoc{}? false: true);
    }
}

// ----------------------------------------------------------------------------

auto main() -> int
{
    test_scounting_ctor();
    test_scounting_assoc_ctor();
}