# cmake-format: off
# src/beman/execution26/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

set(TARGET_LIBRARY beman_execution26)

add_subdirectory(tests)
add_library(${TARGET_LIBRARY} STATIC)

target_sources(${TARGET_LIBRARY}
    PRIVATE
    execution.cpp
    PUBLIC
    FILE_SET ${TARGET_LIBRARY}_public_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/execution.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/functional.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/stop_token.hpp
    PUBLIC
    FILE_SET ${TARGET_LIBRARY}_detail_headers TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/almost_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/as_except_ptr.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/basic_operation.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/basic_receiver.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/basic_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/basic_state.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/call_result_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/callable.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/check_type_alias_exist.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/common.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/completion_domain.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/completion_signature.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/completion_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/completion_signatures_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/completion_signatures_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/completion_tag.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/connect.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/connect_all.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/connect_all_result.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/connect_result_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/continues_on.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/decayed_same_as.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/decayed_tuple.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/decayed_typeof.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/decays_to.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/default_domain.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/default_impls.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/empty_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/env_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/env_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/error_types_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/forward_like.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/forwarding_query.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/fwd_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/gather_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_allocator.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_completion_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_completion_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_domain.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_domain_early.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_domain_late.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/get_stop_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/impls_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/indices_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/indirect_meta_apply.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/inplace_stop_source.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/join_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/just.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/make_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/make_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/matching_sig.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/meta_contains.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/meta_filter.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/meta_prepend.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/meta_unique.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/movable_value.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/never_stop_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/nostopstate.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/nothrow_callable.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/operation_state.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/product_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/query_with_default.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/queryable.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/receiver.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sched_attrs.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sched_env.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/schedule.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/schedule_result_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/scheduler.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sender_adaptor.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sender_adaptor_closure.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sender_decompose.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sender_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sender_in.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/sends_stopped.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/set_error.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/set_stopped.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/set_value.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/simple_allocator.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/single_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/single_sender_value_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/start.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/state_type.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/stop_callback_for_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/stop_source.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/stoppable_source.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/stoppable_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/tag_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/transform_sender.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/type_list.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/unstoppable_token.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/valid_completion_for.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/valid_completion_signatures.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/valid_specialization.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/value_types_of_t.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/variant_or_empty.hpp
    ${PROJECT_SOURCE_DIR}/include/beman/execution26/detail/write_env.hpp
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
