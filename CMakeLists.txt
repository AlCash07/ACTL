cmake_minimum_required(VERSION 3.0)

set(BINARY_DIR .)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})

project("ACTL")
set(CMAKE_CXX_STANDARD 17)

function(get_files OUTPUT PATH EXTENSION)
get_filename_component(FULL_PATH ${PATH} ABSOLUTE)
file(GLOB_RECURSE FILES ${PATH}/*.${EXTENSION})
foreach(FILE_NAME ${FILES})
    get_filename_component(ABSOLUTE_FILE_NAME ${FILE_NAME} ABSOLUTE)
    file(RELATIVE_PATH FILE_RELATIVE_PATH ${FULL_PATH} ${ABSOLUTE_FILE_NAME})
    get_filename_component(FILE_DIRECTORY ${FILE_RELATIVE_PATH} DIRECTORY)
    set(FILE_DIRECTORY "\\${FILE_DIRECTORY}")
    string(REPLACE / \\ FILE_DIRECTORY ${FILE_DIRECTORY})
    source_group("${FILE_DIRECTORY}" FILES "${FILE_NAME}")
endforeach()
set(${OUTPUT} ${FILES} PARENT_SCOPE)
endfunction(get_files)

include_directories(include)
# make includes show up in IDE
get_files(HEADER_FILES include hpp)
add_custom_target(include SOURCES ${HEADER_FILES})

get_files(SRC_FILES src cpp)
add_library(src ${SRC_FILES})

get_files(TEST_FILES tests cpp)
add_executable(tests ${TEST_FILES})
target_link_libraries(tests src)

# AppCode support
if(APPLE AND NOT IOS)
    set(CMAKE_OSX_SYSROOT macosx)
    set(CMAKE_XCODE_ATTRIBUTE_CXX_LANGUAGE_STANDARD "с++${CMAKE_CXX_STANDARD}")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()