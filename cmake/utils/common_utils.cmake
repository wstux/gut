# The MIT License
#
# Copyright (c) 2022 wstux
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

function(_is_supported_platform PLATFORM_NAME RESULT_VAL)
    set(${RESULT_VAL} 0 PARENT_SCOPE)
    get_property(_supported_platform_list GLOBAL PROPERTY supported_platform_list)

    if (NOT DEFINED _supported_platform_list)
        return()
    endif()

    list(FIND _supported_platform_list "${PLATFORM_NAME}" IS_FIND)
    if (NOT IS_FIND EQUAL -1)
        set(${RESULT_VAL} 1 PARENT_SCOPE)
    endif()
endfunction()

################################################################################
# Adding value at the end of list.
# LIST_OF_VALUES - list of values ​​where a new value should be added.
# VALUE          - a new value to add to the end of the list.
#
# Example:
#   foreach (_val IN LISTS values)
#       _append_to_list(_new_list "${_val}")
#   endforeach()
################################################################################
function(_append_to_list LIST_OF_VALUES VALUE)
    if (${LIST_OF_VALUES})
        set(${LIST_OF_VALUES} "${${LIST_OF_VALUES}}" "${VALUE}" PARENT_SCOPE)
    else()
        set(${LIST_OF_VALUES} "${VALUE}" PARENT_SCOPE)
    endif()
endfunction()

################################################################################
# Adding value at the end of string.
# LIST_OF_VALUES - list of values ​​where a new value should be added.
# VALUE          - a new value to add to the end of the list.
#
# Example:
#   foreach (_val IN LISTS values)
#       _push_back(_new_list "${_val}")
#   endforeach()
################################################################################
function(_push_back LIST_OF_VALUES VALUE)
    if (${LIST_OF_VALUES})
        string(REPLACE " " ";" _tmp_values_list "${${LIST_OF_VALUES}}")
        list(FIND _tmp_values_list "${VALUE}" _value_index)
        if (_value_index EQUAL -1)
            set(${LIST_OF_VALUES} "${${LIST_OF_VALUES}} ${VALUE}" PARENT_SCOPE)
        endif()
    else()
        set(${LIST_OF_VALUES} "${VALUE}" PARENT_SCOPE)
    endif()
endfunction()
