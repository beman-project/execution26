// include/beman/execution26/detail/tag_of_t.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_TAG_OF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_TAG_OF

#include <beman/execution26/detail/sender_decompose.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    template <typename Sender>
    using tag_of_t = typename decltype(
            ::beman::execution26::detail::get_sender_meta(::std::declval<Sender&&>())
        )::tag_type;
}

// ----------------------------------------------------------------------------

#endif
