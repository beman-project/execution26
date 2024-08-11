// src/Beman/Execution26/tests/exe-snd-expos.pass.cpp                 -*-C++-*-
// ----------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// ----------------------------------------------------------------------------

#include <Beman/Execution26/detail/query_with_default.hpp>
#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace
{
    struct domain
    {
        int value{};
    };
    struct default_domain
    {
        int default_value{};
    };

    struct env
    {
        int value{};
        auto query(test_std::get_domain_t const&) const noexcept { return domain{value}; }
    };

    auto test_query_with_default()
    {
        auto result1{test_detail::query_with_default(test_std::get_domain,
                                                     env{43},
                                                     default_domain{74})};
        static_assert(std::same_as<domain, decltype(result1)>);
        assert(result1.value == 43);

        auto result2{test_detail::query_with_default(test_std::get_domain,
                                                     test_std::empty_env(),
                                                     default_domain{74})};
        static_assert(std::same_as<default_domain, decltype(result2)>);
        assert(result2.default_value == 74);
    }
}

auto main() -> int
{
    test_query_with_default();
}