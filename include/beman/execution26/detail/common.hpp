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
/*!
 * \mainpage Asynchronous Operation Support
 *
 * This project implements the C++ support for asynchronous operations,
 * knows as _sender/receiver_ or `std::execution`.
 */

/*!
 * \namespace beman
 * \brief Namespace for Beman projects http://github.com/beman-project/beman
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 */
namespace beman {
/*!
 * \namespace beman::execution26
 * \brief Namespace for asynchronous operations and their vocabulary.
 *
 * \details
 * The beman::execution26 namespace contains various components for
 * accessing asynchronous operations.
 *
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 */
namespace execution26 {}
} // namespace beman

// ----------------------------------------------------------------------------

#endif
