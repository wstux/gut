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

################################################################################
# Functions
################################################################################

function(_get_qualifier TARGET_NAME RESULT)
    set(${RESULT} PUBLIC PARENT_SCOPE)
    get_target_property(_type ${TARGET_NAME} TYPE)
    if (_type STREQUAL "INTERFACE_LIBRARY")
        set(${RESULT} INTERFACE PARENT_SCOPE)
    endif()
endfunction()

function(_get_default_include_dirs TARGET_NAME RESULT)
    set(_include_dir "${CMAKE_CURRENT_SOURCE_DIR}")

    cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH _lib_dir)
    if (_lib_dir)
        set(_include_dir "${_lib_dir}")
    endif()

    set(${RESULT} "${_include_dir}" PARENT_SCOPE)
endfunction()

function(_configure_target TARGET_NAME)
    # Setting of default include directories.
    _get_default_include_dirs(${TARGET_NAME}    _dfl_include_dirs)
    _get_qualifier(${TARGET_NAME}               _qualifier)
    if (NOT ${TARGET_NAME}_INCLUDE_DIR)
        target_include_directories(${TARGET_NAME}
            ${_qualifier} ${_dfl_include_dirs}
        )
    endif()

    if (${TARGET_NAME}_COMPILE_DEFINITIONS)
        foreach (_def IN LISTS ${TARGET_NAME}_COMPILE_DEFINITIONS)
            _get_qualifier(${TARGET_NAME}   _qualifier)
            target_compile_definitions(${TARGET_NAME} ${_qualifier} ${_def})
        endforeach()
    endif()

    get_property(_depends DIRECTORY PROPERTY ${TARGET_NAME}_DEPENDS)
    foreach (_dep IN LISTS _depends)
        if (NOT TARGET ${_dep})
            target_link_libraries(${TARGET_NAME} ${_qualifier} ${_dep})
        else()
            add_dependencies(${TARGET_NAME} ${_dep})
        endif()
    endforeach()

    get_property(_libraries DIRECTORY PROPERTY ${TARGET_NAME}_LIBRARIES)
    if (_libraries)
        target_link_libraries(${TARGET_NAME} ${_qualifier} ${_libraries})
    endif()

    foreach (_lib IN LISTS _libraries _depends)
        if (NOT TARGET ${_lib})
            continue()
        endif()

        get_target_property(_target_type ${_lib} TYPE)
        if (_target_type STREQUAL "INTERFACE_LIBRARY")
            get_property(_include_dirs TARGET ${_lib} PROPERTY ${_lib}_INCLUDE_DIR)
            if (NOT _include_dirs)
                get_property(_include_dirs TARGET ${_lib} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
            endif()
            if (_include_dirs)
                target_include_directories(${TARGET_NAME} ${_qualifier} ${_include_dirs})
            endif()
            continue()
        endif()

        if (_target_type STREQUAL "STATIC_LIBRARY" OR _target_type STREQUAL "MODULE_LIBRARY" OR _target_type STREQUAL "SHARED_LIBRARY")
            target_link_libraries(${TARGET_NAME} ${_qualifier} ${_lib})
        endif()

        get_target_property(_dep_libraries ${_lib} LIBRARIES)
        if (_dep_libraries)
            target_link_libraries(${TARGET_NAME} ${_qualifier} ${_dep_libraries})
        endif()

        get_target_property(_include_dirs ${_lib} INCLUDE_DIRECTORIES)
        if (NOT _include_dirs)
            get_property(_include_dirs DIRECTORY PROPERTY ${_lib}_INCLUDE_DIR)
        endif()
        if (_include_dirs)
            target_include_directories(${TARGET_NAME} ${_qualifier} ${_include_dirs})
        endif()
    endforeach()
endfunction()

function(_is_kw CHECK_STR KW_LIST RESULT)
    set(${RESULT} 0 PARENT_SCOPE)
    list(FIND ${KW_LIST} "${CHECK_STR}" IS_FIND)
    if (NOT IS_FIND EQUAL -1)
        set(${RESULT} 1 PARENT_SCOPE)
    endif()
endfunction()

function(_parse_target_args TARGET_NAME KW_FLAGS KW_VALUES KW_LISTS)
    # https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html
    # https://stackoverflow.com/questions/23327687/how-to-write-a-cmake-function-with-more-than-one-parameter-groups
    cmake_parse_arguments(PARSE_ARGV 1 ${TARGET_NAME}
        "${${KW_FLAGS}}" "${${KW_VALUES}}" "${${KW_LISTS}}"
    )

    set(_kw_list "${${KW_FLAGS}}" "${${KW_VALUES}}" "${${KW_LISTS}}")
    foreach (_arg IN LISTS ARGN)
        # Check is 'arg' a keyword.
        _is_kw(${_arg} _kw_list _is_keyword)

        # If 'arg' is keyword - save 'arg' to 'key' variable and save key-flag to parent scope.
        if (_is_keyword)
            set(_key "${_arg}")
            set_property(DIRECTORY APPEND PROPERTY ${TARGET_NAME}_${_key} "${${TARGET_NAME}_${_key}}")
            set(${TARGET_NAME}_${_key} "${${TARGET_NAME}_${_key}}" PARENT_SCOPE)
        endif()
    endforeach()
endfunction()

function(_parse_target_args_strings TARGET_NAME KW_FLAGS KW_VALUES KW_LISTS)
    set(_kw_flags   "${${KW_FLAGS}}")
    set(_kw_values  "${${KW_VALUES}}")
    set(_kw_lists   "${${KW_LISTS}}")
    set(_kw_list    "${_kw_flags}" "${_kw_values}" "${_kw_lists}")
    set(_key        "")

    foreach (_arg IN LISTS ARGN)
        # Check is 'arg' a keyword.
        _is_kw(${_arg} _kw_list _is_keyword)
        if (_is_keyword)
            set(_key "${_arg}")

            _is_kw(${_key} _kw_flags _is_flag)
            if (_is_flag)
                set(${TARGET_NAME}_${_key} 1 PARENT_SCOPE)
            endif()
        endif()

        if (_key AND NOT _is_keyword)
            _is_kw(${_key} _kw_values _is_value)
            if (_is_value)
                if (${TARGET_NAME}_${_key})
                    set(${TARGET_NAME}_${_key} "${${TARGET_NAME}_${_key}} ${_arg}")
                else()
                    set(${TARGET_NAME}_${_key} "${_arg}")
                endif()
                set(${TARGET_NAME}_${_key} "${${TARGET_NAME}_${_key}}" PARENT_SCOPE)
            endif()

            _is_kw(${_key} _kw_lists _is_list)
            if (_is_list)
                if (${TARGET_NAME}_${_key})
                    set(${TARGET_NAME}_${_key} "${${TARGET_NAME}_${_key}}" "${_arg}")
                else()
                    set(${TARGET_NAME}_${_key} "${_arg}")
                endif()
                set(${TARGET_NAME}_${_key} "${${TARGET_NAME}_${_key}}" PARENT_SCOPE)
            endif()
        endif()
    endforeach()
endfunction()
