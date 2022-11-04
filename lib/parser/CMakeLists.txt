FILE(GLOB PARSER_SOURCES *.cpp)
FILE(GLOB PARSER_HEADERS ${CMAKE_SOURCE_DIR}/include/japc/parser/*.h)
add_library(parser STATIC ${PARSER_HEADERS} ${PARSER_SOURCES})
target_link_libraries(parser PUBLIC ast)
target_link_libraries(parser PUBLIC basic)
target_link_libraries(parser PUBLIC ${llvm_libs})

target_include_directories(parser INTERFACE .)
