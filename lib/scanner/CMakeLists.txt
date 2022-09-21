add_library(scanner scanner.cpp)

target_link_libraries(scanner PRIVATE string_utils)
target_link_libraries(scanner PRIVATE characters)

target_include_directories(scanner INTERFACE .)