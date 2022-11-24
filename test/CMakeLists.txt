set(CTEST_PROJECT_NAME "JAPC")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")


function(do_test target arg result)
    add_test(NAME Comp${arg} COMMAND ${target} ${arg})
    set_tests_properties(Comp${arg}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
            )
endfunction()

do_test(japc ${PROJECT_SOURCE_DIR}/test/sieve.pas "^")