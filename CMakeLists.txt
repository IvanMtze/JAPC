cmake_minimum_required(VERSION 3.4.3)
project(japc)
include(CTest) # For testing
option(BUILD_DOC "Build documentation" OFF)
option(BUILD_USER_DOC "Build user documentation" PFF)

include(GNUInstallDirs)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
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
include_directories(${PROJECT_SOURCE_DIR}/test)

add_subdirectory(tools)
add_subdirectory(utils)
add_subdirectory(lib)
add_subdirectory(test)

find_package(Doxygen)

if(BUILD_DOC)
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
        message("Doxygen is not found!")
    endif(DOXYGEN_FOUND)
else(BUILD_DOC)
    message("Omitting doxygen doc!")
endif(BUILD_DOC)

if(BUILD_USER_DOC)
    find_program(MAKEINFO_EXECUTABLE makeinfo)
    set(info_in_es ${CMAKE_CURRENT_SOURCE_DIR}/man/japc_es.texi)
    set(info_out_es ${CMAKE_CURRENT_BINARY_DIR}/japc_es.info)
    set(info_in_en ${CMAKE_CURRENT_SOURCE_DIR}/man/japc_en.texi)
    set(info_out_en ${CMAKE_CURRENT_BINARY_DIR}/japc_en.info)
    add_custom_command(OUTPUT ${info_out_es}
            COMMAND ${MAKEINFO_EXECUTABLE} -o ${info_out_es} ${info_in_es}
            DEPENDS ${info_in_es}
            COMMENT "Creating Info file ${info_out_es}"
            VERBATIM)
    add_custom_command(OUTPUT ${info_out_en}
            COMMAND ${MAKEINFO_EXECUTABLE} -o ${info_out_en} ${info_in_en}
            DEPENDS ${info_in_en}
            COMMENT "Creating Info file ${info_out_en}"
            VERBATIM)

    add_custom_target(info ALL DEPENDS ${info_out_es} ${info_out_en})
else(BUILD_USER_DOC)
    message("Omitting user doc!")
endif(BUILD_USER_DOC)