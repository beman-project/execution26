// include/beman/execution26/detail/make_sender.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_MAKE_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_MAKE_SENDER

#include <beman/execution26/detail/basic_sender.hpp>
#include <beman/execution26/detail/movable_value.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct make_sender_empty {};

    template <typename Tag,
              typename Data = ::beman::execution26::detail::make_sender_empty,
              typename... Child>
        requires ::std::semiregular<Tag>
        && ::beman::execution26::detail::movable_value<Data>
        && (::beman::execution26::sender<Child> && ...)
    constexpr auto make_sender(Tag tag, Data&& data, Child&&... child)
    {
        return ::beman::execution26::detail::basic_sender<
            Tag, ::std::decay_t<Data>, ::std::decay_t<Child>...
            >{{{{tag}, {::std::forward<Data>(data)}, {child}...}}};
    }
}

// ----------------------------------------------------------------------------

#endif
