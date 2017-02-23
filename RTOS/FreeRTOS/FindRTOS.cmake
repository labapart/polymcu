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

include_directories(${CMAKE_BINARY_DIR}/RTOS/FreeRTOS)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/cmsis)

if ((CMAKE_C_COMPILER_ID STREQUAL "GNU") OR (CMAKE_C_COMPILER_ID STREQUAL "Clang"))
  if(CPU STREQUAL "ARM Cortex-M0")
    include_directories("${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM0")
  elseif(CPU STREQUAL "ARM Cortex-M0plus")
    include_directories("${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM0")
  elseif(CPU STREQUAL "ARM Cortex-M3")
    include_directories("${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM3")
  elseif(CPU STREQUAL "ARM Cortex-M4F")
    include_directories("${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM4F")
  elseif(CPU STREQUAL "ARM Cortex-M7")
    include_directories("${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM7/r0p1")
  else()
    message(FATAL_ERROR "CPU not supported.")
  endif()
else()
  message(FATAL_ERROR "Toolchain not supported.")
endif()

if(NOT SUPPORT_RTOS_NO_CMSIS)
  add_definitions(-D__CMSIS_RTOS)
endif()

set(RTOS_LIBRARIES freertos)
