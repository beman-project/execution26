# cmake-format: off
# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

set(BEMAN_EXECUTION26_LIBRARY beman_${TARGET_NAME})

set(EXAMPLES
  stop_token
  stopping
)

foreach(EXAMPLE ${EXAMPLES})
  set(EXAMPLE_TARGET ${TARGET_PREFIX}.examples.${EXAMPLE})
  add_executable(${EXAMPLE_TARGET})
  target_sources(${EXAMPLE_TARGET} PRIVATE ${EXAMPLE}.cpp)
  target_link_libraries(${EXAMPLE_TARGET} PRIVATE ${BEMAN_EXECUTION26_LIBRARY})
endforeach()
