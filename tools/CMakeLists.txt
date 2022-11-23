set(SOURCE japc.cpp)

add_executable(${PROJECT_NAME} ./${SOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC basic)
target_link_libraries(${PROJECT_NAME} PUBLIC stdc++)