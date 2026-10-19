# The MIT License
#
# Copyright (c) 2026 Chistyakov Alexander
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

set(LOG_LEVEL_FATAL   0)
set(LOG_LEVEL_ERROR   1)
set(LOG_LEVEL_WARNING 2)
set(LOG_LEVEL_INFO    3)
set(LOG_LEVEL_DEBUG   4)
set(LOG_LEVEL_TRACE   5)

set(__global_log_level ${LOG_LEVEL_INFO})

function(_can_log LEVEL RESULT)
    set(${RESULT} 0 PARENT_SCOPE)
    if (${__global_log_level} GREATER_EQUAL ${LEVEL})
        set(${RESULT} 1 PARENT_SCOPE)
    endif()
endfunction()

function(_level_format LEVEL RESULT_LEVEL RESULT_LEVEL_STR)
    set(${RESULT_LEVEL}     "" PARENT_SCOPE)
    set(${RESULT_LEVEL_STR} "" PARENT_SCOPE)
    if (${LEVEL} EQUAL ${LOG_LEVEL_FATAL})
        set(${RESULT_LEVEL}     FATAL_ERROR PARENT_SCOPE)
        set(${RESULT_LEVEL_STR} "[FATAL]"   PARENT_SCOPE)
    elseif (${LEVEL} EQUAL ${LOG_LEVEL_ERROR})
        set(${RESULT_LEVEL}     SEND_ERROR  PARENT_SCOPE)
        set(${RESULT_LEVEL_STR} "[ERROR]"   PARENT_SCOPE)
    elseif (${LEVEL} EQUAL ${LOG_LEVEL_WARNING})
        set(${RESULT_LEVEL}     WARNING     PARENT_SCOPE)
        set(${RESULT_LEVEL_STR} "[WARN ]"   PARENT_SCOPE)
    elseif (${LEVEL} EQUAL ${LOG_LEVEL_INFO})
        set(${RESULT_LEVEL}     STATUS      PARENT_SCOPE)
        set(${RESULT_LEVEL_STR} "[INFO ]"   PARENT_SCOPE)
    elseif (${LEVEL} EQUAL ${LOG_LEVEL_DEBUG})
        set(${RESULT_LEVEL}     STATUS      PARENT_SCOPE)
        set(${RESULT_LEVEL_STR} "[DEBUG]"   PARENT_SCOPE)
    elseif (${LEVEL} EQUAL ${LOG_LEVEL_TRACE})
        set(${RESULT_LEVEL}     STATUS      PARENT_SCOPE)
        set(${RESULT_LEVEL_STR} "[TRACE]"   PARENT_SCOPE)
    endif()
endfunction()

function(_log LEVEL MSG)
    _can_log(${LEVEL} _enable)
    if (_enable)
        _level_format(${LEVEL} _level _level_str)
        message(${_level} "${_level_str} ${MSG}")
    endif()
endfunction()

################################################################################
# Logging interface
################################################################################

function(log_fatal MSG)
    _log(${LOG_LEVEL_FATAL}   "${MSG}")
endfunction()

function(log_error MSG)
    _log(${LOG_LEVEL_ERROR}   "${MSG}")
endfunction()

function(log_warn MSG)
    _log(${LOG_LEVEL_WARNING} "${MSG}")
endfunction()

function(log_info MSG)
    _log(${LOG_LEVEL_INFO}    "${MSG}")
endfunction()

function(log_debug MSG)
    _log(${LOG_LEVEL_DEBUG}   "${MSG}")
endfunction()

function(log_trace MSG)
    _log(${LOG_LEVEL_TRACE}   "${MSG}")
endfunction()
