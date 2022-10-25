add_library(diagnostics diagnostics.cpp)
add_library(compiler compiler.cpp)
add_library(file    file.cpp)

target_link_libraries(diagnostics PUBLIC scanner)

target_include_directories(diagnostics INTERFACE .)

target_link_libraries(compiler PUBLIC scanner)

target_include_directories(compiler INTERFACE .)

target_link_libraries(file PUBLIC scanner)

target_include_directories(file INTERFACE .)