********************************
Running tests
********************************

Project is fully configured for Visual Studio Code with
`cmake-tools <https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools>`_.

Other IDEs are supported via CMake, please see *.vscode/settings.json* for the configuration.
Preferred CMake generator can be set there for VSCode as well.

On Windows, `"cmake.generator"` can be changed to `"Visual Studio 16 2019"` in *.vscode/settings.json*.
Version 16.10.2 or higher is required because of the numerous bugs in the older versions.

Tests are written in Catch2 and are run using `actl_unit_tests` target.

Arguments to filter test cases can be specified in:

- *.vscode/tasks.json* for release.
- *.vscode/launch.json* for debug.
