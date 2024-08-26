# cmake-format: off
# src/beman/execution26/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

set(TARGET_LIBRARY "beman_execution26")

add_subdirectory(tests)
add_library("${TARGET_LIBRARY}" STATIC "")

target_sources("${TARGET_LIBRARY}" PRIVATE execution.cpp)

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
  "${TARGET_LIBRARY}"
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>
)

install(
  TARGETS "${TARGET_LIBRARY}"
  EXPORT ${TARGETS_EXPORT_NAME}1
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
  FILES_MATCHING
  PATTERN "*.hpp")

target_link_libraries("${TARGET_LIBRARY}")
