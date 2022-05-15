#!/bin/bash

current_branch=$(eval "git rev-parse --abbrev-ref HEAD")
./scripts/git/start_tracking_vscode.sh &&
git checkout doc &&
rm -r _static actl contributing &&
cp -r build/doc/sphinx/* . &&
git add .
git commit -m "Documentation update"
git push
git checkout $current_branch
./scripts/git/stop_tracking_vscode.sh
