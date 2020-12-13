cmake_minimum_required(VERSION 3.10)

project(ACTL)
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)
set(BENCHMARK_DIR ${CMAKE_SOURCE_DIR}/benchmark)

add_definitions(-DACTL_DEBUG -ftemplate-backtrace-limit=0)
if(NOT MSVC)
    add_definitions(-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
    -Wall -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wshift-overflow -Wcast-qual -Wcast-align
    -pedantic -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

include_directories(${INCLUDE_DIR})
# make includes show up in IDE
file(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.hpp)
source_group(TREE ${INCLUDE_DIR} FILES ${HEADER_FILES})
add_custom_target(include SOURCES ${HEADER_FILES})

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)
source_group(TREE ${SRC_DIR} FILES ${SRC_FILES})
add_library(src ${SRC_FILES})

file(GLOB_RECURSE TEST_FILES ${TESTS_DIR}/*)
list(FILTER TEST_FILES EXCLUDE REGEX ${TESTS_DIR}/geometry/*.*)
source_group(TREE ${TESTS_DIR} FILES ${TEST_FILES})
add_executable(tests ${TEST_FILES})
target_include_directories(tests PRIVATE ${TESTS_DIR})
target_link_libraries(tests src)

source_group(TREE ${BENCHMARK_DIR} FILES ${BENCHMARK_DIR}/main.cpp)
add_executable(benchmark ${BENCHMARK_DIR}/main.cpp)
find_package(benchmark)
find_path(benchmark_INCLUDE_DIRS "benchmark/benchmark.h")
find_library(benchmark_LIBRARIES NAMES "benchmark")
target_include_directories(benchmark PRIVATE ${benchmark_INCLUDE_DIRS})
target_link_libraries(benchmark src ${benchmark_LIBRARIES})
