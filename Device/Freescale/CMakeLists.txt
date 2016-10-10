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

# Required by the UART Debug
find_package(Board)
find_package(PolyMCU)
# Required to add the appropriate definitions and include directories
find_package(Freescale)

#
# Freescale MKL25Z4
#
if(MCU_DEVICE STREQUAL "MKL25Z4")
  set(freescale_SRCS ${MCU_DEVICE_ROOT}/system_MKL25Z4.c
                     ${MCU_DEVICE_ROOT}/gcc/startup_MKL25Z4.S
                     ${MCU_DEVICE_ROOT}/drivers/fsl_adc16.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_clock.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_cmp.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_common.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_cop.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_dac.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_dmamux.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flash.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_gpio.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_i2c.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_i2c_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_llwu.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_lpsci.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_lpsci_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_lptmr.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_pit.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_pmc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_rcm.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_rtc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_sim.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_smc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_spi.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_spi_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_tpm.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_tsi_v4.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_uart.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_uart_dma.c)
endif()

#
# Freescale MKL27Z644
#
if(MCU_DEVICE STREQUAL "MKL27Z644")
  set(freescale_SRCS ${MCU_DEVICE_ROOT}/system_MKL27Z644.c
                     ${MCU_DEVICE_ROOT}/gcc/startup_MKL27Z644.S
                     ${MCU_DEVICE_ROOT}/drivers/fsl_adc16.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_clock.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_cmp.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_common.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_cop.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_crc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_dmamux.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flash.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_i2c_master.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_i2s.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_i2s_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_spi.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_spi_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_uart.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_flexio_uart_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_gpio.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_i2c.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_i2c_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_llwu.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_lptmr.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_lpuart.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_lpuart_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_pit.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_pmc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_rcm.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_rtc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_sim.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_smc.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_spi.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_spi_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_tpm.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_uart.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_uart_dma.c
                     ${MCU_DEVICE_ROOT}/drivers/fsl_vref.c)
endif()

#
# Debug UART support
#
if (NOT SUPPORT_DEBUG_UART STREQUAL "none")
  if(SUPPORT_DEBUG_UART STREQUAL "uart")
    add_definitions(-DFREESCALE_DEBUG_UART)
    list(APPEND freescale_SRCS Driver/uart_debug/Driver_USART.c)
  elseif(SUPPORT_DEBUG_UART STREQUAL "lpuart")
    add_definitions(-DFREESCALE_DEBUG_LPUART)
    list(APPEND freescale_SRCS Driver/uart_debug/Driver_USART.c)
  elseif(SUPPORT_DEBUG_UART STREQUAL "lpsci")
    add_definitions(-DFREESCALE_DEBUG_LPSCI)
    list(APPEND freescale_SRCS Driver/uart_debug/Driver_USART.c)
  endif()

  if(DEBUG_UART_BAUDRATE)
    add_definitions(-DDEBUG_UART_BAUDRATE=${DEBUG_UART_BAUDRATE})
  endif()
endif()

#
# USB Support
#
if (SUPPORT_DEVICE_USB)
  list(APPEND freescale_SRCS middleware/usb_1.1.0/device/usb_device_dci.c
                             middleware/usb_1.1.0/device/usb_device_khci.c)

  if(SUPPORT_RTOS)
    message(FATAL_ERROR "Require to port OSA to CMSIS RTOS.")
    list(APPEND freescale_SRCS middleware/usb_1.1.0/osa/usb_osa_cmsis.c)
  else()
    list(APPEND freescale_SRCS middleware/usb_1.1.0/osa/usb_osa_bm.c)
  endif()
endif()

add_library(device_freescale STATIC ${freescale_SRCS})
