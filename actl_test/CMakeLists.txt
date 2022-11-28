cmake_minimum_required(VERSION 3.10)
project(actl_test)
set(CMAKE_CXX_STANDARD 20)

set(actl_root_dir ${CMAKE_CURRENT_LIST_DIR}/..)
add_library(actl_test INTERFACE)

add_subdirectory(${actl_root_dir} ${CMAKE_BINARY_DIR}/actl)
target_include_directories(actl_test INTERFACE ${actl_root_dir}/actl_test)
target_link_libraries(actl_test INTERFACE actl)
