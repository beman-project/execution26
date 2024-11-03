# cmake-format: off
# /CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

cmake_minimum_required(VERSION 3.25...3.31)

project(beman_execution26 VERSION 0.0.1 LANGUAGES CXX)

set(TARGET_NAME execution26)
set(TARGET_NAMESPACE beman) # FIXME: not used in install(EXPORT ...) CK?
set(TARGET_PREFIX ${TARGET_NAMESPACE}.${TARGET_NAME})
set(TARGET_LIBRARY ${PROJECT_NAME})
set(TARGET_ALIAS ${TARGET_LIBRARY}::${TARGET_LIBRARY})
set(TARGET_PACKAGE_NAME ${PROJECT_NAME}-config)
set(TARGETS_EXPORT_NAME ${PROJECT_NAME}-targets)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds are not allowed!")
endif()

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

add_subdirectory(src/beman/execution26)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()

  add_subdirectory(tests/beman/execution26)
  add_subdirectory(examples)
endif()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}-version.cmake
  VERSION ${CMAKE_PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
  "cmake/Config.cmake.in" ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}-version.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

include(CPack)
