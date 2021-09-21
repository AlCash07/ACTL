cmake_minimum_required(VERSION 3.10)
project(actl_test)
set(CMAKE_CXX_STANDARD 17)

set(root_dir ${CMAKE_CURRENT_LIST_DIR}/..)
add_library(actl_test INTERFACE)

add_subdirectory(${root_dir} ${CMAKE_BINARY_DIR}/actl)
target_include_directories(actl_test INTERFACE ${root_dir}/actl_test ${root_dir}/include)
target_link_libraries(actl_test INTERFACE actl)
