# ACTL
Algorithm Competition Template Library

## How to run tests

Project is fully configured for Visual Studio Code with [cmake-tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools). The only additional setting required is the platform-specific CMake generator. Recommended ones are:
- Windows: `"cmake.generator": "Visual Studio 16 2019"` (older versions are not supported because of numerous MSVC compiler bugs).
- Mac: `"cmake.generator": "Ninja"` (will probably work on Linux as well, but not tested).

Other IDEs are supported via CMake, please see _.vscode/settings.json_ for the configuration.

Tests are written in Catch2 and are run using _run_tests_ target.

Arguments to filter test cases can be specified in:
- _.vscode/tasks.json_ for release.
- _.vscode/launch.json_ for debug.
