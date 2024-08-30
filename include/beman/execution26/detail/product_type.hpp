// include/beman/execution26/detail/product_type.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_PRODUCT_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_PRODUCT_TYPE

#include <cstddef>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    // This should really be a generic implementation, possibly based on
    // http://wg21.link/p1061

    template <typename...> struct product_type;
    template <typename... T>
    product_type(T...) -> product_type<T...>;

    template <>
    struct product_type<>
    {
        static constexpr ::std::size_t size{0u};
        auto operator== (product_type const&) const -> bool = default;
    };

    template <typename T1>
    struct product_type<T1>
    {
        static constexpr ::std::size_t size{1u};
        auto operator== (product_type const&) const -> bool = default;
        T1 value1;
    };

    template <typename T1, typename T2>
    struct product_type<T1, T2>
    {
        static constexpr ::std::size_t size{2u};
        auto operator== (product_type const&) const -> bool = default;
        T1 value1;
        T2 value2;
    };

    template <typename T1, typename T2, typename T3>
    struct product_type<T1, T2, T3>
    {
        static constexpr ::std::size_t size{3u};
        auto operator== (product_type const&) const -> bool = default;
        T1 value1;
        T2 value2;
        T3 value3;
    };

    template <typename T1, typename T2, typename T3, typename T4>
    struct product_type<T1, T2, T3, T4>
    {
        static constexpr ::std::size_t size{4u};
        auto operator== (product_type const&) const -> bool = default;
        T1 value1;
        T2 value2;
        T3 value3;
        T4 value4;
    };

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    struct product_type<T1, T2, T3, T4, T5>
    {
        static constexpr ::std::size_t size{5u};
        auto operator== (product_type const&) const -> bool = default;
        T1 value1;
        T2 value2;
        T3 value3;
        T4 value4;
        T5 value5;
    };

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    struct product_type<T1, T2, T3, T4, T5, T6>
    {
        static constexpr ::std::size_t size{6u};
        auto operator== (product_type const&) const -> bool = default;
        T1 value1;
        T2 value2;
        T3 value3;
        T4 value4;
        T5 value5;
        T6 value6;
    };
}

// ----------------------------------------------------------------------------

#endif
