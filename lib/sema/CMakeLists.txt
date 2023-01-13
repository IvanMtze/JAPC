FILE(GLOB SEMA_SOURCES *.cpp)
FILE(GLOB SEMA_HEADERS ${CMAKE_SOURCE_DIR}/include/japc/sema/*.h)

add_library(sema STATIC ${SEMA_HEADERS} ${SEMA_SOURCES})
target_link_libraries(sema PRIVATE utils)
target_link_libraries(sema PRIVATE ast)
target_link_libraries(sema INTERFACE parser)