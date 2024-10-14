# src/beman/execution26/tests/CMakeLists.txt
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND execution_tests
    exec-when-all.pass
    exec-continues-on.pass
    exec-schedule-from.pass
    exec-starts-on.pass
    exec-let.pass
    exec-then.pass
    exec-just.pass
    meta-transform.pass
    exec-read-env.pass
    exec-get-delegation-scheduler.pass
    exec-snd-apply.pass
    meta-combine.pass
    exec-into-variant.pass
    exec-run-loop-types.pass
    exec-run-loop-general.pass
    exec-snd-expos.pass
    exec-recv-concepts.pass
    forward-like.pass
    meta-filter.pass
    meta-prepend.pass
    meta-contains.pass
    meta-unique.pass
    exec-getcomplsigs.pass
    exec-connect.pass
    execution-syn.pass
    exec-snd-transform.pass
    exec-sched.pass
    exec-schedule.pass
    exec-domain-default.pass
    exec-get-scheduler.pass
    exec-get-compl-sched.pass
    exec-snd-concepts.pass
    exec-get-domain.pass
    exec-get-stop-token.pass
    exec-get-allocator.pass
    exec-fwd-env.pass
    exec-opstate.pass
    exec-opstate-start.pass
    allocator-requirements-general.pass
    exec-utils-cmplsigs.pass
    functional-syn.pass
    exec-recv.pass
    execution-queryable-concept.pass
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
    exec-sync-wait.pass
    exec-general.pass
)

if(PROJECT_IS_TOP_LEVEL)
    foreach(test ${execution_tests})
        set(TEST_EXE ${TARGET_PREFIX}.${test})
        add_executable(${TEST_EXE} ${test}.cpp)
        target_link_libraries(${TEST_EXE} PRIVATE ${TARGET_LIBRARY})
        add_test(NAME ${TEST_EXE} COMMAND $<TARGET_FILE:${TEST_EXE}>)
    endforeach()
endif()
