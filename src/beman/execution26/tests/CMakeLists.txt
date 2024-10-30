# src/beman/execution26/tests/CMakeLists.txt
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
cmake_minimum_required(VERSION 3.25...3.31)

project(beman_execution26_tests LANGUAGES CXX)

list(APPEND execution_tests
    exec-awaitable.pass
    allocator-requirements-general.pass
    exec-connect.pass
    exec-continues-on.pass
    exec-domain-default.pass
    exec-fwd-env.pass
    exec-general.pass
    exec-get-allocator.pass
    exec-get-compl-sched.pass
    exec-get-delegation-scheduler.pass
    exec-get-domain.pass
    exec-get-env.pass
    exec-get-scheduler.pass
    exec-get-stop-token.pass
    exec-getcomplsigs.pass
    exec-into-variant.pass
    exec-just.pass
    exec-let.pass
    exec-opstate-start.pass
    exec-opstate.pass
    exec-read-env.pass
    exec-recv-concepts.pass
    exec-recv.pass
    exec-run-loop-general.pass
    exec-run-loop-types.pass
    exec-sched.pass
    exec-schedule-from.pass
    exec-schedule.pass
    exec-set-error.pass
    exec-set-stopped.pass
    exec-set-value.pass
    exec-snd-apply.pass
    exec-snd-concepts.pass
    exec-snd-expos.pass
    exec-snd-transform.pass
    exec-starts-on.pass
    exec-sync-wait.pass
    exec-then.pass
    exec-utils-cmplsigs.pass
    exec-when-all.pass
    execution-queryable-concept.pass
    execution-syn.pass
    forward-like.pass
    function-objects.pass
    functional-syn.pass
    meta-combine.pass
    meta-contains.pass
    meta-filter.pass
    meta-prepend.pass
    meta-transform.pass
    meta-unique.pass
    stopcallback-cons.pass
    stopcallback-general.pass
    stopcallback-inplace-cons.pass
    stopcallback-inplace-general.pass
    stopcallback-inplace.pass
    stopcallback.pass
    stopsource-cons.pass
    stopsource-general.pass
    stopsource-inplace-cons.pass
    stopsource-inplace-general.pass
    stopsource-inplace-mem.pass
    stopsource-inplace.pass
    stopsource-mem.pass
    stopsource.pass
    stoptoken-concepts.pass
    stoptoken-general.pass
    stoptoken-inplace-general.pass
    stoptoken-inplace-members.pass
    stoptoken-inplace.pass
    stoptoken-mem.pass
    stoptoken-never-general.pass
    stoptoken-never.pass
    stoptoken.pass
    thread-stoptoken-intro.pass
    thread-stoptoken-syn.compile.pass
    thread-stoptoken.pass
    thread.pass
    utilities.pass
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
  find_package(beman_execution26 REQUIRED)
  if(beman_execution26_FOUND)
    set(CMAKE_CXX_STANDARD 26)
    set(TARGET_LIBRARY beman_execution26::beman_execution26)
  endif()
endif()

foreach(test ${execution_tests})
  set(TEST_EXE ${TARGET_PREFIX}.${test})
  add_executable(${TEST_EXE} ${test}.cpp)
  target_link_libraries(${TEST_EXE} PRIVATE ${TARGET_LIBRARY})
  add_test(NAME ${TEST_EXE} COMMAND $<TARGET_FILE:${TEST_EXE}>)
endforeach()

if(NOT PROJECT_IS_TOP_LEVEL)
    # test if the targets are findable from the build directory
    # cmake-format: off
    add_test(find-package-test
        ${CMAKE_CTEST_COMMAND}
        -C ${CMAKE_BUILD_TYPE}
        --build-and-test
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/find-package-test"
        --build-generator ${CMAKE_GENERATOR}
        --build-makeprogram ${CMAKE_MAKE_PROGRAM}
        --build-options
            "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
    )
    # cmake-format: on
endif()
