cmake_minimum_required(VERSION 3.10)

project(ACTL)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_BENCHMARK "Build benchmark target" OFF)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_library(ACTL INTERFACE)
target_include_directories(ACTL INTERFACE ${INCLUDE_DIR})
# make includes show up in IDE
file(GLOB_RECURSE header_files ${INCLUDE_DIR}/*.hpp)
source_group(TREE ${INCLUDE_DIR} FILES ${header_files})
add_custom_target(include SOURCES ${header_files})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
