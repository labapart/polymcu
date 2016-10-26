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

find_package(CMSIS)

# MCU specific paths
if (BOARD STREQUAL "ST/STM32L4xx_Nucleo")
  set(MCU_ROOT ${CMAKE_CURRENT_LIST_DIR}/STM32L4xx)
  set(MCU_HAL_ROOT ${CMAKE_CURRENT_LIST_DIR}/STM32L4xx_HAL_Driver)
else()
  set(MCU_ROOT ${CMAKE_CURRENT_LIST_DIR}/STM32F7xx)
  set(MCU_HAL_ROOT ${CMAKE_CURRENT_LIST_DIR}/STM32F7xx_HAL_Driver)
endif()

include_directories(${MCU_ROOT}/Include ${MCU_HAL_ROOT}/Inc)

# MCU specific definitions
if (BOARD STREQUAL "ST/STM32L4xx_Nucleo")
  add_definitions(-DSTM32L476xx)
  set(MCU_EXE_LINKER_FLAGS "-T ${MCU_ROOT}/Linker/STM32L476RGTx_FLASH.ld")
else()
  add_definitions(-DSTM32F767xx)
  set(MCU_EXE_LINKER_FLAGS "-T ${MCU_ROOT}/Linker/STM32F767ZITx_FLASH.ld")
endif()

set(ST_LIBRARIES device_st)
