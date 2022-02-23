#!/bin/bash

modified_files=$(git diff-index --cached --name-only HEAD)
for file in ${modified_files}; do
    # Check that the file exists. If it doesn't, it was deleted by the commit.
    if [[ -f ${file} ]]; then
        case $file in *.hpp|*.cpp)
            clang-format -i --verbose ${file} || {
            # Cancel the commit if clang-format fails.
                exit 1
            }
            git add ${file}
        esac
    fi
done
