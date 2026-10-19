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
# Utilities
################################################################################

macro(_enable_sanitizer OPT SANITIZER)
    if(${${OPT}})
        log_info("Building project with ${SANITIZER} sanitizer")
        set_flag("-fsanitize=${SANITIZER}")
        set_linker_flag("-fsanitize=${SANITIZER}")
    endif()
endmacro()

################################################################################
# Setting sanitizer flags
################################################################################

_enable_sanitizer(USE_ADDR_SANITIZER "address")
_enable_sanitizer(USE_LEAK_SANITIZER "leak")
_enable_sanitizer(USE_BEHAVIOR_SANITIZER "undefined")

if (USE_THREAD_SANITIZER)
    if (USE_ADDR_SANITIZER OR USE_LEAK_SANITIZER)
        log_fatal("Thread sanitizer does not work with address and leak sanitizers")
    endif()
    _enable_sanitizer(USE_THREAD_SANITIZER "thread")
endif()

if (USE_BEHAVIOR_SANITIZER)
    # Force the program to crash on the first UB error. By default, UBSan simply
    # writes to the console and continues running. The flag below will turn
    # warnings into critical runtime errors.
    #set_flag("-fno-sanitize-recover=undefined")
endif()

if (USE_ADDR_SANITIZER OR USE_LEAK_SANITIZER OR USE_BEHAVIOR_SANITIZER OR USE_THREAD_SANITIZER)
    set_flag("-fno-omit-frame-pointer -g")
endif()
