// include/beman/execution26/detail/on_stop_request.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ON_STOP_REQUEST
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ON_STOP_REQUEST

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename St>
    struct on_stop_request
    {
        St&       st;
        auto operator()() const -> void { this->st.request_stop(); }
    };
}

// ----------------------------------------------------------------------------

#endif
