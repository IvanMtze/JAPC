function(do_test name target arg result)
    add_test(NAME ${name} COMMAND ${target} ${ARGN} ${arg})
    set_tests_properties(${name}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
            )
endfunction()

# File does not exists
do_test(FileNotExists japc ${PROJECT_SOURCE_DIR}/test/sievE.pas "Unable to find *")
# Help
do_test(PrintHelp japc -h "This help is to be done")
# Empty result (Input is correct)
do_test(SieveCorrect japc ${PROJECT_SOURCE_DIR}/test/sieve.pas "^")
# Empty result as no file given to compiler
do_test(NonArgs japc '' "^")
# Variable handling
do_test(DuplicatedVariables japc ${PROJECT_SOURCE_DIR}/test/duplicated_vars.pas "error: J is already defined")
# Variable handling
do_test(WarningAsError japc ${PROJECT_SOURCE_DIR}/test/duplicated_vars.pas "error: Is this an empty statement?" -werror)
# Variable handling
do_test(Warning japc ${PROJECT_SOURCE_DIR}/test/duplicated_vars.pas "warning: Is this an empty statement?" -wall)

