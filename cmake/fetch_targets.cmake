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

include(FetchContent)

include(utils/target_utils)
include(utils/common_utils)

################################################################################
# Setting of cmake policies
################################################################################

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

################################################################################
# Constants
################################################################################

set(FETCH_CONTENT_PREFIX "${CMAKE_BINARY_DIR}/externals")

################################################################################
# Utility functions
################################################################################

function(_get_key_value INPUT_STRING KEY VALUE)
    if (INPUT_STRING MATCHES "^([^=]+)=(.*)$")
        set(${KEY} "${CMAKE_MATCH_1}" PARENT_SCOPE)
        set(${VALUE} "${CMAKE_MATCH_2}" PARENT_SCOPE)
    endif()
endfunction()

################################################################################
# Targets
################################################################################

function(FetchTarget EXT_TARGET_NAME)
    set(_flags_kw   )
    set(_values_kw  GIT_REPOSITORY
                    GIT_TAG
                    URL
                    URL_MD5
    )
    set(_lists_kw   CMAKE_OPTIONS
                    CMAKE_VARIABLES
#                    COMPILE_OPTIONS
#                    CXX_COMPILE_FLAGS
                    LIBRARIES
    )
    _parse_target_args_strings(${EXT_TARGET_NAME}
        _flags_kw _values_kw _lists_kw ${ARGN}
    )

    set(_target_name    "${EXT_TARGET_NAME}")
    set(_target_dir     "${EXTERNALS_PREFIX}/${_target_name}")

    set(_ext_url        ${${EXT_TARGET_NAME}_URL})
    set(_ext_url_md5    ${${EXT_TARGET_NAME}_URL_MD5})

    set(_ext_git        ${${EXT_TARGET_NAME}_GIT_REPOSITORY})
    set(_ext_git_tag    ${${EXT_TARGET_NAME}_GIT_TAG})

    ############################################################################
    # Setting of compile flags
    ############################################################################

#    foreach (_opt IN LISTS ${EXT_TARGET_NAME}_COMPILE_OPTIONS)
#        add_compile_options(${_opt})
#    endforeach()

#    foreach (_flag IN LISTS ${EXT_TARGET_NAME}_CXX_COMPILE_FLAGS)
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flag}")
#    endforeach()

    ############################################################################
    # Setting of cmake configure options
    ############################################################################

    foreach (_opt IN LISTS ${EXT_TARGET_NAME}_CMAKE_OPTIONS)
        _get_key_value("${_opt}" _opt_name _opt_value)
        option(${_opt_name} "" ${_opt_value})
    endforeach()

    foreach (_opt IN LISTS ${EXT_TARGET_NAME}_CMAKE_VARIABLES)
        _get_key_value("${_opt}" _var_name _var_value)
        set(${_var_name} ${_var_value} CACHE INTERNAL "Forced external variable" FORCE)
    endforeach()

    ############################################################################
    # Fills list of libraries
    ############################################################################

    set(_libraries "")
    if (${EXT_TARGET_NAME}_LIBRARIES)
        foreach (_lib IN LISTS ${EXT_TARGET_NAME}_LIBRARIES)
            _append_to_list(_libraries "${_lib}")
        endforeach()
    endif()

    ############################################################################
    # Fetching of source code of target
    ############################################################################

    set(_src_dir      "${FETCH_CONTENT_PREFIX}/${_target_name}/${_target_name}")
    set(_subbuild_dir "${FETCH_CONTENT_PREFIX}/${_target_name}/subbuild/")
    set(_bin_dir      "${FETCH_CONTENT_PREFIX}/${_target_name}/install/")
    if (_ext_git)
        FetchContent_Declare(${_target_name}
            GIT_REPOSITORY  ${_ext_git}
            GIT_TAG         ${_ext_git_tag}
            SOURCE_DIR      "${_src_dir}"
            BINARY_DIR      "${_bin_dir}"
            SUBBUILD_DIR    "${_subbuild_dir}"
            UPDATE_DISCONNECTED ON
            #UPDATE_COMMAND  ""
            #PATCH_COMMAND   ""
            OVERRIDE_FIND_PACKAGE
            EXCLUDE_FROM_ALL
        )
    elseif (_ext_url)
        FetchContent_Declare(${_target_name}
            URL             ${_ext_url}
            URL_HASH        MD5=${_ext_url_md5}
            SOURCE_DIR      "${_src_dir}"
            BINARY_DIR      "${_bin_dir}"
            SUBBUILD_DIR    "${_subbuild_dir}"
            #UPDATE_DISCONNECTED ON
            #UPDATE_COMMAND  ""
            #PATCH_COMMAND   ""
            OVERRIDE_FIND_PACKAGE
            EXCLUDE_FROM_ALL
        )
    endif()

    FetchContent_MakeAvailable(${_target_name})

    #set(_src_dir ${${_target_name}_SOURCE_DIR})
    #set(_bin_dir ${${_target_name}_BINARY_DIR})
    #set(_include_dir "${_src_dir}")
    #if (${_target_name}_INCLUDE_DIR)
    #    set(_include_dir "${_src_dir}/${${_target_name}_INCLUDE_DIR}")
    #endif()
    #include_directories(${_include_dir})
    #include_directories(${_bin_dir})
    set_target_properties(${EXT_TARGET_NAME} PROPERTIES
        LIBRARIES       "${_libraries}"
    )
endfunction()
