cmake_minimum_required(VERSION 3.10)

project(ACTL)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DACTL_DEBUG)
if(NOT MSVC)
    add_definitions(-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
    -Wall -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wshift-overflow -Wcast-qual -Wcast-align
    -pedantic -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

include_directories(include)
# make includes show up in IDE
file(GLOB_RECURSE HEADER_FILES include/*.hpp)
source_group(TREE include FILES ${HEADER_FILES})
add_custom_target(include SOURCES ${HEADER_FILES})

file(GLOB_RECURSE SRC_FILES src/*.cpp)
source_group(TREE src FILES ${SRC_FILES})
add_library(src ${SRC_FILES})

file(GLOB_RECURSE TEST_FILES tests/*)
source_group(TREE tests FILES ${TEST_FILES})
add_executable(tests ${TEST_FILES})
target_link_libraries(tests src)

source_group(TREE benchmark FILES benchmark/main.cpp)
add_executable(benchmark benchmark/main.cpp)
find_package(benchmark)
find_path(benchmark_INCLUDE_DIRS "benchmark/benchmark.h")
find_library(benchmark_LIBRARIES NAMES "benchmark")
target_include_directories(benchmark PRIVATE ${benchmark_INCLUDE_DIRS})
target_link_libraries(benchmark src ${benchmark_LIBRARIES})

# AppCode support
if(APPLE AND NOT IOS)
    set(CMAKE_OSX_SYSROOT macosx)
    set(CMAKE_XCODE_ATTRIBUTE_CXX_LANGUAGE_STANDARD "с++${CMAKE_CXX_STANDARD}")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()
