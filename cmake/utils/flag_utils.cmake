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

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

include(logging)
include(utils/common_utils)

################################################################################
# Utilities
################################################################################

################################################################################
# Macro for set build flags
################################################################################

# Set C compiler flag.
macro(set_c_flag FLAG)
    if(CMAKE_C_COMPILER)
        if(${ARGC} GREATER 1)
            _push_back(CMAKE_C_FLAGS_${ARGV1} "${FLAG}")
            log_trace("Macro 'set_c_flag': ARGV1='${ARGV1}', FLAG='${FLAG}'")
        else()
            _push_back(CMAKE_C_FLAGS "${FLAG}")
            log_trace("Macro 'set_c_flag': FLAG='${FLAG}'")
        endif()
    endif()
endmacro()

# Set C++ compiler flag.
macro(set_cxx_flag FLAG)
    if(CMAKE_CXX_COMPILER)
        if(${ARGC} GREATER 1)
            _push_back(CMAKE_CXX_FLAGS_${ARGV1} "${FLAG}")
            log_trace("Macro 'set_cxx_flag': ARGV1='${ARGV1}', FLAG='${FLAG}'")
        else()
            _push_back(CMAKE_CXX_FLAGS "${FLAG}")
            log_trace("Macro 'set_cxx_flag': FLAG='${FLAG}'")
        endif()
    endif()
endmacro()

# Set C and C++ compiler flag.
macro(set_flag FLAG)
    set_c_flag(${FLAG} ${ARGN})
    set_cxx_flag(${FLAG} ${ARGN})
endmacro()

# Set C and C++ compiler flag if isset option.
macro(set_flag_by_opt OPT FLAG)
    log_trace("Macro 'set_flag_by_opt': OPT='${OPT}', OPT VALUE='${${OPT}}'")
    if("${${OPT}}" STREQUAL "ON")
        set_flag(${FLAG})
    endif()
endmacro()

# Try set C compiler flag if flag supported.
macro(try_set_c_flag PROP FLAG)
    if (CMAKE_C_COMPILER)
        log_trace("Macro 'try_set_c_flag': PROP='${PROP}', FLAG='${FLAG}', ARGV2='${ARGV2}'")

        set(CMAKE_REQUIRED_QUIET TRUE)
        check_c_compiler_flag(${FLAG} FLAG_${PROP})
        if (FLAG_${PROP})
            set_c_flag(${FLAG} ${ARGV2})
        endif()
    endif()
endmacro()

# Try set C++ compiler flag if flag supported.
macro(try_set_cxx_flag PROP FLAG)
    if (CMAKE_CXX_COMPILER)
        log_trace("Macro 'try_set_cxx_flag': PROP='${PROP}', FLAG='${FLAG}', ARGV2='${ARGV2}'")

        set(CMAKE_REQUIRED_QUIET TRUE)
        check_cxx_compiler_flag(${FLAG} FLAG_${PROP})
        if (FLAG_${PROP})
            set_cxx_flag(${FLAG} ${ARGV2})
        endif()
    endif()
endmacro()

# Try set C and C++ compiler flag if flag supported.
macro(try_set_flag PROP FLAG)
    log_trace("Macro 'try_set_flag': PROP='${PROP}', FLAG='${FLAG}', ARGV2='${ARGV2}'")
    set(CMAKE_REQUIRED_QUIET TRUE)
    if (CMAKE_C_COMPILER)
        check_C_compiler_flag(${FLAG} FLAG_${PROP})
    endif()
    if (CMAKE_CXX_COMPILER)
        check_cxx_compiler_flag(${FLAG} FLAG_${PROP})
    endif()
    if(FLAG_${PROP})
        set_c_flag(${FLAG} ${ARGV2})
        set_cxx_flag(${FLAG} ${ARGV2})
    endif()
endmacro()

# Try set C and C++ compiler flag if isset option.
macro(try_set_flag_by_opt OPT FLAG)
    log_trace("Macro 'try_set_flag_by_opt': OPT='${OPT}', OPT VALUE='${${OPT}}', FLAG='${FLAG}'")
    if("${${OPT}}" STREQUAL "ON")
        try_set_flag(${OPT} ${FLAG} ${ARGV2})
    endif()
endmacro()

################################################################################
# Macro for set linker flags
################################################################################

# Set linker flag.
macro(set_linker_flag FLAG)
    _push_back(CMAKE_EXE_LINKER_FLAGS    "${FLAG}")
    _push_back(CMAKE_SHARED_LINKER_FLAGS "${FLAG}")
    _push_back(CMAKE_MODULE_LINKER_FLAGS "${FLAG}")
    log_trace("Macro 'set_linker_flag': FLAG='${FLAG}'")
endmacro()

# Set linker flag if isset option.
macro(set_linker_flag_by_opt OPT FLAG)
    log_trace("Macro 'set_linker_flag_by_opt': OPT='${OPT}', OPT VALUE='${${OPT}}'")
    if ("${${OPT}}" STREQUAL "ON")
        set_linker_flag(${FLAG})
    endif()
endmacro()

# Try set linker flag.
macro(try_set_linker_flag PROP FLAG)
    # Check it with the C compiler
    set(CMAKE_REQUIRED_QUIET TRUE)
    set(CMAKE_REQUIRED_FLAGS ${FLAG})
    check_C_compiler_flag(${FLAG} FLAG_${PROP})
    set(CMAKE_REQUIRED_FLAGS "")
    if (FLAG_${PROP})
        set_linker_flag(${FLAG})
    endif()
endmacro()

# Try set linker flag if isset option.
macro(try_set_linker_flag_by_opt OPT FLAG)
    log_trace("Macro 'try_set_linker_flag_by_opt': OPT='${OPT}', OPT VALUE='${${OPT}}', FLAG='${FLAG}'")
    if ("${${OPT}}" STREQUAL "ON")
        try_set_linker_flag(${OPT} ${FLAG})
    endif()
endmacro()
