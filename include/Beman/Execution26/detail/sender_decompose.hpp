// include/Beman/Execution26/detail/sender_decompose.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_DECOMPOSE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SENDER_DECOMPOSE

#include <tuple>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    inline constexpr struct sender_any_t
    {
        template <typename T> constexpr operator T() const;
    } sender_any;

    template <typename Tag, typename Data, typename Children>
    struct sender_meta
    {
        using tag_type = ::std::remove_cvref_t<Tag>;
        using data_type = ::std::remove_cvref_t<Data>;
        using children_type = ::std::remove_cvref_t<Children>;
    };

    template <typename Sender>
    auto get_sender_meta(Sender&& sender)
    {
        #if 0
        //-dk:TODO should use a dynamic/language approach:
        auto&& [tag, data, ... children] = sender;
        return sender_meta<decltype(tag), decltype(data), ::std::tuple<decltype(children)...>>;
        #endif
        using sender_type = ::std::remove_cvref_t<Sender>;
        static constexpr ::Beman::Execution26::Detail::sender_any_t any{};
        if constexpr (requires(){ sender_type{ any, any, any, any, any, any }; })
        {
            auto&& [tag, data, c0, c1, c2, c3] = sender;
            return ::Beman::Execution26::Detail::sender_meta<decltype(tag), decltype(data), ::std::tuple<decltype(c0), decltype(c1), decltype(c2), decltype(c3)>>{};
        }
        else if constexpr (requires(){ sender_type{ any, any, any, any, any }; })
        {
            auto&& [tag, data, c0, c1, c2] = sender;
            return ::Beman::Execution26::Detail::sender_meta<decltype(tag), decltype(data), ::std::tuple<decltype(c0), decltype(c1), decltype(c2)>>{};
        }
        else if constexpr (requires(){ sender_type{ any, any, any, any }; })
        {
            auto&& [tag, data, c0, c1] = sender;
            return ::Beman::Execution26::Detail::sender_meta<decltype(tag), decltype(data), ::std::tuple<decltype(c0), decltype(c1)>>{};
        }
        else if constexpr (requires(){ sender_type{ any, any, any }; })
        {
            auto&& [tag, data, c0] = sender;
            return ::Beman::Execution26::Detail::sender_meta<decltype(tag), decltype(data), ::std::tuple<decltype(c0)>>{};
        }
        else if constexpr (requires(){ sender_type{ any, any }; })
        {
            auto&& [tag, data] = sender;
            return ::Beman::Execution26::Detail::sender_meta<decltype(tag), decltype(data), ::std::tuple<>>{};
        }
    }
}

namespace Beman::Execution26
{
    template <typename Sender>
    using tag_of_t = typename decltype(::Beman::Execution26::Detail::get_sender_meta(::std::declval<Sender&&>()))::tag_type;
}

// ----------------------------------------------------------------------------

#endif
