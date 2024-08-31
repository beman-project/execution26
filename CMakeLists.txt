# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: off
# /CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

cmake_minimum_required(VERSION 3.23)

project(beman_execution26 VERSION 0.0.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wno-missing-braces)
set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

add_subdirectory(src)
add_subdirectory(examples)

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

#-dk:TODO detect if deleted functions can have a message: BEMAN_EXECUTION26_HAS_DELETED_MESSAGE

#-dk:TODO install(
#-dk:TODO   EXPORT ${TARGETS_EXPORT_NAME}
#-dk:TODO   NAMESPACE ${CMAKE_PROJECT_NAME}
#-dk:TODO   DESTINATION ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  "cmake/Config.cmake.in"
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR})
