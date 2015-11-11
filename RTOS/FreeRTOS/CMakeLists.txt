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

find_package(Board)
find_package(RTOS)

# Generate Configuration header file
configure_file(include/FreeRTOSConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOSConfig.h)

set(FreeRTOS_SRCS list.c queue.c tasks.c timers.c hook.c)

list(APPEND FreeRTOS_SRCS "${CMAKE_CURRENT_LIST_DIR}/portable/MemMang/heap_4.c")

if ((CMAKE_C_COMPILER_ID STREQUAL "GNU") OR (CMAKE_C_COMPILER_ID STREQUAL "Clang"))
  if(CPU STREQUAL "ARM Cortex-M0")
    list(APPEND FreeRTOS_SRCS "${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM0/port.c")
  elseif(CPU STREQUAL "ARM Cortex-M0plus")
    list(APPEND FreeRTOS_SRCS "${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM0/port.c")
  elseif(CPU STREQUAL "ARM Cortex-M3")
    list(APPEND FreeRTOS_SRCS "${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM3/port.c")
  elseif(CPU STREQUAL "ARM Cortex-M4F")
    list(APPEND FreeRTOS_SRCS "${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM4F/port.c")
  elseif(CPU STREQUAL "ARM Cortex-M7")
    list(APPEND FreeRTOS_SRCS "${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM7/r0p1/port.c")
  else()
    message(FATAL_ERROR "CPU not supported.")
  endif()
else()
  message(FATAL_ERROR "Toolchain not supported.")
endif()

if (NOT DEFINED SUPPORT_RTOS_NO_CMSIS)
  list(APPEND FreeRTOS_SRCS cmsis/cmsis.c)
endif()

add_library(freertos STATIC ${FreeRTOS_SRCS})
