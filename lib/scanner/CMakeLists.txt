FILE(GLOB SCANNER_SOURCES *.cpp)
add_library(scanner STATIC ${SCANNER_SOURCES})
target_link_libraries(scanner PRIVATE utils)
target_include_directories(scanner INTERFACE .)