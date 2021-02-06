#!/bin/bash

clang_format_folder () {
    for file in $1/*; do
        if [[ -d $file ]]; then
            clang_format_folder $file
        elif [[ -e $file ]]; then
            case $file in *.hpp|*.cpp)
                clang-format -i --verbose $file
            esac
        fi
    done
}

clang_format_folder "include"
clang_format_folder "tests"
