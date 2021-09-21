cmake_minimum_required(VERSION 3.10)
project(actl_unit_tests)
set(CMAKE_CXX_STANDARD 17)

set(root_dir ${CMAKE_CURRENT_LIST_DIR}/..)
file(GLOB_RECURSE test_files *.hpp *.cpp)
list(FILTER test_files EXCLUDE REGEX geometry/*)
list(FILTER test_files EXCLUDE REGEX graph/*)
source_group(TREE ${root_dir}/tests FILES ${test_files})
add_executable(actl_unit_tests ${test_files})

target_include_directories(actl_unit_tests PRIVATE ${root_dir}/tests)
target_include_directories(actl_unit_tests PUBLIC ${root_dir}/include)
add_subdirectory(${root_dir}/actl_test ${CMAKE_BINARY_DIR}/actl_test)
add_subdirectory(${root_dir}/third_party/catch2 ${CMAKE_BINARY_DIR}/catch2)
target_link_libraries(actl_unit_tests actl_test Catch2::Catch2WithMain)

target_compile_definitions(actl_unit_tests PRIVATE ACTL_DEBUG)
if(MSVC)
    target_compile_options(actl_unit_tests PRIVATE
        /W4
        /EHsc   # warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
        /wd4455 # warning C4455: literal suffix identifiers that do not start with an underscore are reserved
    )
else()
    target_compile_definitions(actl_unit_tests PRIVATE _GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC)
    target_compile_options(actl_unit_tests PRIVATE
    -Wall -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wshift-overflow -Wcast-qual -Wcast-align
    -pedantic -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector
    -ftemplate-backtrace-limit=0 -fconstexpr-backtrace-limit=0)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()
