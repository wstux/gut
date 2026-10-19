# The MIT License
#
# Copyright (c) 2025 wstux
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

function(InstallHook HOOK_NAME)
    if (NOT GIT_FOUND)
        find_package(Git QUIET)
    endif()

    if (GIT_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.git)
        if (NOT EXISTS ${PROJECT_SOURCE_DIR}/.git/hooks/${HOOK_NAME})
            configure_file(
                ${PROJECT_SOURCE_DIR}/cmake/hooks/${HOOK_NAME}.in
                ${PROJECT_SOURCE_DIR}/.git/hooks/${HOOK_NAME}
                @ONLY
            )
        endif()
    endif()
endfunction()
