set(SOURCE japc.cpp)

add_executable(${PROJECT_NAME} ./${SOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC scanner)
target_link_libraries(${PROJECT_NAME} PUBLIC parser)
target_link_libraries(${PROJECT_NAME} PUBLIC ast)
target_link_libraries(${PROJECT_NAME} PUBLIC stdc++)