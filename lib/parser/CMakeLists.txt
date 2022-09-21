add_library(parser parser.cpp)

target_link_libraries(parser PRIVATE string_utils)
target_link_libraries(parser PRIVATE scanner)
target_link_libraries(parser PRIVATE ast)

target_include_directories(parser INTERFACE .)