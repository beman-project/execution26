// include/beman/execution26/detail/suppress_push.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// ----------------------------------------------------------------------------

#if defined(__GNUC__)
#define BEMAN_EXECUTION26_DIAGNOSTIC_PUSHED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#if defined(__clang__)
#define BEMAN_EXECUTION26_DIAGNOSTIC_PUSHED
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wc++26-extensions"
#endif