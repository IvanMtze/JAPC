add_library(parser parser.cpp)

target_link_libraries(parser PUBLIC string_utils)
target_link_libraries(parser PUBLIC scanner)
target_link_libraries(parser PUBLIC ast)

target_include_directories(parser INTERFACE .)