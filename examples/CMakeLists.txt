# cmake-format: off
# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

set(BEMAN_OPTIONAL26_LIBRARY "beman_execution26")

include(GNUInstallDirs)

set(EXAMPLES
  stop_token
)

foreach(EXAMPLE ${EXAMPLES})
  add_executable(${EXAMPLE} "")
  target_sources(${EXAMPLE} PRIVATE ${EXAMPLE}.cpp)
  target_link_libraries(${EXAMPLE} "${BEMAN_EXECUTION26_LIBRARY}")
  install(
    TARGETS ${EXAMPLE}
    EXPORT ${TARGETS_EXPORT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach()
