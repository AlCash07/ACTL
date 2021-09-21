#!/bin/bash

find .vscode -type f -name "*" -exec \
    git update-index --no-assume-unchanged {} \;
