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

################################################################################
# Set compiler standard
################################################################################

if (CMAKE_C_COMPILER AND NOT CMAKE_CXX_COMPILER)
    if (PROJECT_C_STANDARD)
        # Setting the C standard version from defined variable
        SetCStandard("${PROJECT_C_STANDARD}")
    elseif (USE_DEFAULT_STANDARD)
        # Setting the default C standard version
        SetCStandard("default")
    endif()
endif()

if (CMAKE_CXX_COMPILER)
    if (PROJECT_CXX_STANDARD)
        # Setting the C++ standard version from defined variable
        SetCxxStandard("${PROJECT_CXX_STANDARD}")
    elseif (USE_DEFAULT_STANDARD)
        # Setting the default C++ standard version
        SetCxxStandard("default")
    endif()
endif()

################################################################################
# Setting common compile flags
################################################################################

set_flag("-Os -DNDEBUG"     MINSIZEREL)
set_flag("-O3 -DNDEBUG"     RELEASE)
set_flag("-O2 -DNDEBUG -g3" RELWITHDEBINFO)
set_flag("-O0 -g3"          DEBUG)
set_flag("-Wall -Wextra")

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set_flag("-rdynamic")
    set_flag("-fPIC")
    set_flag("-ggdb3")
    set_flag("-ffunction-sections")
    set_flag("-fstrict-aliasing")
endif()

#try_set_flag(FPIE "-fPIE")
#try_set_linker_flag(LINKER_PIE "-pie")

# Use hidden symbol visibility if possible.
# void __attribute__((visibility("default"))) Exported() {...}
#try_set_flag(FVISIBILITY_HIDDEN "-fvisibility=hidden")

################################################################################
# Protecting stack
################################################################################

# try_set_flag(FSTACK_PROTECTOR "-fstack-protector-strong")
# if (NOT FLAG_FSTACK_PROTECTOR)
#     try_set_flag(FSTACK_PROTECTOR "-fstack-protector-all")
# endif()
# try_set_flag(WSTACK_PROTECTOR "-Wstack-protector")

# try_set_flag(FNO_STRICT_OVERFLOW "-fno-strict-overflow")

################################################################################
# Set flags if isset options
################################################################################

set_flag_by_opt(USE_FAST_MAT        "--ffast-math")

if (NOT USE_THREAD_SANITIZER)
    set_flag_by_opt(USE_LTO         "-flto=auto")
    set_linker_flag_by_opt(USE_LTO  "-flto=auto")
endif()

set_flag_by_opt(USE_PEDANTIC        "-pedantic")
set_flag_by_opt(USE_PEDANTIC        "-pedantic-errors")
set_flag_by_opt(USE_WERROR          "-Werror")

################################################################################
# Setting linker options
################################################################################

set_linker_flag("-Wl,-rpath=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

################################################################################
# Setting coverage compile flags
################################################################################

if (COVERAGE_BUILD)
    set_flag("-g -O0 --coverage -fprofile-arcs -ftest-coverage")
    set_linker_flag("-fprofile-arcs -ftest-coverage")
endif()
