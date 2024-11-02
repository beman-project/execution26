// include/beman/execution26/detail/common.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMMON
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMMON

// ----------------------------------------------------------------------------

#if defined(__cpp_deleted_function)
#define BEMAN_EXECUTION26_DELETE(msg) delete (msg)
#else
#define BEMAN_EXECUTION26_DELETE(msg) delete
#endif

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename...>
struct fail_type;
template <typename... T>
auto fail(T&&...) {
    fail_type<T...>{};
}
auto use(auto&&...) noexcept -> void {}
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
