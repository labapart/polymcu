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
find_package(PolyMCU)

if (BOARD STREQUAL "ST/STM32L4xx_Nucleo")
  set(st_SRCS ${MCU_ROOT}/Source/gcc/startup_stm32l476xx.s
            ${MCU_ROOT}/Source/system_stm32l4xx.c
            ${MCU_HAL_ROOT}/Src/stm32l4xx_hal.c
            ${MCU_HAL_ROOT}/Src/stm32l4xx_hal_cortex.c
            ${MCU_HAL_ROOT}/Src/stm32l4xx_hal_gpio.c
            ${MCU_HAL_ROOT}/Src/stm32l4xx_hal_pwr_ex.c
            ${MCU_HAL_ROOT}/Src/stm32l4xx_hal_rcc.c
            ${MCU_HAL_ROOT}/Src/stm32l4xx_hal_uart.c)
else()
  set(st_SRCS ${MCU_ROOT}/Source/gcc/startup_stm32f767xx.s
      ${MCU_ROOT}/Source/system_stm32f7xx.c
      ${MCU_HAL_ROOT}/Src/stm32f7xx_hal.c
      ${MCU_HAL_ROOT}/Src/stm32f7xx_hal_cortex.c
      ${MCU_HAL_ROOT}/Src/stm32f7xx_hal_gpio.c
      ${MCU_HAL_ROOT}/Src/stm32f7xx_hal_pwr_ex.c
      ${MCU_HAL_ROOT}/Src/stm32f7xx_hal_rcc.c
      ${MCU_HAL_ROOT}/Src/stm32f7xx_hal_uart.c)
endif()

list(APPEND st_SRCS Driver/VCom/Driver_USART.c)

# Define 'device_st' as the static library for Device/ST
add_library(device_st STATIC ${st_SRCS})
