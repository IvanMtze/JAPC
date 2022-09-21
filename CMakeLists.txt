cmake_minimum_required(VERSION 3.4.3)
project(japc)

include(GNUInstallDirs)

set(CMAKE_C_STANDARD 11)

add_subdirectory(tools)
add_subdirectory(utils)
add_subdirectory(include)
add_subdirectory(lib)