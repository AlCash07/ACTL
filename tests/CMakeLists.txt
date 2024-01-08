file(GLOB_RECURSE test_files *.hpp *.cpp)
list(FILTER test_files EXCLUDE REGEX geometry/*)
list(FILTER test_files EXCLUDE REGEX graph/*)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${test_files})
# add_executable(run_tests ${test_files})
# add_executable(run_tests operation/scalar/basic_math.cpp)
add_executable(run_tests range/operation/equal.cpp)
# add_executable(run_tests operation/policy/common/allow_promotion.cpp)
target_include_directories(run_tests PRIVATE .)

set(actl_root_dir ${CMAKE_CURRENT_LIST_DIR}/..)
file(GLOB_RECURSE actl_test_files *.hpp *.cpp)
list(FILTER actl_test_files EXCLUDE REGEX geometry/*)
list(FILTER actl_test_files EXCLUDE REGEX graph/*)
source_group(TREE ${actl_root_dir}/tests FILES ${actl_test_files})
add_executable(actl_unit_tests ${actl_test_files})

target_include_directories(actl_unit_tests PRIVATE ${actl_root_dir}/tests)
target_include_directories(actl_unit_tests PUBLIC ${actl_root_dir}/include)
add_subdirectory(${actl_root_dir}/actl_test ${CMAKE_BINARY_DIR}/actl_test)
add_subdirectory(${actl_root_dir}/third_party/catch2 ${CMAKE_BINARY_DIR}/catch2)
target_link_libraries(actl_unit_tests actl_test Catch2::Catch2WithMain)

target_compile_definitions(actl_unit_tests PRIVATE ACTL_DEBUG)
if(MSVC)
    target_compile_options(actl_unit_tests PRIVATE
        /EHsc   # warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
        /wd4455 # warning C4455: literal suffix identifiers that do not start with an underscore are reserved
    )
else()
    target_compile_definitions(actl_unit_tests PRIVATE _GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC)
    target_compile_options(actl_unit_tests PRIVATE
        -Wall -Wformat=2 -Wfloat-equal -Wconversion -Wshift-overflow -Wcast-qual -Wcast-align
        -pedantic -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all
        -ftemplate-backtrace-limit=0)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(actl_unit_tests PRIVATE
            -Wshadow -fconstexpr-backtrace-limit=0)
    endif()
endif()
