# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND execution_tests
    thread-stoptoken-syn.compile.pass
)
foreach(test ${execution_tests})
    add_executable(${test} ${test}.cpp)
    add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
endforeach()
