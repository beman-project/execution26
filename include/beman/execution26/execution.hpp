// include/beman/execution26/execution.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_EXECUTION
#define INCLUDED_BEMAN_EXECUTION26_EXECUTION

// ----------------------------------------------------------------------------

#include <beman/execution26/detail/queryable.hpp>
#include <beman/execution26/detail/empty_env.hpp>
#include <beman/execution26/detail/forwarding_query.hpp>
#include <beman/execution26/detail/get_allocator.hpp>
#include <beman/execution26/detail/get_stop_token.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/get_domain.hpp>

#include <beman/execution26/detail/completion_signature.hpp>
#include <beman/execution26/detail/completion_signatures.hpp>
#include <beman/execution26/detail/valid_completion_signatures.hpp>
#include <beman/execution26/detail/movable_value.hpp>
#include <beman/execution26/detail/matching_sig.hpp>
#include <beman/execution26/detail/as_except_ptr.hpp>

#include <beman/execution26/detail/receiver.hpp>
#include <beman/execution26/detail/valid_completion_for.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/start.hpp>
#include <beman/execution26/detail/scheduler.hpp>
#include <beman/execution26/detail/schedule.hpp>

// ----------------------------------------------------------------------------

#endif