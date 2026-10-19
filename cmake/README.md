# CMake C++ Project Template
*cmake_cpp_project_template* is a simple template for C and C++ projects.
The template includes targets for building libraries, tests, and executables.

Currently supported build on GNU/Linux.

The *cmake_cpp_project_template* assumes the project is configured using:
* CMake (3.10 or above)
* C/C++ compiler

## Contents

* [Description](#description)
  * [Options](#options)
* [Build](#build)
  * [Custom build targets](#custom-build-targets)
  * [Coverage build target](#coverage-build-target)
* [Usage](#usage)
* [Targets](#targets)
  * [Libraries](#libraries)
  * [Executables](#executables)
  * [Tests](#tests)
  * [Drivers](#drivers)
  * [Examples](#examples)
  * [Externals](#externals)
  * [Custom targets](#custom-targets)
* [Examples of using targets](#examples-of-using-targets)
  * [Libraries example](#libraries-example)
  * [Executables example](#executables-example)
  * [Tests example](#tests-example)
  * [Drivers example](#drivers-example)
  * [Externals example](#externals-example)
  * [Custom tests example](#custom-tests-example)
* [License](#license)

## Description

The template sets up hooks for `git`. In particular, a check for the presence of
last whitespace characters in lines is added. This can lead to errors due to
empty lines at the end of the file. To fix this error, you need to run the
command:
```
$ git config --global core.whitespace -blank-at-eof
```

The project is built with the `make` command. Assembly takes place in three
stages:
1. the `make` command starts configuring the project with `cmake`;
2. the project is configured by the `cmake` system;
3. start building the project/target with `make`.

The build looks like this:
```
user -> make -> cmake -> make
```

Initializing a new project based on the this template:
1. Initial repository setup
    1. Initialize a local git repository for the new project
    ```
    $ git init
    ```
    2. Add a remote reference to the main repository of your new project (origin)
    ```
    $ git remote add origin git@github.com:org/new_project.git
    ```
    3. Add a remote reference to the parent template repository (template)
    ```
    $ git remote add template git@github.com:org/cmake_cpp_project_template.git
    ```
    4. Fetch the latest state of the template repository
    ```
    $ git checkout -b master template/master
    ```
    5. Create a local master branch based on the stable template branch
    ```
    $ git checkout -b master template/master
    ```
2. Updating the template in a child project
    1. Fetch the latest changes from the template repository
    ```
    $ git fetch template
    ```
    2. Merge changes from the template branch into current working branch
    ```
    $ git merge template/master --allow-unrelated-histories
    ```
    **Note**: The `--allow-unrelated-histories` flag is only required during the
    very first merge to link the commit histories. For subsequent updates, a
    standard git merge template/master is sufficient. If conflicts occur in
    configuration files (such as the root CMakeLists.txt), resolve them manually.

### Options

Allowed extra command line sanitizers options:
* `USE_ADDR_SANITIZER` - build project with address sanitizer;
* `USE_LEAK_SANITIZER` - build project with leak sanitizer;
* `USE_BEHAVIOR_SANITIZER` - build project with undefined behavior sanitizer;
* `USE_THREAD_SANITIZER` - build project with thread sanitizer.

Allowed extra command line C/C++ standard options:
* `USE_DEFAULT_STANDARD` - using the C/C++ standard by default;
* `PROJECT_C_STANDARD` - C standard;
* `PROJECT_CXX_STANDARD` - C++ standard.

Allowed extra command line building options:
* `USE_LTO` - use link-time optimization for release builds;
* `USE_PEDANTIC` - tell the compiler to be pedantic;
* `USE_WERROR` - tell the compiler to make the build fail when warnings are present.

Allowed extra command line components building options:
* `BUILD_EXAMPLES` - build examples;
* `BUILD_TESTS` - build perftests and unittests.

## Build

The project is built with the `make` command:
```
$ make <build_type>/<target_name>
```
Supported build types:
* `release` - release build of the project with optimization enabled in the
  directory `<repo_dir>/build_release`;
* `debug` - debug build of the project without optimization in the directory
  `<repo_dir>/build_debug`.

Making a build without specifying a type results in a build with the release
build type.
```
$ make <target_name>
```

### Custom build targets

Supported custom build targets:
* `all` - building of all targets;
* `clean` - cleaning the build directory, removing all build artifacts and the
  build directory;
* `test` - build all unit tests (targets marked as tests) and run the built
  tests;
* `<unit_test_name>_run` - building a unit test `<unit_test_name>` and running
  the built test;
* `doxygen_doc` - generation of doxygen documentation. To build the target,
  needs to have the `doxygen` and `dot` utilities installed. The analysis result
  will be saved in the directory `<build_directory>/doc/html`;
* `stat_cppcheck` - static analysis of the project's source code using the
  `cppcheck` utility. The analysis result will be saved in the directory
  `<build_directory>/lint/cppcheck/report`.

### Coverage build target

Testing code coverage is performed using the command:
```
$ make coverage
```

Executing this command will build the repository, run unit tests, and generate
code coverage statistics. The build will be performed in `debug` mode in the
`<repo_dir>/build_coverage` directory. Code test coverage statistics will be
saved in the `<repo_dir>/build_coverage/__coverage/report` directory.

## Usage

## Targets

The library supports targets:
* [LibTarget](#libraries) - building libraries;
* [ExecTarget](#executables) - building executable files;
* [TestTarget](#tests) - building tests;
* [DriverTarget](#drivers) - building kernel modules;
* [ExampleTarget](#examples) - building example executable files;
* [ExternalTarget, FetchTarget and WrapperTarget](#externals) - building external modules;
* [CustomTarget](#custom-targets) - custom targets.

### Libraries

`LibTarget` declares the library as the target of the build. `LibTarget`
supports keywords:
* library type `SHARED`, `STATIC` or `INTERFACE`;
* `HEADERS` - list of public header files. If this parameter is not specified,
  all header files in the directory containing the target's source code will be used;
* `SOURCES` - list of files with target source code;
* `INCLUDE_DIR` - directory relative to which the target's header files will
  be searched;
* `LINKER_LANGUAGE` - specifies language whose compiler will invoke the linker
  (supports only 'C' or 'CXX');
* `COMPILE_DEFINITIONS` - preprocessor definitions for compiling a target's sources;
* `LIBRARIES` - list of libraries the target depends on;
* `DEPENDS` - list of the target's dependencies.

The `LIBRARIES` field specifies the list of libraries (targets) included in the
project, the `DEPENDS` field specifies the system/third-party libraries (targets)
required to build the project/target.

Library build target template:
```
LibTarget(<lib_name> <lib_type>
    HEADERS     <list_of_headers>
    SOURCES     <list_of_source_files>
    INCLUDE_DIR     <directory>
    LINKER_LANGUAGE <lang>
    COMPILE_DEFINITIONS
        <preprocessor_definitions>
    LIBRARIES   <list_of_libraries_target_depends_on>
    DEPENDS     <list_of_target_dependencies>
)
```

### Executables

`ExecTarget` declares the build target to be an executable file. `ExecTarget`
supports keywords:
* `HEADERS` - list of header files included in the target (this parameter
  is optional);
* `SOURCES` - list of files with target source code;
* `INCLUDE_DIR` - directory relative to which the target's header files will
  be searched;
* `LINKER_LANGUAGE` - specifies language whose compiler will invoke the linker
  (supports only 'C' or 'CXX');
* `COMPILE_DEFINITIONS` - preprocessor definitions for compiling a target's sources;
* `LIBRARIES` - list of libraries the target depends on;
* `DEPENDS` - list of the target's dependencies.

The `LIBRARIES` field specifies the list of libraries (targets) included in the
project, the `DEPENDS` field specifies the system/third-party libraries (targets)
required to build the project/target.

Executable file build target template:
```
ExecTarget(<exec_name>
    HEADERS     <list_of_headers>
    SOURCES     <list_of_source_files>
    INCLUDE_DIR     <directory>
    LINKER_LANGUAGE <lang>
    COMPILE_DEFINITIONS
        <preprocessor_definitions>
    LIBRARIES   <list_of_libraries_target_depends_on>
    DEPENDS     <list_of_target_dependencies>
)
```

### Tests

`TestTarget` declares the build target to be an executable file. `cmake` supports
a `test` metatarget that runs all declared tests, `TestTarget` is essentially an
executable and is needed to use the `cmake` build system test framework. `TestTarget`
supports keywords:
* `HEADERS` - list of header files included in the target (this parameter
  is optional);
* `SOURCES` - list of files with target source code;
* `INCLUDE_DIR` - directory relative to which the target's header files will
  be searched;
* `LINKER_LANGUAGE` - specifies language whose compiler will invoke the linker
  (supports only 'C' or 'CXX');
* `COMPILE_DEFINITIONS` - preprocessor definitions for compiling a target's sources;
* `LIBRARIES` - list of libraries the target depends on;
* `DEPENDS` - list of the target's dependencies;
* `DISABLE` - disable autorun test with `make test` command.

The `LIBRARIES` field specifies the list of libraries (targets) included in the
project, the `DEPENDS` field specifies the system/third-party libraries (targets)
required to build the project/target.
The build of this target can be enabled/disabled using the BUILD_TESTS
configuration option.

Test build target template:
```
TestTarget(<test_name>
    HEADERS     <list_of_headers>
    SOURCES     <list_of_source_files>
    INCLUDE_DIR     <directory>
    LINKER_LANGUAGE <lang>
    COMPILE_DEFINITIONS
        <preprocessor_definitions>
    LIBRARIES   <list_of_libraries_target_depends_on>
    DEPENDS     <list_of_target_dependencies>
    DISABLE
)
```

### Drivers

To build a Linux kernel modules, needs to install kernel headers.
Command for installing kernel headers:
```
$ sudo apt-get install linux-headers-$(uname -r)
```

`DriverTarget` declares the kernel driver as the build target. `DriverTarget`
supports keywords:
* `INCLUDE_DIR` - directory relative to which the target's header files will
  be searched;
* `SOURCES` - list of files with target source code;
* `EXTRA_CFLAGS` - list of additional target compilation flags;
* `EXTRA_LDFLAGS` - list of additional target linking flags;
* `COMPILE_DEFINITIONS` - preprocessor definitions for compiling a target's sources.

The assembly of driders is somewhat unique, so it takes place in several stages:
1. in the build directory, a subdirectory of driver build source codes
   `__kbuild_src` is created with a directory tree up to the driver, which
   (directory tree) corresponds to the directory tree in the source code directory;
2. all source codes of the driver are copied to the subdirectory of driver
   assembly source codes (`__kbuild_src`) and `Makefile` is configured to build
   the driver based on the template `Makefile_driver.in`;
3. the driver is built based on the rules specified in the configured `Makefile`.

Driver build target template:
```
DriverTarget(<driver_name>
    INCLUDE_DIRS    <list_of_include_directories>
    SOURCES         <list_of_source_files>
    EXTRA_CFLAGS    <list_of_c_compile_flags>
    EXTRA_LDFLAGS   <list_of_ld_flags>
    COMPILE_DEFINITIONS
        <list_of_compile_definitions>
)
```

### Examples

This target type is synonymous with `ExecTarget`. A unique feature of this target
type is that their build can be enabled/disabled using the BUILD_EXAMPLES
configuration option.

### Externals

#### Externals

`ExternalTarget` creates a custom target to drive download, update/patch,
configure, build, install and test steps of an external project. `ExternalTarget`
supports keywords:
* `CONFIGURE_COMMAND` - configure command for target;
* `BUILD_COMMAND` - build command for target;
* `INSTALL_COMMAND` - build command for target;
* `INCLUDE_DIR` - directory relative to which the target's header files will
  be searched;
* `INSTALL_DIR` - installation directory. This directory will be searched for
  header files and libraries of the compiled target;
* `URL` - internet address or path to the archive with the target's source files;
* `LIBRARIES` - list of libraries provided by the target;
* `DEPENDS` - list of the target's dependencies;
* `PATCHES` - list of patches.

Example of creating patches:
```
$ diff -uNr ./orig_repo/ ./patched_repo/ > patch_name.patch
$ git diff > patch_name.patch #For git repositories
```

Example of applying patches:
```
$ patch -p1 --dry-run < patch_name.patch
```

External project build target template:
```
ExternalTarget(<ext_name>
    URL
    CONFIGURE_COMMAND   <command>
    BUILD_COMMAND       <command>
    INSTALL_COMMAND     <command>
    INSTALL_DIR     <directory>
    INCLUDE_DIR     <directory>
    PATCHES     <list_of_pathes>
    LIBRARIES   <list_of_libraries>
    DEPENDS     <list_of_target_dependencies>
)
```

#### Fetchs

`FetchTarget` is a wrapper around the cmake `FetchContent` functionality for
embedding third-party projects into project. `FetchTarget` supports keywords:
* `GIT_REPOSITORY` - link to a third-party project's git repository;
* `GIT_TAG` - git tag of the required project version;
* `CMAKE_OPTIONS` - list of variables declared in cmake as `options` with their
  values;
* `CMAKE_VARIABLES` - list of cmake variables with their values;
* `LIBRARIES` - list of libraries provided by the target.

External project build target template:
```
FetchTarget(<ext_name>
    GIT_REPOSITORY  <url_to_git_repo>
    GIT_TAG         <git_tag>
    CMAKE_OPTIONS   <list_of_options>
    CMAKE_VARIABLES <list_of_variables>
    LIBRARIES       <list_of_libraries>
)
```

#### Wrappers

### Custom targets

#### Custom commands

`CustomCommand` is a delineation of the `cmake` `add_custom_command` function.
Documentation on `add_custom_command` can be requested from the official
[website](https://cmake.org/cmake/help/latest/command/add_custom_command.html).

#### Custom targets

`CustomTarget` is a delineation of the `cmake` `add_custom_target` function.
Documentation on `add_custom_target` can be requested from the official
[website](https://cmake.org/cmake/help/latest/command/add_custom_target.html).

#### Custom test targets

`CustomTestTarget` declares a custom test build target that is not an executable
file. `cmake` supports a `test` metatarget that runs all declared tests,
`CustomTestTarget` is essentially a script file and is needed to use the `cmake`
build system testing framework. `CustomTestTarget` supports keywords:
* `INTERPRETER` - interpreter for running the script;
* `SOURCE` - script file with target source code;
* `ARGUMENTS` - command line arguments to run the script;
* `DEPENDS` - list of the target's dependencies;
* `DISABLE` - disable autorun test with `make test` command.

The build of this target can be enabled/disabled using the BUILD_TESTS
configuration option.

Custom test build target template:
```
CustomTestTarget(<test_name>
    INTERPRETER <interpreter>
    SOURCE      <source_file>
    ARGUMENTS   <list_of_arguments>
    DEPENDS     <list_of_target_dependencies>
    DISABLE
)
```

## Examples of using targets

### Libraries example

Example of using the library target:
```
LibTarget(shared_lib SHARED
    HEADERS
        shared_lib.h
        details/shared_lib_impl.h
    SOURCES
        details/shared_lib.cpp
        details/shared_lib_impl.cpp
    INCLUDE_DIR     libs
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS
        BOOST_LOG_DYN_LINK
    LIBRARIES
        interface_lib
        shared_lib_2
        static_lib
    DEPENDS
        boost
        ext1
        ext2
)
```

### Executables example

Example of using the executable target:
```
ExecTarget(executable
    HEADERS
        executable.h
    SOURCES
        main.cpp
    LIBRARIES
        interface_lib
        shared_lib
        static_lib
    DEPENDS
        boost
        ext1
        ext2
)
```

### Tests example

Example of using the test target:
```
TestTarget(ut_test
    HEADERS
        test.h
    SOURCES
        ut_test.cpp
    LIBRARIES
        interface_lib
        shared_lib
        static_lib
    DEPENDS
        boost
        ext1
        ext2
    DISABLE
)
```

### Drivers example

Example of using the driver target:
```
DriverTarget(hello_module
    INCLUDE_DIRS
        module
    SOURCES
        main.c
    EXTRA_CFLAGS
        -Wno-unused-variable
        -Wno-unused-function
    EXTRA_LDFLAGS
        --strip-all
        -O3
    DEFINES
        MOD_NAME="hello_module"
        MOD_CONFIG
        MOD_NUM=1
)
```

### Externals example

Example of using the external target:
```
ExternalTarget(testing
    URL
        https://github.com/wstux/testing_template/archive/refs/heads/master.zip
    CONFIGURE_COMMAND
        cmake --install-prefix ${EXTERNALS_PREFIX}/testing/install  ./
    BUILD_COMMAND
        cmake --build ./
    INSTALL_COMMAND
        cmake --install ./
    PATCHES
        ${CMAKE_SOURCE_DIR}/externals/patches/patch_example.patch
    INCLUDE_DIR
        ${EXTERNALS_PREFIX}/testing/install/include
    INSTALL_DIR
        ${EXTERNALS_PREFIX}/testing/install
)
```

### Custom tests example

Example of using the custom test target:
```
CustomTestTarget(ut_custom_test_target_with_args
    SOURCE
        ut_custom_test_target_with_args.sh
    INTERPRETER
        /bin/bash
    ARGUMENTS
        ${CMAKE_BINARY_DIR}
    DISABLE
)
```

## ToDo

To do:
* for `LibTarget` add the ability to specify several types of libraries for one
  target (`SHARED` and `STATIC`);
* for `LibTarget` change `HEADERS` or `INCLUDE_DIR` to specify the public
  interface, and make the rest private, [example](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/).

## License

&copy; 2022 Chistyakov Alexander.

Open sourced under MIT license, the terms of which can be read here — [MIT License](http://opensource.org/licenses/MIT).
