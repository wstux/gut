cmake_minimum_required (VERSION 3.10)

################################################################################
# Project definition
################################################################################

project(gut
        VERSION 0.0.1
        LANGUAGES C CXX)

################################################################################
# Build options
################################################################################

option(USE_ADDR_SANITIZER       "Build project with address sanitizer" OFF)
option(USE_LEAK_SANITIZER       "Build project with leak sanitizer" OFF)
option(USE_BEHAVIOR_SANITIZER   "Build project with undefined behavior sanitizer" OFF)
option(USE_THREAD_SANITIZER     "Build project with thread sanitizer" OFF)

option(COVERAGE_BUILD       "Building a project in coverage mode" OFF)

option(USE_DEFAULT_STANDARD "Using the C/C++ standard by default" ON)
option(USE_FAST_MATH        "Tell the compiler to use fast math" OFF)
option(USE_LTO              "Use link-time optimization for release builds" ON)
option(USE_PEDANTIC         "Tell the compiler to be pedantic" ON)
option(USE_WERROR           "Tell the compiler to make the build fail when warnings are present" ON)

option(BUILD_BOOST          "Build boost" ON)
option(BUILD_EXAMPLES       "Build examples" OFF)
option(BUILD_TESTS          "Build perftests and unittests" ON)

################################################################################
# Init cmake modules path
################################################################################

set(COMMON_CMAKE_DIR cmake)
set(COMMON_MAIN_CMAKE ${COMMON_CMAKE_DIR}/main.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/${COMMON_CMAKE_DIR} ${CMAKE_MODULE_PATH})

include(${COMMON_MAIN_CMAKE} PUBLIC)

################################################################################
# Include source code
################################################################################

add_subdirectory(externals)
add_subdirectory(src)
