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

# Tell RTOS we are running at 10Mhz
set(RTOS_CLOCK 10000000)
# No UART on this board template
set(SUPPORT_DEBUG_UART none)

# List of HW modules
list(APPEND LIST_MODULES Device/ARM
                         Board/ARM
                         Lib/PolyMCU)

#
# Build options
#
string(REGEX REPLACE "ARM/" "" _board_cpu ${BOARD})
if (${_board_cpu} STREQUAL "ARMCM0")
  set(CPU "ARM Cortex-M0")
elseif (${_board_cpu} STREQUAL "ARMCM0plus")
  set(CPU "ARM Cortex-M0plus")
elseif (${_board_cpu} STREQUAL "ARMCM3")
  set(CPU "ARM Cortex-M3")
elseif (${_board_cpu} STREQUAL "ARMCM4")
  if(FPU_PRESENT)
    set(CPU "ARM Cortex-M4F")
    add_definitions(-DARMCM4_FP)
  else()
    set(CPU "ARM Cortex-M4")
    add_definitions(-DARMCM4)
  endif()
elseif (${_board_cpu} STREQUAL "ARMCM7")
  set(CPU "ARM Cortex-M7")
  add_definitions(-DARMCM7)
else()
  message(FATAL_ERROR "Board not supported.")
endif()
