file(GLOB_RECURSE test_files *.hpp *.cpp)
list(FILTER test_files EXCLUDE REGEX geometry/*.*)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${test_files})
add_executable(run_tests ${test_files})
target_include_directories(run_tests PRIVATE .)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/catch2 ${CMAKE_BINARY_DIR}/catch2)
target_link_libraries(run_tests Catch2::Catch2WithMain)
