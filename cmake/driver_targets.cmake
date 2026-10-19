# The MIT License
#
# Copyright (c) 2023 wstux
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

set(_DRIVER_FLAGS_KW    )
set(_DRIVER_VALUES_KW   )
set(_DRIVER_LISTS_KW    COMPILE_DEFINITIONS
                        EXTRA_CFLAGS
                        EXTRA_LDFLAGS
                        INCLUDE_DIRS
                        SOURCES
)

################################################################################
# Functions
################################################################################

function(_IsLinuxHeadersInstalled RESULT)
    set(${RESULT} 0 PARENT_SCOPE)
    # 1. Get current kernel version with 'uname -r'
    execute_process(
        COMMAND uname -r
        OUTPUT_VARIABLE _kernel_release
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # 2. Check headers with standard linux path.
    find_path(KERNEL_HEADERS_DIR
        NAMES include/linux/kernel.h
        PATHS "/usr/src/linux-headers-${_kernel_release}"
        NO_DEFAULT_PATH
    )
    if (KERNEL_HEADERS_DIR)
        set(${RESULT} 1 PARENT_SCOPE)
    endif()
endfunction()

function(_LinuxDriverTarget TARGET_NAME)
    set(_module_target  "${TARGET_NAME}.ko")
    set(_module_name    "${TARGET_NAME}")

    ############################################################################
    # Init module source directory.
    file(RELATIVE_PATH _rel_source_dir ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    set(_module_src_root_dir "${CMAKE_BINARY_DIR}/__kbuild_src/${_rel_source_dir}")
    file(MAKE_DIRECTORY ${_module_src_root_dir})

    set(_module_build_dir   "${CMAKE_CURRENT_BINARY_DIR}")

    ############################################################################
    # Adding header directories to build flags.
    set(_module_include_dirs "EXTRA_CFLAGS += -I${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_dir IN LISTS ${TARGET_NAME}_INCLUDE_DIRS)
        set(_module_include_dirs "${_module_include_dirs}\nEXTRA_CFLAGS += -I${CMAKE_CURRENT_SOURCE_DIR}/${_dir}")
    endforeach()

    ############################################################################
    # Add compile flags
    set(_extra_flags "EXTRA_CFLAGS += -fno-pie")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-unused-variable")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-unused-label")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-unused-function")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-cast-function-type")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-unknown-pragmas")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-address")
    set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -Wno-vla -g")
    if (CMAKE_BUILD_TYPE STREQUAL "release" OR CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += -O3")
    endif()
    foreach (_cflag IN LISTS ${TARGET_NAME}_EXTRA_CFLAGS)
        set(_extra_flags "${_extra_flags}\nEXTRA_CFLAGS += ${_cflag}")
    endforeach()

    ############################################################################
    # Add linkage flags
    foreach (_ldflag IN LISTS ${TARGET_NAME}_EXTRA_LDFLAGS)
        set(_extra_ld_flags "${_extra_ld_flags}\nEXTRA_LDFLAGS += ${_ldflag}")
    endforeach()

    ############################################################################
    # Add compile definitions
    set(_compile_definitions "")
    foreach (_def IN LISTS ${TARGET_NAME}_COMPILE_DEFINITIONS)
        string(REPLACE "\"" "\\\"" _def ${_def})
        if (_compile_definitions)
            set(_compile_definitions "${_compile_definitions}\nEXTRA_CFLAGS += -D${_def}")
        else()
            set(_compile_definitions "EXTRA_CFLAGS += -D${_def}")
        endif()
    endforeach()

    set(_src_files_list "")
    set(_subdirs_list "")
    set(_module_deps "")
    foreach (_src IN LISTS ${TARGET_NAME}_SOURCES)
        # Fill list of source files.
        if (_src_files_list)
            set(_src_files_list "${_src_files_list}\\\n\t${_src}")
        else()
            set(_src_files_list "${_src}")
        endif()

        # Copying source files to the source directory in the build directory.
        get_filename_component(_dest_src_dir "${_module_src_root_dir}/${_src}" DIRECTORY)
        file(MAKE_DIRECTORY "${_dest_src_dir}")
        add_custom_command(
            OUTPUT ${_src}_copy_file
            COMMAND cp -r "${CMAKE_CURRENT_SOURCE_DIR}/${_src}" "${_module_src_root_dir}/${_src}"
            VERBATIM
        )
        # Cmake swears at the '/' character in the target name, so this character
        # is replaced with the '_' character.
        if (_module_deps)
            set(_module_deps "${_module_deps};${_src}_copy_file")
        else()
            set(_module_deps "${_src}_copy_file")
        endif()

        get_filename_component(_src_dir ${_src} DIRECTORY)
        if (_src_dir)
            if ("${_src_dir}" IN_LIST _tmp_list)
                continue()
            else()
                set(_tmp_list ${_tmp_list} ${_src_dir})
            endif()

            if (_subdirs_list)
                set(_subdirs_list "${_subdirs_list}\n\tmkdir -p \"${_module_build_dir}/${_src_dir}\"")
            else()
                set(_subdirs_list "mkdir -p \"${_module_build_dir}/${_src_dir}\"")
            endif()
        endif()
    endforeach()

    ############################################################################
    # Configure Makefile for module
    set(MODULE_NAME             "${_module_name}")
    set(MODULE_SRC_ROOT_DIR     "${_module_src_root_dir}")
    set(MODULE_BUILD_DIR        "${_module_build_dir}")
    set(MODULE_INCLUDE_DIRS     "${_module_include_dirs}")
    set(MODULE_EXTRA_CFLAGS     "${_extra_flags}")
    set(MODULE_EXTRA_LDFLAGS    "${_extra_ld_flags}")
    set(MODULE_COMPILE_DIFS     "${_compile_definitions}")
    set(MODULE_SOURCE_FILES     "${_src_files_list}")
    set(MODULE_MAKE_SUB_DIRS    "${_subdirs_list}")
    set(_makefile_src "${CMAKE_SOURCE_DIR}/${COMMON_CMAKE_DIR}/Makefile_driver.in")
    set(_makefile_dest "${_module_src_root_dir}/Makefile")
    configure_file("${_makefile_src}" "${_makefile_dest}")

    ############################################################################
    # Create module build target
    file(MAKE_DIRECTORY ${CMAKE_DRIVER_OUTPUT_DIRECTORY})
    add_custom_command(
        OUTPUT ${_module_target}
        COMMAND cd "${_module_src_root_dir}"
        COMMAND $(MAKE) -f "${_makefile_dest}"
        COMMAND ${CMAKE_COMMAND} -E copy "${_module_build_dir}/${_module_target}" "${CMAKE_DRIVER_OUTPUT_DIRECTORY}/${_module_target}"
        WORKING_DIRECTORY ${_module_src_root_dir}
        DEPENDS ${_module_deps}
        VERBATIM
    )
    add_custom_target(${TARGET_NAME} ALL
                      DEPENDS ${_module_target}
    )
endfunction()

################################################################################
# Targets
################################################################################

function(DriverTarget TARGET_NAME)
    _parse_target_args(${TARGET_NAME}
        _DRIVER_FLAGS_KW _DRIVER_VALUES_KW _DRIVER_LISTS_KW ${ARGN}
    )

    if (UNIX)
        _LinuxDriverTarget(${TARGET_NAME})
    else()
        message(FATAL_ERROR " Unsupported platform")
    endif()
endfunction()
