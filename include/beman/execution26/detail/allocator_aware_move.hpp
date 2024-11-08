// include/beman/execution26/detail/allocator_aware_move.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_ALLOCATOR_AWARE_MOVE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_ALLOCATOR_AWARE_MOVE

#include <beman/execution26/detail/product_type.hpp>
#include <beman/execution26/detail/get_allocator.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <memory>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {
template <typename T, typename Context>
/*!
 * \brief Utility function use to move a possibly allocator aware object with an allocator from an environment.
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 * \internal
 */
auto allocator_aware_move(T&& obj, Context&& context) noexcept -> decltype(auto) {
    if constexpr (requires { ::beman::execution26::get_allocator(::beman::execution26::get_env(context)); }) {
        if constexpr (decltype(::beman::execution26::detail::is_product_type(obj))()) {
            return obj.make_from(::beman::execution26::get_allocator(::beman::execution26::get_env(context)),
                                 ::std::forward<T>(obj));
        } else {
            return ::std::make_obj_using_allocator<T>(
                ::beman::execution26::get_allocator(::beman::execution26::get_env(context)), ::std::forward<T>(obj));
        }
    } else {
        return ::std::forward<T>(obj);
    }
}
} // namespace beman::execution26::detail

// ----------------------------------------------------------------------------

#endif
