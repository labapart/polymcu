#
# Copyright (c) 2015, Lab A Part
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
      execute_process(COMMAND ${gcc_path} -v -x c++ -E NUL ERROR_VARIABLE gcc_output OUTPUT_QUIET)
    else()
      execute_process(COMMAND ${gcc_path} -v -x c++ -E - INPUT_FILE /dev/null ERROR_VARIABLE gcc_output OUTPUT_QUIET)
    endif()
  else()
    if (WIN32)
      execute_process(COMMAND ${gcc_path} -v -x c -E NUL ERROR_VARIABLE gcc_output OUTPUT_QUIET)
    else()
      execute_process(COMMAND ${gcc_path} -v -x c -E - INPUT_FILE /dev/null ERROR_VARIABLE gcc_output OUTPUT_QUIET)
    endif()
  endif()

  # Build an array of string from the GCC output
  string(REPLACE "\n" ";" gcc_output "${gcc_output}")

  set(capture_include FALSE)
  set(include_path "")

  # Go through the lines and capture between '"#include <...> search starts here:"' and 'End of search list.'
  foreach(line ${gcc_output})
    if(${line} STREQUAL "End of search list.")
      set(capture_include FALSE)
    endif()

    if(capture_include)
      # Remove the leading and trailing empty characters
      string(REPLACE "\r" "" line ${line})
      string(SUBSTRING "${line}" 1 -1 line)

      set(include_path "${include_path} -I${line}")
    endif()

    if(${line} STREQUAL "#include <...> search starts here:")
      set(capture_include TRUE)
    endif()
  endforeach()
  set(${gcc_include_path} ${include_path})
ENDMACRO()

#
# Toolchain support
#
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-but-set-variable -Wno-unused-variable")
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
  FILE(GLOB vendors RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir} ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir}/*)
  SET(${list} "")
  FOREACH(vendor ${vendors})
    FILE(GLOB entries RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir} ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir}/${vendor}/*)
    FOREACH(entry ${entries})
      IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${base_dir}/${entry})
        LIST(APPEND ${list} ${entry})
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
  list (FIND APPLICATION_LIST ${APPLICATION} _index)
  if (NOT ${_index} GREATER -1)
    message(FATAL_ERROR "Application '${APPLICATION}' is not supported. Here is the list of available applications: ${APPLICATION_LIST}")
  endif()
endif()

#
# Helper macro to test if the board is supported
#
MACRO(TEST_SUPPORTED_BOARD BoardList)
  if (BOARD)
    if (NOT ${BOARD} STREQUAL ${BoardList})
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
      install(CODE "execute_process(COMMAND ${Board_INSTALL_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR}/${Name}.bin ${Board_INSTALL_SCRIPT_ARG})")
    else()
      install(CODE "execute_process(COMMAND ${Board_INSTALL_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR}/${Name}.bin)")
    endif()
  endif(Board_INSTALL_SCRIPT)
ENDMACRO(BUILD_FIRMWARE)

# Include application & middleware configurations
include(Application/${APPLICATION}/Application.cmake)

# Check if the given board is valid
GET_BOARD_LIST(BOARD_LIST)
if (NOT BOARD)
  message(FATAL_ERROR "Define BOARD variable with one of these available boards: ${BOARD_LIST}")
else()
  list (FIND BOARD_LIST ${BOARD} _index)
  if (NOT ${_index} GREATER -1)
    message(FATAL_ERROR "Board '${BOARD}' is not supported. Here is the list of available boards: ${BOARD_LIST}")
  endif()
endif()

# Include board & HW configurations
string(REGEX REPLACE "/.*" "" BOARD_VENDOR ${BOARD})
include(Board/${BOARD_VENDOR}/Board.cmake)

# Add RTOS Support
if (SUPPORT_RTOS)
  list(APPEND LIST_MODULES "RTOS/${SUPPORT_RTOS}")
  add_definitions(-DSUPPORT_RTOS=${SUPPORT_RTOS})
endif()

# Remove duplicate entries
list(REMOVE_DUPLICATES LIST_MODULES)

# Declare module directories to use their potential Find*.cmake files
foreach(module ${LIST_MODULES})
  # Sanity check
  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${module})
    message(FATAL_ERROR "Module '${module}' does not exist.")
  endif()

  # Declare directory to use its potential Find*.cmake files
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${module})
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

# Build the different modules
foreach(module ${LIST_MODULES})
  add_subdirectory(${module})
endforeach()

# And finally add the application itself
add_subdirectory(Application/${APPLICATION})

# Add Test support if the Test Framework is present
include(Test/Tests.cmake OPTIONAL)
