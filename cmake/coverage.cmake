# The MIT License
#
# Copyright (c) 2025 wstux
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

function(_get_lcov_version LCOV_EXE MAJOR_RESULT MINOR_RESULT)
    set(${MAJOR_RESULT} 1 PARENT_SCOPE)
    set(${MINOR_RESULT} 0 PARENT_SCOPE)

    execute_process(
        COMMAND ${LCOV_EXE} --version
        OUTPUT_VARIABLE _lcov_version_output
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(_lcov_version_output MATCHES "version ([0-9]+)\\.([0-9]+)")
        set(${MAJOR_RESULT} ${CMAKE_MATCH_1} PARENT_SCOPE)
        set(${MINOR_RESULT} ${CMAKE_MATCH_2} PARENT_SCOPE)
    endif()
endfunction()

if (COVERAGE_BUILD)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "debug")
        message(FATAL_ERROR "Code coverage supports only debug build")
    endif()

    find_program(GCOV_EXECUTABLE NAMES gcov)
    if (NOT GCOV_EXECUTABLE)
        message(WARNING "gcov not found")
        return()
    endif()
    find_program(LCOV_EXECUTABLE NAMES lcov)
    if (NOT LCOV_EXECUTABLE)
        message(WARNING "lcov not found")
        return()
    endif()
    find_program(GENHTML_EXECUTABLE NAMES genhtml)
    if (NOT GENHTML_EXECUTABLE)
        message(WARNING "genhtml not found")
        return()
    endif()

    _get_lcov_version(${LCOV_EXECUTABLE} _lcov_version_major _lcov_version_minor)

    set(_coverage_dir   "${CMAKE_BINARY_DIR}/__coverage")
    set(_lcov_out       "${_coverage_dir}/lcov_base.info")
    set(_lcov_final_out "${_coverage_dir}/lcov_final.info")
    set(_genhtml_out    "${_coverage_dir}/report")
    set(_cov_log_file)
    if (TRUE)
        set(_cov_log_file   > ${_coverage_dir}/cov_generate.log 2>&1)
    endif()

    file(MAKE_DIRECTORY "${_coverage_dir}")

    set(_lcov_ignore_errors "source")
    if(_lcov_version_major GREATER_EQUAL 2)
        set(_lcov_ignore_errors "deprecated,inconsistent,mismatch,negative")
    endif()
    set(_lcov_args      --follow --ignore-errors ${_lcov_ignore_errors} --rc lcov_branch_coverage=1)
    CustomTarget(coverage
        # Cleanup lcov
        COMMAND ${LCOV_EXECUTABLE} ${_lcov_args} --initial --zerocounters
                    --directory "${CMAKE_BINARY_DIR}" ${_cov_log_file}
        # Run tests
        COMMAND ${CMAKE_MAKE_PROGRAM} test #make coverage/test
        # Capturing lcov counters
        COMMAND ${LCOV_EXECUTABLE} ${_lcov_args} --capture
                    --directory "${CMAKE_BINARY_DIR}"
                    --output-file ${_lcov_out} ${_cov_log_file}
        COMMAND ${LCOV_EXECUTABLE} ${_lcov_args} --add-tracefile ${_lcov_out}
                    --add-tracefile ${_lcov_out}
                    --output-file ${_lcov_out} ${_cov_log_file}
        COMMAND ${LCOV_EXECUTABLE} ${_lcov_args} --remove ${_lcov_out}
                    "*/usr/include*" "*${CMAKE_BINARY_DIR}*"
                    --output-file ${_lcov_final_out} ${_cov_log_file}
        # Generating report
        COMMAND ${GENHTML_EXECUTABLE} --branch-coverage --demangle-cpp --legend
                    --show-details --ignore-errors ${_lcov_ignore_errors}
                    --title "${PROJECT_NAME}"
                    --output-directory ${_genhtml_out} ${_lcov_final_out} ${_cov_log_file}
        # Print summary
        COMMAND ${LCOV_EXECUTABLE} --summary ${_lcov_final_out}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Processing code coverage counters and generating report."
    )
endif()
