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

include(utils/target_utils)

################################################################################
# Keywords
################################################################################

set(_WRAP_TARGET_FLAGS_KW   VERBATIM
)
set(_WRAP_TARGET_VALUES_KW  BUILD_CMD
                            BUILDSYS
                            CONFIGURE_CMD
                            INCLUDE_DIR
                            SOURCE_DIR
)
set(_WRAP_TARGET_LISTS_KW   LIBRARIES
)

################################################################################
# Targets
################################################################################

function(WrapperTarget TARGET_NAME)
    _parse_target_args(${TARGET_NAME}
        _WRAP_TARGET_FLAGS_KW _WRAP_TARGET_VALUES_KW _WRAP_TARGET_LISTS_KW ${ARGN}
    )

    set(_build_dir "")
    set(_source_dir "")
    if (DEFINED ${TARGET_NAME}_SOURCE_DIR)
        set(_build_dir "${CMAKE_CURRENT_BINARY_DIR}/${${TARGET_NAME}_SOURCE_DIR}")
        set(_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/${${TARGET_NAME}_SOURCE_DIR}")
    else()
        message(FATAL_ERROR " WrapperTarget [${TARGET_NAME}]: SOURCE_DIR is required")
    endif()

    set(_configure_cmd "")
    set(_real_configure_cmd "")
    if (DEFINED ${TARGET_NAME}_BUILDSYS)
        if (${TARGET_NAME}_BUILDSYS STREQUAL cmake)
            set(_real_configure_cmd
                "cmake -S \"${_source_dir}\" -B \"${_build_dir}\" \
-DCMAKE_BUILD_TYPE=\"${CMAKE_BUILD_TYPE}\" \
> \"${_build_dir}/${TARGET_NAME}_wrapper_configure.log\""
            )
        else()
            message(FATAL_ERROR " WrapperTarget [${TARGET_NAME}]: build system \
'${BUILDSYS}' not supported now")
        endif()
    elseif (DEFINED ${TARGET_NAME}_CONFIGURE_CMD)
        set(_real_configure_cmd "${${TARGET_NAME}_CONFIGURE_CMD} \
> \"${_build_dir}/${TARGET_NAME}_wrapper_configure.log\""
        )
    endif()
    if (_real_configure_cmd)
        file(WRITE ${_build_dir}/${TARGET_NAME}_wrapper_configure.sh "${_real_configure_cmd}")
        set(_configure_cmd COMMAND bash ${_build_dir}/${TARGET_NAME}_wrapper_configure.sh)
    endif()

    set(_build_cmd "")
    set(_real_build_cmd "")
    if (DEFINED ${TARGET_NAME}_BUILD_CMD)
        set(_real_build_cmd "${${TARGET_NAME}_BUILD_CMD} \
> \"${_build_dir}/${TARGET_NAME}_wrapper_build.log\""
        )
    else ()
        set(_real_build_cmd "make > \"${_build_dir}/${TARGET_NAME}_wrapper_build.log\"")
    endif()
    if (_real_build_cmd)
        file(WRITE ${_build_dir}/${TARGET_NAME}_wrapper_build.sh "${_real_build_cmd}")
        set(_build_cmd COMMAND bash ${_build_dir}/${TARGET_NAME}_wrapper_build.sh)
    endif()

    if (NOT _configure_cmd AND NOT _build_cmd)
        message(FATAL_ERROR " WrapperTarget [${TARGET_NAME}]: BUILDSYS/CONFIGURE_CMD \
and/or BUILD_CMD is required")
    endif()

    set(_include_dir "")
    if (DEFINED ${TARGET_NAME}_INCLUDE_DIR)
        foreach(_dir IN LISTS ${TARGET_NAME}_INCLUDE_DIR)
            list(APPEND _include_dir "${_dir}")
        endforeach()
    else()
        message(FATAL_ERROR " WrapperTarget [${TARGET_NAME}]: INCLUDE_DIR is required")
    endif()

    set(_libraries "")
    if (DEFINED ${TARGET_NAME}_LIBRARIES)
        foreach(_lib IN LISTS ${TARGET_NAME}_LIBRARIES)
            list(APPEND _libraries ${_lib})
        endforeach()
    endif()

    add_custom_command(
        OUTPUT ${TARGET_NAME}_configure
        ${_configure_cmd}
        ${${TARGET_NAME}_VERBATIM}
    )

    add_custom_command(
        OUTPUT ${TARGET_NAME}_build
        ${_build_cmd}
        WORKING_DIRECTORY ${_build_dir}
        DEPENDS ${TARGET_NAME}_configure
        ${${TARGET_NAME}_VERBATIM}
    )
    add_custom_target(${TARGET_NAME} ALL DEPENDS ${TARGET_NAME}_build)

    if (_libraries)
        set_target_properties(${TARGET_NAME}
                              PROPERTIES
                                  LIBRARIES "${_libraries}"
        )
    endif()

    set_target_properties(${TARGET_NAME}
                          PROPERTIES
                              INCLUDE_DIRECTORIES "${_include_dir}"
    )
endfunction()
