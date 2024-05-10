// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_TEST_EXECUTION
#define INCLUDED_TEST_EXECUTION

#include <beman/stop_token.hpp>
#include <cassert>
#include <cstddef>

namespace test_std    = ::beman::cpp26;
namespace test_detail = ::beman::cpp26::detail;

namespace test
{
    template <typename> auto type_exists() {}

    // custom std::execution tests
    // [thread.stoptoken.intro]

    template <::test_detail::stoppable_source Source>
    auto test_stop_visible(Source& source) -> void
    {
        using Token = decltype(source.get_token());
        Token tokens[] = { source.get_token(), source.get_token(), source.get_token() };

        for (auto const& token: tokens)
        {
            assert(not token.stop_requested());
        }

        source.request_stop();

        for (auto const& token: tokens)
        {
            assert(token.stop_requested());
        }
    }

    template <::test_detail::stoppable_source Source>
    auto test_stop_callback(Source& source) -> void
    {
        struct callback
        {
            ::std::size_t* d_count;
            auto operator()() { ++*this->d_count; }
        };

        using Token = decltype(source.get_token());
        using CB    = ::test_std::stop_callback_for_t<Token, callback>;

        ::std::size_t count{};
        CB cb0(source.get_token(), callback{&count});

        assert(count == 0);
        source.request_stop();
        assert(count == 1);
        source.request_stop();
        assert(count == 1);

        CB cb1(source.get_token(), callback{&count});
        source.request_stop();
        assert(count == 1);
    }
}


#endif // INCLUDED_TEST_EXECUTION