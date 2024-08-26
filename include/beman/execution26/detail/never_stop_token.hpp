// include/beman/execution26/detail/never_stop_token.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NEVER_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NEVER_STOP_TOKEN

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    class never_stop_token;
}

// ----------------------------------------------------------------------------

class beman::execution26::never_stop_token
{
    struct private_callback_type
    {
        explicit private_callback_type(never_stop_token, auto&&) noexcept {}
    };
public:
    template <typename>
    using callback_type = private_callback_type;

    static constexpr auto stop_requested() noexcept  -> bool { return {}; }
    static constexpr auto stop_possible() noexcept  -> bool { return {}; }
    auto operator== (never_stop_token const&) const -> bool = default;
};

// ----------------------------------------------------------------------------

#endif
