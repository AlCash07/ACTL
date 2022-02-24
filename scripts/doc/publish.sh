#!/bin/bash

current_branch=$(eval "git rev-parse --abbrev-ref HEAD")
git checkout doc &&
cp -r build/doc/sphinx/_static . &&
cp -r build/doc/sphinx/actl . &&
cp build/doc/sphinx/* .
git add .
git commit -m "Documentation update"
git push
git checkout $current_branch
