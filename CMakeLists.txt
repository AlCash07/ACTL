cmake_minimum_required(VERSION 3.10)
project(actl)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_BENCHMARK "Build benchmark target" OFF)

set(include_dir ${CMAKE_SOURCE_DIR}/include)
add_library(actl INTERFACE)
target_include_directories(actl INTERFACE ${include_dir})
# make includes show up in IDE
file(GLOB_RECURSE header_files ${include_dir}/*.hpp)
source_group(TREE ${include_dir} FILES ${header_files})
add_custom_target(include SOURCES ${header_files})

if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
