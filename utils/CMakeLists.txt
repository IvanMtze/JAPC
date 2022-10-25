add_library(string_utils string_utils.cpp string_utils.h)
add_library(parser_utils parser_utils.cpp parser_utils.h)
target_include_directories(string_utils INTERFACE .)
target_include_directories(parser_utils INTERFACE .)
