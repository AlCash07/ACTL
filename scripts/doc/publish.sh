#!/bin/bash

current_branch=$(eval "git rev-parse --abbrev-ref HEAD")
./scripts/git/start_tracking_vscode.sh &&
git restore .vscode/settings.json &&
git switch doc &&
rm -r _static actl contributing &&
cp -r build/doc/sphinx/* . &&
git add .
git commit -m "Documentation update"
git push
git switch $current_branch
./scripts/git/stop_tracking_vscode.sh
