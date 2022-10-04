add_library(diagnostics diagnostics.cpp)

target_link_libraries(diagnostics PUBLIC scanner)

target_include_directories(diagnostics INTERFACE .)