// include/beman/execution26/detail/single_inplace_stop_token.hpp     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SINGLE_INPLACE_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SINGLE_INPLACE_STOP_TOKEN

// ----------------------------------------------------------------------------

namespace beman::execution26 {
    template <typename Callback>
    class single_inplace_stop_callback;
    class single_inplace_stop_token;
    class single_inplace_stop_source;
}

// ----------------------------------------------------------------------------

class beman::execution26::single_inplace_stop_token
{
public:
    template <typename Callback>
    using callback_type = ::beman::execution26::single_inplace_stop_callback<Callback>;

    single_inplace_stop_token() = default;
    auto operator== (single_inplace_stop_token const&) const -> bool  = default;
    auto stop_requested() const noexcept -> bool;
    auto stop_possible() const noexcept -> bool;
    auto swap(single_inplace_stop_token&) noexcept -> void;

private:
    ::beman::execution26::single_inplace_stop_source* source{};
};

// ----------------------------------------------------------------------------

class beman::execution26::single_inplace_stop_source
{
public:
    constexpr single_inplace_stop_source() noexcept {}
    single_inplace_stop_source(single_inplace_stop_source&&) = delete;
    single_inplace_stop_source(single_inplace_stop_source const&) = delete;
    ~single_inplace_stop_source();
    auto operator=(single_inplace_stop_source&&) -> single_inplace_stop_source& = delete;
    auto operator=(single_inplace_stop_source const&) -> single_inplace_stop_source& = delete;

    constexpr auto get_token() const noexcept -> ::beman::execution26::single_inplace_stop_token;
    static constexpr auto stop_possible() noexcept -> bool { return true; }
    auto stop_requested() const noexcept -> bool;
    auto request_stop() noexcept -> bool;
};

// ----------------------------------------------------------------------------


#endif
