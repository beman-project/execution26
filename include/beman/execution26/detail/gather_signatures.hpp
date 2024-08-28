// include/beman/execution26/detail/gather_signatures.hpp             -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GATHER_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GATHER_SIGNATURES

#include <beman/execution26/detail/indirect_meta_apply.hpp>
#include <beman/execution26/detail/valid_completion_signatures.hpp>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename...> concept always_true = true;

    template <typename Tag,
              ::beman::execution26::detail::valid_completion_signatures,
              template <typename...> class Tuple,
              template <typename...> class Variant>
    using gather_signatures = Variant<>;
}

// ----------------------------------------------------------------------------

#endif
