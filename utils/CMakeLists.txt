file(GLOB UTILS_SOURCES *.h *.cpp)
add_library(utils ${UTILS_SOURCES})
target_include_directories(utils PUBLIC .)