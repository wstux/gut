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

include(utils/flag_utils)

function(AddPlatform PLATFORM_NAME)
    get_property(_supported_platform_list GLOBAL PROPERTY supported_platform_list)

    list(APPEND _supported_platform_list "${PLATFORM_NAME}")
    set_property(GLOBAL PROPERTY supported_platform_list "${_supported_platform_list}")
endfunction()

################################################################################
# Public functions for setting global compilation options
################################################################################

macro(SetCxxStandard STANDARD)
    set(_std_version "${STANDARD}")

    string(TOLOWER ${STANDARD} STANDARD)
    if ("${STANDARD}" STREQUAL "default")
        set(_std_version "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT}")
    endif()

    set(_cxx_std "-std=gnu++${_std_version}")
    #set(_cxx_std "-std=c++${_std_version}")
    try_set_cxx_flag(CXX_STD "${_cxx_std}")
    if (FLAG_CXX_STD)
        log_info("Using C++${_std_version} standard")
    else ()
        log_fatal("Failed to set C++${_std_version} standard")
    endif()
endmacro()

macro(SetCStandard STANDARD)
    set(_std_version "${STANDARD}")

    string(TOLOWER ${STANDARD} STANDARD)
    if ("${STANDARD}" STREQUAL "default")
        set(_std_version "${CMAKE_C_STANDARD_COMPUTED_DEFAULT}")
    endif()

    set(_c_std "-std=gnu${_std_version}")
    #set(_cxx_std "-std=c${_std_version}")
    try_set_c_flag(C_STD "${_c_std}")
    if (FLAG_C_STD)
        log_info("Using C${_std_version} standard")
    else ()
        log_fatal("Failed to set C${_std_version} standard")
    endif()
endmacro()
