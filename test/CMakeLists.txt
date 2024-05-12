# Copyright © 2024 Beman Project
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND execution_tests
    thread.pass
    thread-stoptoken.pass
    thread-stoptoken-intro.pass
    thread-stoptoken-syn.compile.pass
    stoptoken-concepts.pass
)
foreach(test ${execution_tests})
    add_executable(${test} ${test}.cpp)
    add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
endforeach()
