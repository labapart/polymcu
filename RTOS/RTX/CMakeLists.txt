#
# Copyright (c) 2015-2016, Lab A Part
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

find_package(CMSIS)
find_package(RTOS)

include_directories(SRC)

if(SUPPORT_RTOS_NO_CMSIS)
  message(FATAL_ERROR "SUPPORT_RTOS_NO_CMSIS is not supported yet.")
endif()

set(RTOS_STACK_WATERMARK TRUE CACHE BOOL "Enable RTOS Stack Watermark")

# Generate Configuration header file
configure_file(Templates/platform_cmsis.h.in ${CMAKE_CURRENT_BINARY_DIR}/platform_cmsis.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(rtos_SRSC SRC/HAL_CM.c
              SRC/rt_CMSIS.c
              SRC/rt_Event.c
              SRC/rt_List.c
              SRC/rt_Mailbox.c
              SRC/rt_MemBox.c
              SRC/rt_Memory.c
              SRC/rt_Mutex.c
              SRC/rt_Robin.c
              SRC/rt_Semaphore.c
              SRC/rt_System.c
              SRC/rt_Time.c
              SRC/rt_Task.c
              Templates/RTX_Conf_CM.c)

list(APPEND rtos_SRSC SRC/GCC/SVC_Table.S)

if(CPU STREQUAL "ARM Cortex-M0")
  list(APPEND rtos_SRSC SRC/GCC/HAL_CM0.S)
elseif(CPU STREQUAL "ARM Cortex-M0plus")
  list(APPEND rtos_SRSC SRC/GCC/HAL_CM0.S)
elseif(CPU STREQUAL "ARM Cortex-M3")
  list(APPEND rtos_SRSC SRC/GCC/HAL_CM3.S)
elseif(CPU STREQUAL "ARM Cortex-M4")
  list(APPEND rtos_SRSC SRC/GCC/HAL_CM4.S)
elseif(CPU STREQUAL "ARM Cortex-M4F")
  list(APPEND rtos_SRSC SRC/GCC/HAL_CM4.S)
else()
  message(FATAL_ERROR "CPU must be defined.")
endif()

add_library(cmsis_rtos STATIC ${rtos_SRSC})
