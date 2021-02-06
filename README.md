# ACTL
Algorithm Competition Template Library

## How to run tests

Project is fully configured for Visual Studio Code with [cmake-tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).

Other IDEs are supported via CMake, please see _.vscode/settings.json_ for the configuration. Preferred CMake generator can be set there for VSCode as well.

On Windows, `"cmake.generator"` should be changed to `"Visual Studio 16 2019"` in _.vscode/settings.json_. Older versions are not supported because of numerous MSVC compiler bugs.

Tests are written in Catch2 and are run using _run_tests_ target.

Arguments to filter test cases can be specified in:
- _.vscode/tasks.json_ for release.
- _.vscode/launch.json_ for debug.

## For VSCode users

It's highly recommended to run `./scripts/git_stop_tracking_vscode.sh` after the checkout.

This way you can adjust all the VSCode settings without changes showing up in git. In particular, if you're going to contribute to the library, it's recommended to install [blueprint](https://marketplace.visualstudio.com/items?itemName=teamchilla.blueprint) extension and set your name in the `.vscode/blueprints/hpp/__snakeCase_name__.hpp` copyright.
