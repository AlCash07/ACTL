cmake_minimum_required(VERSION 3.10)
project(actl)
set(CMAKE_CXX_STANDARD 17)

option(ACTL_BUILD_BENCHMARK "Build benchmark target" OFF)
option(ACTL_CREATE_INCLUDE_TARGET "Required to show includes in some IDEs" OFF)

set(actl_include_dir ${CMAKE_CURRENT_LIST_DIR}/include)
add_library(actl INTERFACE)
target_include_directories(actl INTERFACE ${actl_include_dir})

if(ACTL_CREATE_INCLUDE_TARGET)
    file(GLOB_RECURSE actl_header_files ${actl_include_dir}/*.hpp)
    source_group(TREE ${actl_include_dir} FILES ${actl_header_files})
    add_custom_target(include SOURCES ${actl_header_files})
endif()

if(ACTL_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
