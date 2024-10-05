// include/beman/execution26/detail/product_type.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_PRODUCT_TYPE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_PRODUCT_TYPE

#include <memory>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <::std::size_t, typename T>
    struct product_type_element
    {
        T value;
        auto operator== (product_type_element const&) const -> bool = default;
    };

    template <typename, typename...>
    struct product_type_base;

    template <::std::size_t... I, typename... T>
    struct product_type_base<::std::index_sequence<I...>, T...>
        : ::beman::execution26::detail::product_type_element<I, T>...
    {
        static constexpr ::std::size_t size() { return sizeof...(T); }

        template <::std::size_t J, typename S>
        static auto element_get(
            ::beman::execution26::detail::product_type_element<J, S>& self)
            noexcept
            -> S&
        {
            return self.value;
        }
        template <::std::size_t J, typename S>
        static auto element_get(
            ::beman::execution26::detail::product_type_element<J, S>&& self)
            noexcept
            -> S
        {
            return ::std::move(self.value);
        }
        template <::std::size_t J, typename S>
        static auto element_get(
            ::beman::execution26::detail::product_type_element<J, S> const& self)
            noexcept
            -> S const&
        {
            return self.value;
        }

        template <::std::size_t J>
        auto get() & -> decltype(auto)
        {
            return this->element_get<J>(*this);
        }
        template <::std::size_t J>
        auto get() && -> decltype(auto)
        {
            return this->element_get<J>(::std::move(*this));
        }
        template <::std::size_t J>
        auto get() const& -> decltype(auto)
        {
            return this->element_get<J>(::std::move(*this));
        }

        template <::std::size_t J, typename Allocator>
        auto to_state_element(Allocator&& alloc) && -> decltype(auto)
        {
            using type = ::std::remove_cvref_t<decltype(this->element_get<J>(::std::move(*this)))>;
            if constexpr (::std::uses_allocator_v<type, Allocator>)
                return ::std::make_obj_using_allocator<type>(alloc, this->element_get<J>(::std::move(*this)));
            else
                return this->element_get<J>(::std::move(*this));
        }
        template <::std::size_t J, typename Allocator>
        auto to_state_element(Allocator&& alloc) const& -> decltype(auto)
        {
            using type = ::std::remove_cvref_t<decltype(this->element_get<J>(::std::move(*this)))>;
            if constexpr (::std::uses_allocator_v<type, Allocator>)
                return ::std::make_obj_using_allocator<type>(alloc, this->element_get<J>(*this));
            else
                return this->element_get<J>(*this);
        }
        template <::std::size_t J, typename Allocator>
        auto to_state_element(Allocator&& alloc) & -> decltype(auto)
        {
            using type = ::std::remove_cvref_t<decltype(this->element_get<J>(::std::move(*this)))>;
            if constexpr (::std::uses_allocator_v<type, Allocator>)
                return ::std::make_obj_using_allocator<type>(alloc, this->element_get<J>(*this));
            else
                return this->element_get<J>(*this);
        }

        auto operator== (product_type_base const&) const -> bool = default;
    };

    template <typename... T>
    struct product_type
        : ::beman::execution26::detail::product_type_base<::std::index_sequence_for<T...>, T...>
    {
        template <typename Allocator, std::size_t... I>
        auto to_state(Allocator&& allocator, std::index_sequence<I...>) && -> product_type
        {
            return { ::std::move(*this).template to_state<I>(allocator)... };
        }
        template <typename Allocator, std::size_t... I>
        auto to_state(Allocator&& allocator) && -> product_type
        {
            return this->to_state(allocator, ::std::index_sequence_for<T...>());
        }
        template <typename Allocator, std::size_t... I>
        auto to_state(Allocator&& allocator, std::index_sequence<I...>) const & -> product_type
        {
            return { ((*this).template to_state_element<I>(std::forward<Allocator>(allocator)))... };
        }
        template <typename Allocator, std::size_t... I>
        auto to_state(Allocator&& allocator) const & -> product_type
        {
            return this->to_state(allocator, std::index_sequence_for<T...>());
        }
        template <typename Allocator, std::size_t... I>
        auto to_state(Allocator&& allocator, std::index_sequence<I...>) & -> product_type
        {
            return { (*this).template to_state_element<I>(allocator)... };
        }
        template <typename Allocator>
        auto to_state(Allocator&& allocator) & -> product_type
        {
            return this->to_state(allocator, ::std::index_sequence_for<T...>());
        }
    };
    template <typename... T>
    product_type(T&&...) -> product_type<::std::decay_t<T>...>;

    template <typename T>
    constexpr auto is_product_type(T const&) -> ::std::false_type { return {}; }
    template <typename... T>
    constexpr auto is_product_type(::beman::execution26::detail::product_type<T...> const&) -> ::std::true_type { return {}; }
}

namespace std
{
    template <typename...T>
    struct tuple_size<::beman::execution26::detail::product_type<T...>>
        : ::std::integral_constant<std::size_t, sizeof...(T)>
    {
    };
    template <::std::size_t I, typename...T>
    struct tuple_element<I, ::beman::execution26::detail::product_type<T...>>
    {
        using type = ::std::decay_t<decltype(::std::declval<::beman::execution26::detail::product_type<T...>>().template get<I>())>;
    };
}

// ----------------------------------------------------------------------------

#endif
