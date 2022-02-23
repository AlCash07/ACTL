#!/bin/bash

find .vscode -type f -name "*" -exec \
    git update-index --assume-unchanged {} \;
