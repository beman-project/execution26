# src/Beman/Execution26/tests/CMakeLists.txt
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND execution_tests
    exec-general.pass
    exec-utils-cmplsigs.pass
    functional-syn.pass
    exec-recv.pass
    queryable.pass
    exec-get-env.pass
    exec-set-stopped.pass
    exec-set-error.pass
    exec-set-value.pass
    utilities.pass
    function-objects.pass
    thread.pass
    thread-stoptoken.pass
    thread-stoptoken-intro.pass
    thread-stoptoken-syn.compile.pass
    stoptoken-concepts.pass
    stoptoken.pass
    stoptoken-general.pass
    stoptoken-mem.pass
    stopsource.pass
    stopsource-general.pass
    stopsource-cons.pass
    stopsource-mem.pass
    stopcallback.pass
    stopcallback-general.pass
    stopcallback-cons.pass
    stoptoken-never.pass
    stoptoken-never-general.pass
    stoptoken-inplace.pass
    stoptoken-inplace-general.pass
    stoptoken-inplace-members.pass
    stopsource-inplace.pass
    stopsource-inplace-general.pass
    stopsource-inplace-cons.pass
    stopsource-inplace-mem.pass
    stopcallback-inplace.pass
    stopcallback-inplace-general.pass
    stopcallback-inplace-cons.pass
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../include)
foreach(test ${execution_tests})
    add_executable(${test} ${test}.cpp)
    target_include_directories(
      "${test}"
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>
    )
    add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
endforeach()
