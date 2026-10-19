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

################################################################################
# Cppcheck static analyzer
################################################################################

find_program(CPPCHECK_EXE cppcheck DOC "cppcheck path")
if (CPPCHECK_EXE)
    set(_cppcheck_dir           "${CMAKE_BINARY_DIR}/lint/cppcheck")
    set(_cppcheck_suppressions  "${CMAKE_SOURCE_DIR}/lint/cppcheck_suppressions.lint")
    set(_cppcheck_src_dir       "${CMAKE_SOURCE_DIR}/src")

    set(_cppcheck_report_dir            "${_cppcheck_dir}/report")
    set(_cppcheck_report_error_path     "${_cppcheck_dir}/cppcheck_err.xml")

    CustomTarget(stat_cppcheck
        COMMAND bash -c "mkdir -p ${_cppcheck_dir}"
        COMMAND bash -c "mkdir -p ${_cppcheck_report_dir}"
        COMMAND cppcheck
            #--platform=unix64
            --language=c++
            #--std=c++17
            --suppressions-list=${_cppcheck_suppressions}
            --report-progress --inline-suppr --enable=all --xml
            #--report-progress --enable=all --xml
            ${_cppcheck_src_dir} 2> ${_cppcheck_report_error_path}
        COMMAND cppcheck-htmlreport --title "${CMAKE_PROJECT_NAME} ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}.${${PROJECT_NAME}_BUILD_NUMBER}"
            --file=${_cppcheck_report_error_path} --report-dir=${_cppcheck_report_dir} --source-dir=${CMAKE_SOURCE_DIR} --source-encoding=UTF-8
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
    )
endif()

################################################################################
# Clang static analyzer
################################################################################
