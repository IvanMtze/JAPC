cmake_minimum_required(VERSION 3.4.3)
project(japc)

option(BUILD_DOC "Build documentation" ON)

include(GNUInstallDirs)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs support core irreader)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories( ${PROJECT_SOURCE_DIR}/tools)
include_directories( ${PROJECT_SOURCE_DIR}/utils)
include_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(tools)
add_subdirectory(utils)
add_subdirectory(lib)


find_package(Doxygen)
if(DOXYGEN_FOUND)
    message("BUILDING DOCUMENTATION")
    set(DOXYGEN_IN docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
    add_custom_target(internal-docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM )
else(DOXYGEN_FOUND)
    message("DOXYGEN NOT FOUND!")
endif(DOXYGEN_FOUND)