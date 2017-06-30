#
# Copyright (c) 2015-2017, Lab A Part
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required(VERSION 2.6)

enable_language(ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Configure the cross toolchain
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  if (DEFINED ENV{CROSS_COMPILE})
    set(CMAKE_C_COMPILER $ENV{CROSS_COMPILE}gcc)
    set(CMAKE_CXX_COMPILER $ENV{CROSS_COMPILE}g++)
    set(CMAKE_ASM_COMPILER $ENV{CROSS_COMPILE}gcc)
    set(CMAKE_OBJCOPY $ENV{CROSS_COMPILE}objcopy)
    set(CMAKE_SIZE $ENV{CROSS_COMPILE}size)
    set(CMAKE_AR $ENV{CROSS_COMPILE}ar)
  else()
    # Test if the cross-compilation toolchain is setup
    find_program(CROSS_COMPILE_GCC arm-none-eabi-gcc)
    if (NOT CROSS_COMPILE_GCC)
      message(FATAL_ERROR "Either add your cross-compilation toolchain to your PATH or define the environment variable CROSS_COMPILE.")
    endif()

    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_SIZE arm-none-eabi-size)
    set(CMAKE_AR arm-none-eabi-ar)
  endif()

  if(WIN32)
    # Redefine the linker command line as some GCC versions do not implement '--out-implib'
    set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
  endif()
elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if (DEFINED ENV{CROSS_COMPILE})
    set(CMAKE_ASM_COMPILER $ENV{CROSS_COMPILE}gcc)
    set(CMAKE_OBJCOPY $ENV{CROSS_COMPILE}objcopy)
    set(CMAKE_SIZE $ENV{CROSS_COMPILE}size)
    # CMake generally calls CMAKE_C_COMPILER to link the executable. Clang invokes itself the linker installed on the host machine
    set(CMAKE_C_LINK_EXECUTABLE "$ENV{CROSS_COMPILE}gcc <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
    set(CMAKE_CXX_LINK_EXECUTABLE "$ENV{CROSS_COMPILE}g++ <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
    set(CROSS_COMPILE_GCC $ENV{CROSS_COMPILE}gcc)
  else()
    # Test if the cross-compilation toolchain is setup
    find_program(CROSS_COMPILE_GCC arm-none-eabi-gcc)
    if (NOT CROSS_COMPILE_GCC)
      message(FATAL_ERROR "Either add your cross-compilation toolchain to your PATH or define the environment variable CROSS_COMPILE.")
    endif()

    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_SIZE arm-none-eabi-size)
    # CMake generally calls CMAKE_C_COMPILER to link the executable. Clang invokes itself the linker installed on the host machine
    set(CMAKE_C_LINK_EXECUTABLE "arm-none-eabi-gcc <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
    set(CMAKE_CXX_LINK_EXECUTABLE "arm-none-eabi-g++ <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
  endif()
else()
  message(FATAL_ERROR "${CMAKE_C_COMPILER_ID} Toolchain not supported")
endif()

if(WIN32)
  # Prevent Windows Standard Libraries to be added
  set(CMAKE_C_STANDARD_LIBRARIES "")
endif()

# Build type: If not defined then we set DEBUG build by default
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#
# Macro to get the list of include path of GCC
#
MACRO(GET_GCC_INCLUDE_PATH is_cxx gcc_path gcc_include_path)
  if (${is_cxx} STREQUAL "TRUE")
    if (WIN32)
      execute_process(COMMAND ${gcc_path} -v -x c++ -E NUL ERROR_VARIABLE _gcc_output OUTPUT_QUIET)
    else()
      execute_process(COMMAND ${gcc_path} -v -x c++ -E - INPUT_FILE /dev/null ERROR_VARIABLE _gcc_output OUTPUT_QUIET)
    endif()
  else()
    if (WIN32)
      execute_process(COMMAND ${gcc_path} -v -x c -E NUL ERROR_VARIABLE _gcc_output OUTPUT_QUIET)
    else()
      execute_process(COMMAND ${gcc_path} -v -x c -E - INPUT_FILE /dev/null ERROR_VARIABLE _gcc_output OUTPUT_QUIET)
    endif()
  endif()

  # Build an array of string from the GCC output
  string(REPLACE "\n" ";" _gcc_output "${_gcc_output}")

  set(_capture_include FALSE)
  set(_include_path "")

  # Go through the lines and capture between '"#include <...> search starts here:"' and 'End of search list.'
  foreach(_line ${_gcc_output})
    if(${_line} STREQUAL "End of search list.")
      set(_capture_include FALSE)
    endif()

    if(_capture_include)
      # Remove the leading and trailing empty characters
      string(REPLACE "\r" "" _line ${_line})
      string(SUBSTRING "${_line}" 1 -1 _line)

      set(_include_path "${_include_path} -I${_line}")
    endif()

    if(${_line} STREQUAL "#include <...> search starts here:")
      set(_capture_include TRUE)
    endif()
  endforeach()
  set(${gcc_include_path} ${_include_path})
ENDMACRO()

#
# Toolchain support
#
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -Wno-unused-but-set-variable -Wno-unused-variable")
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer")
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  # Retrieve the GCC include paths for C and C++
  GET_GCC_INCLUDE_PATH(FALSE ${CROSS_COMPILE_GCC} CROSS_COMPILE_GCC_C_INCLUDE_PATH)
  GET_GCC_INCLUDE_PATH(TRUE ${CROSS_COMPILE_GCC} CROSS_COMPILE_GCC_CXX_INCLUDE_PATH)

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target arm-none-eabi -std=gnu99 -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -fshort-enums ${CROSS_COMPILE_GCC_C_INCLUDE_PATH}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target arm-none-eabi -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -fshort-enums ${CROSS_COMPILE_GCC_CXX_INCLUDE_PATH}")
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
  set(EXTERN_C_FLAGS "-target arm-none-eabi")
  set(EXTERN_CXX_FLAGS "-target arm-none-eabi")

  # Prevent the warning related to non supported function attribute - see: https://sourceware.org/ml/newlib/2015/msg00714.html
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-attributes")
  set(EXTERN_C_FLAGS "${EXTERN_C_FLAGS} -Wno-unknown-attributes ${CROSS_COMPILE_GCC_INCLUDE_PATH}")
else()
  message(FATAL_ERROR "${CMAKE_C_COMPILER_ID} Toolchain not supported")
endif()

#
# Project Configuration Management
#
# Macro to return all the entries by vendor for a specific directory
MACRO(GET_ENTRIES_BY_VENDOR base_dir list)
  FILE(GLOB _vendors RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir} ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir}/*)
  SET(${list} "")
  FOREACH(_vendor ${_vendors})
    FILE(GLOB _entries RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir} ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir}/${_vendor}/*)
    FOREACH(_entry ${_entries})
      IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir}/${_entry})
        LIST(APPEND ${list} ${_entry})
      ENDIF()
    ENDFOREACH()
  ENDFOREACH()
ENDMACRO()

# Macro to return all the available applications
MACRO(GET_APPLICATION_LIST applist)
  GET_ENTRIES_BY_VENDOR(Application ${applist})
ENDMACRO()

# Macro to return all the available boards
MACRO(GET_BOARD_LIST boardlist)
  GET_ENTRIES_BY_VENDOR(Board ${boardlist})
ENDMACRO()

# Check if the given APPLICATION is valid
GET_APPLICATION_LIST(APPLICATION_LIST)
if (NOT APPLICATION)
  message(FATAL_ERROR "Define APPLICATION variable with one of these available applications: ${APPLICATION_LIST}")
else()
  # Check if the application is defined outside of the PolyMCU tree
  if (IS_ABSOLUTE "${APPLICATION}")
    if (NOT EXISTS ${APPLICATION})
      message(FATAL_ERROR "The external application '${APPLICATION}' does not exist.")
    endif()
  else()
    list (FIND APPLICATION_LIST ${APPLICATION} _index)
    if (NOT ${_index} GREATER -1)
      message(FATAL_ERROR "Application '${APPLICATION}' is not supported. Here is the list of available applications: ${APPLICATION_LIST}")
    endif()
  endif()
endif()

#
# Helper macro to test if the board is supported
#
MACRO(TEST_SUPPORTED_BOARD BoardList)
  if (BOARD)
    if (NOT ${BOARD} MATCHES ${BoardList})
      message(FATAL_ERROR "Board '${BOARD}' not supported for this application. This application only supports '${BoardList}'.")
    endif()
  else(BOARD)
    set(BOARD ${BoardList})
  endif(BOARD)
ENDMACRO(TEST_SUPPORTED_BOARD)

#
# Helper macro to build firmware application
#
MACRO(BUILD_FIRMWARE Target Name SourceList LibraryList)
  add_executable(${Target} ${SourceList})
  set_target_properties(${Target} PROPERTIES
    OUTPUT_NAME ${Name}
    SUFFIX .elf)

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CPU_FLAGS} ${MCU_EXE_LINKER_FLAGS}")

  if(CMAKE_COMPILER_IS_GNUCC OR (CMAKE_C_COMPILER_ID STREQUAL "Clang"))
    # We need '-Wl,--whole-archive' to ensure the symbols from the static libraries are correctly seen
    # by the linker to override the 'weak' symbols
    target_link_libraries(${Target} -Wl,--whole-archive ${LibraryList} -Wl,--no-whole-archive)
  else()
    target_link_libraries(${Target} ${LibraryList})
  endif()

  add_custom_command(TARGET ${Target} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${Target}> ${Name}.bin
  	COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${Target}>)

  if(POST_BUILD_COMMANDS)
    list(LENGTH POST_BUILD_COMMANDS list_length)
    MATH(EXPR list_length "${list_length}-1")
    foreach(list_index RANGE 0 ${list_length} 2)
      list(GET POST_BUILD_COMMANDS ${list_index} cmd)
      MATH(EXPR list_index "${list_index}+1")
      list(GET POST_BUILD_COMMANDS ${list_index} arg)
      # CMake adds '\' in front of spaces - this magic command does it...
      SEPARATE_ARGUMENTS(arg)
      add_custom_command(TARGET ${Target} POST_BUILD COMMAND ${cmd} ARGS ${arg})
    endforeach()
  endif()

  # Installation: Copy the image on the device
  if(Board_INSTALL_SCRIPT)
    if (Board_INSTALL_SCRIPT_ARG)
      install(CODE "execute_process(COMMAND ${Board_INSTALL_SCRIPT} \"${CMAKE_CURRENT_BINARY_DIR}/${Name}.bin\" \"${Board_INSTALL_SCRIPT_ARG}\")")
    else()
      install(CODE "execute_process(COMMAND ${Board_INSTALL_SCRIPT} \"${CMAKE_CURRENT_BINARY_DIR}/${Name}.bin\")")
    endif()
  endif(Board_INSTALL_SCRIPT)
ENDMACRO(BUILD_FIRMWARE)

# Include application & middleware configurations
if(IS_ABSOLUTE "${APPLICATION}")
  include(${APPLICATION}/Application.cmake)
else()
  include(Application/${APPLICATION}/Application.cmake)
endif()

# Check if the given board is valid
GET_BOARD_LIST(BOARD_LIST)
if (NOT BOARD)
  message(FATAL_ERROR "Define BOARD variable with one of these available boards: ${BOARD_LIST}")
else()
  if(NOT IS_ABSOLUTE "${BOARD}")
    list (FIND BOARD_LIST ${BOARD} _index)
    if (NOT ${_index} GREATER -1)
      message(FATAL_ERROR "Board '${BOARD}' is not supported. Here is the list of available boards: ${BOARD_LIST}")
    endif()
  endif()
endif()

# Include board & HW configurations
string(REGEX REPLACE "/.*" "" BOARD_VENDOR ${BOARD})
if(IS_ABSOLUTE "${BOARD}")
  include(${BOARD}/Board.cmake)
else()
  include(Board/${BOARD_VENDOR}/Board.cmake)
endif()

# Debug Support
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(DEBUG_MASK 0x0 CACHE STRING "PolyMCU Debug Mask")
else()
  set(DEBUG_MASK 0xF CACHE STRING "PolyMCU Debug Mask")
endif()

# Add RTOS Support
if (SUPPORT_RTOS)
  list(APPEND LIST_MODULES "RTOS/${SUPPORT_RTOS}")
endif()

# Remove duplicate entries
list(REMOVE_DUPLICATES LIST_MODULES)

# Declare module directories to use their potential Find*.cmake files
foreach(_module ${LIST_MODULES})
  if(IS_ABSOLUTE "${_module}")
    # Sanity check
    if(NOT EXISTS ${_module})
      message(FATAL_ERROR "Module '${_module}' does not exist.")
    endif()

    # Declare directory to use its potential Find*.cmake files
    list(APPEND CMAKE_MODULE_PATH ${_module})
  else()
    # Sanity check
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_module})
      message(FATAL_ERROR "Module '${_module}' does not exist.")
    endif()

    # Declare directory to use its potential Find*.cmake files
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${_module})
  endif()
endforeach()

#
# Add CPU support
#
if(CPU STREQUAL "ARM Cortex-M0")
  add_definitions(-D__CORTEX_M0)
  set(CPU_FLAGS "-mcpu=cortex-m0 -mthumb")
elseif(CPU STREQUAL "ARM Cortex-M0plus")
  add_definitions(-D__CORTEX_M0)
  if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # Clang does not support ARM Cortex M0+ at the moment
    set(CPU_FLAGS "-mcpu=cortex-m0 -mthumb")
  else()
    set(CPU_FLAGS "-mcpu=cortex-m0plus -mthumb")
  endif()
elseif(CPU STREQUAL "ARM Cortex-M3")
  add_definitions(-D__CORTEX_M3)
  set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb")
elseif(CPU STREQUAL "ARM Cortex-M4")
  add_definitions(-D__CORTEX_M4)
  set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb")
elseif(CPU STREQUAL "ARM Cortex-M4F")
  add_definitions(-D__CORTEX_M4F -D__FPU_PRESENT)
  set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
elseif(CPU STREQUAL "ARM Cortex-M7")
  add_definitions(-D__CORTEX_M7)
  set(CPU_FLAGS "-mcpu=cortex-m7 -mthumb")
else()
  message(FATAL_ERROR "CPU must be defined.")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPU_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CPU_FLAGS}")

#
# Generate 'polymcu_config.h'
#
file(WRITE "${CMAKE_BINARY_DIR}/polymcu_config.h" "#ifndef __POLYMCU_CONFIG_H__\n#define __POLYMCU_CONFIG_H__\n\n")
# Get list of Variables
get_cmake_property(_variableNames VARIABLES)
# Get list of Cached Variables
get_cmake_property(_cacheVariableNames CACHE_VARIABLES)
set(_listVariableName ${_variableNames} ${_cacheVariableNames})
list(REMOVE_DUPLICATES _listVariableName)
foreach (_variableName ${_listVariableName})
  # Get the size of the variable
  string(LENGTH ${_variableName} _variableNameLength)

  # Remove variable name that starts with 'CMAKE_' and '_' and are only one character long
  if ((NOT (_variableName MATCHES "^CMAKE_")) AND (NOT (_variableName MATCHES "^_")) AND (NOT _variableNameLength EQUAL 1))
    # Workaround to remove variables 'val' that are defined by CMake
    set(_ignoredVariableNames "val;type;f;l")
    list(FIND _ignoredVariableNames "${_variableName}" _foundVariableName)
    if (${_foundVariableName} EQUAL -1)
      # Try to get type of the variable
      get_property(_typeVariableName CACHE ${_variableName} PROPERTY TYPE)
      if ("${_typeVariableName}" STREQUAL "BOOL")
        file(APPEND "${CMAKE_BINARY_DIR}/polymcu_config.h" "#define ${_variableName} ${${_variableName}}\n")
      elseif ((${_variableName} MATCHES "^[0-9]+$") OR (${_variableName} MATCHES "^0x"))
        file(APPEND "${CMAKE_BINARY_DIR}/polymcu_config.h" "#define ${_variableName} ${${_variableName}}\n")
      else()
        file(APPEND "${CMAKE_BINARY_DIR}/polymcu_config.h" "#define ${_variableName} \"${${_variableName}}\"\n")
      endif()
    endif()
  endif()
endforeach()
file(APPEND "${CMAKE_BINARY_DIR}/polymcu_config.h" "\n#endif\n")
# Add '${CMAKE_BINARY_DIR}' to include "polymcu_config.h"
include_directories(${CMAKE_BINARY_DIR})

#
# Build the different modules
#
foreach(_module ${LIST_MODULES})
  if(IS_ABSOLUTE "${_module}")
    get_filename_component(module_name "${_module}" NAME)
    add_subdirectory(${_module} ${PROJECT_BINARY_DIR}/${module_name})
  else()
    add_subdirectory(${_module})
  endif()
endforeach()

# And finally add the application itself
if(IS_ABSOLUTE "${APPLICATION}")
  get_filename_component(APPLICATION_NAME "${APPLICATION}" NAME)
  add_subdirectory(${APPLICATION} ${PROJECT_BINARY_DIR}/Application/${APPLICATION_NAME})
else()
  add_subdirectory(Application/${APPLICATION})
endif()

# Add Test support if the Test Framework is present
include(Test/Tests.cmake OPTIONAL)
