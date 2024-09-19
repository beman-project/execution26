# cmake-format: off
# src/beman/${TARGET_NAME}/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

add_library(${TARGET_LIBRARY} STATIC)
add_library(${TARGET_ALIAS} ALIAS ${TARGET_LIBRARY})

target_sources(${TARGET_LIBRARY}
    PRIVATE
    execution.cpp
    PUBLIC
    FILE_SET ${TARGET_LIBRARY}_public_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/execution.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/functional.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/stop_token.hpp
    PUBLIC
    FILE_SET ${TARGET_LIBRARY}_detail_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/almost_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/apply_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/as_except_ptr.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/basic_operation.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/basic_receiver.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/basic_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/basic_state.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/call_result_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/callable.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/check_type_alias_exist.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/child_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/common.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/completion_domain.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/completion_signature.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/completion_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/completion_signatures_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/completion_signatures_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/completion_tag.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/connect.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/connect_all.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/connect_all_result.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/connect_result_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/continues_on.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/decayed_same_as.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/decayed_tuple.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/decayed_typeof.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/decays_to.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/default_domain.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/default_impls.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/empty_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/env_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/env_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/error_types_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/forward_like.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/forwarding_query.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/fwd_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/gather_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_allocator.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_completion_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_completion_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_delegation_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_domain.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_domain_early.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_domain_late.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/get_stop_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/has_completions.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/impls_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/indices_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/indirect_meta_apply.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/inplace_stop_source.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/into_variant.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/join_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/just.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/make_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/make_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/matching_sig.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/meta_combine.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/meta_contains.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/meta_filter.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/meta_prepend.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/meta_transform.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/meta_unique.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/movable_value.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/never_stop_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/nostopstate.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/nothrow_callable.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/operation_state.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/product_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/query_with_default.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/queryable.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/read_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/receiver.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/receiver_of.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/run_loop.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sched_attrs.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sched_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/schedule.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/schedule_result_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sender_adaptor.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sender_adaptor_closure.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sender_decompose.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sender_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sender_in.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sends_stopped.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/set_error.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/set_stopped.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/set_value.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/simple_allocator.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/single_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/single_sender_value_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/start.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/state_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/stop_callback_for_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/stop_source.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/stoppable_source.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/stoppable_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/sync_wait.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/tag_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/then.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/transform_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/type_list.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/unstoppable_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/valid_completion_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/valid_completion_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/valid_specialization.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/value_types_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/variant_or_empty.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/${TARGET_NAME}/detail/write_env.hpp
)
get_property(DETAIL_HEADER_FILES TARGET ${TARGET_LIBRARY} PROPERTY HEADER_SET_${TARGET_LIBRARY}_detail_headers)
source_group("Header Files\\detail" FILES ${DETAIL_HEADER_FILES})

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  TARGETS ${TARGET_LIBRARY}
  EXPORT ${TARGETS_EXPORT_NAME}1
  ARCHIVE DESTINATION lib/$<CONFIG>
  FILE_SET ${TARGET_LIBRARY}_public_headers
  FILE_SET ${TARGET_LIBRARY}_detail_headers
)
target_include_directories(${TARGET_LIBRARY} PUBLIC $<INSTALL_INTERFACE:include>)

install(EXPORT ${TARGETS_EXPORT_NAME}1
  FILE ${TARGET_LIBRARY}-config.cmake
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_LIBRARY}"
  NAMESPACE ${TARGET_LIBRARY}::
)
