# The MIT License
#
# Copyright (c) 2023 wstux
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

include(custom_targets)
include(utils/target_utils)

################################################################################
# Functions
################################################################################

function(_generate_perf_tests_runner_script)
    get_property(ALL_PERFS GLOBAL PROPERTY PERF_TEST_TARGETS)

    set(SCRIPT_CONTENT "# Generated automatically by CMake\n")
    foreach(TARGET_NAME ${ALL_PERFS})
        string(APPEND SCRIPT_CONTENT "execute_process(COMMAND \"$<TARGET_FILE:${TARGET_NAME}>\")\n")
    endforeach()

    file(GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/run_all_perf_tests.cmake"
        CONTENT "${SCRIPT_CONTENT}"
    )
endfunction()

################################################################################
# Configure
################################################################################

# Call function _generate_perf_tests_runner_script after reading all CMakeLists.txt files
cmake_language(DEFER CALL _generate_perf_tests_runner_script)

################################################################################
# Targets
################################################################################

function(TestLibTarget TARGET_NAME)
    if (NOT BUILD_TESTS)
        return()
    endif()

    LibTarget(${TARGET_NAME} ${ARGN})
endfunction()

macro(CustomTestTarget TARGET_NAME)
    if (NOT BUILD_TESTS)
        return()
    endif()

    set(_flags_kw   DISABLE)
    set(_values_kw  INTERPRETER SOURCE)
    set(_lists_kw   ARGUMENTS DEPENDS)
    _parse_target_args(${TARGET_NAME}
        _flags_kw _values_kw _lists_kw ${ARGN}
    )

    set(_target_dir "${CMAKE_BINARY_DIR}/test")
    set(_enable_autorun true)
    if (${TARGET_NAME}_DISABLE)
        set(_enable_autorun false)
    endif()

    get_filename_component(_src_filename ${${TARGET_NAME}_SOURCE} NAME_WE)
    set(_target_sh "${_target_dir}/${_src_filename}.sh")
    set(_src_interpreter "${${TARGET_NAME}_INTERPRETER}")
    set(_src_file "${CMAKE_CURRENT_SOURCE_DIR}/${${TARGET_NAME}_SOURCE}")
    set(_src_args "")
    foreach (_arg IN LISTS ${TARGET_NAME}_ARGUMENTS)
        set(_src_args "${_src_args} ${_arg}")
    endforeach()
    file(GENERATE OUTPUT ${_target_sh}
         CONTENT    "${_src_interpreter} ${_src_file} ${_src_args}\n"
         FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    )

    CustomTarget(${TARGET_NAME}
        COMMAND ${_target_sh}
    )
    foreach (_dep IN LISTS ${TARGET_NAME}_DEPENDS)
        add_dependencies(${TARGET_NAME} ${_dep})
    endforeach()

    if (${_enable_autorun})
        add_test(
            NAME ${TARGET_NAME}
            COMMAND sh ${_target_sh}
        )
    endif()

    CustomTarget(${TARGET_NAME}_run
        COMMAND sh "${_target_dir}/${TARGET_NAME}"
        DEPENDS ${TARGET_NAME}
        VERBATIM
    )
endmacro()

function(PerfTestTarget TARGET_NAME)
    if (NOT BUILD_TESTS)
        return()
    endif()

    TestTarget(${TARGET_NAME} ${ARGN} DISABLE)

    set_property(GLOBAL APPEND PROPERTY PERF_TEST_TARGETS ${TARGET_NAME})

    add_dependencies(PerfTestsAllRun ${TARGET_NAME})
endfunction()

CustomTarget(PerfTestsAllRun
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/run_all_perf_tests.cmake"
    COMMENT "Running all performance tests sequentially..."
    VERBATIM
)
