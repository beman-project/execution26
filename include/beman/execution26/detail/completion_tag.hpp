// include/Beman/Execution26/detail/completion_tag.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_TAG
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_TAG

#include <Beman/Execution26/detail/set_error.hpp>
#include <Beman/Execution26/detail/set_stopped.hpp>
#include <Beman/Execution26/detail/set_value.hpp>
#include <concepts>

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Tag>
    concept completion_tag
        =  ::std::same_as<Tag, ::Beman::Execution26::set_error_t>
        || ::std::same_as<Tag, ::Beman::Execution26::set_stopped_t>
        || ::std::same_as<Tag, ::Beman::Execution26::set_value_t>
        ;
}

// ----------------------------------------------------------------------------

#endif
