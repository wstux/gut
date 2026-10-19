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

include(utils/target_utils)

################################################################################
# Targets
################################################################################

macro(CustomCommand)
    add_custom_command(${ARGN})
endmacro()

macro(CustomTarget TARGET_NAME)
    add_custom_target(${TARGET_NAME} ${ARGN})
endmacro()

macro(ConfigureFile TARGET_NAME CONF_FILE)
    set(_configure_variables)
    foreach(_conf_var IN ITEMS ${ARGN})
        list(APPEND _configure_variables -D${_conf_var})
    endforeach()

    set(_file "${CONF_FILE}")
    CustomCommand(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
        COMMAND ${CMAKE_COMMAND}
                -DINFILE=${CMAKE_CURRENT_SOURCE_DIR}/${_file}.in
                -DOUTFILE=${CMAKE_CURRENT_BINARY_DIR}/${_file}
                ${_configure_variables}
                -P ${CMAKE_SOURCE_DIR}/cmake/configure_file.cmake
        VERBATIM
    )

    CustomTarget(${TARGET_NAME}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    )
endmacro()
