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

cmake_minimum_required(VERSION 2.8)

find_package(Board)
find_package(CMSIS)
find_package(PolyMCU)
find_package(Nordic)

# Debug support
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_NRF -DDEBUG_NRF_USER")

include_directories(${NORDIC_SDK_ROOT}
					${NORDIC_SDK_ROOT}/drivers_nrf/gpiote
                    ${NORDIC_SDK_ROOT}/drivers_nrf/hal
                    ${NORDIC_SDK_ROOT}/drivers_nrf/uart
                    ${NORDIC_SDK_ROOT}/libraries/util)

add_definitions(-DCONFIG_GPIO_AS_PINRESET -DGPIOTE_ENABLED=1)
set(nordic_SRCS ${NORDIC_SDK_ROOT}/toolchain/gcc/gcc_startup_nrf52.s
				${NORDIC_SDK_ROOT}/toolchain/system_nrf52.c
				${NORDIC_SDK_ROOT}/drivers_nrf/common/nrf_drv_common.c
				${NORDIC_SDK_ROOT}/drivers_nrf/clock/nrf_drv_clock.c
				${NORDIC_SDK_ROOT}/drivers_nrf/rtc/nrf_drv_rtc.c
				${NORDIC_SDK_ROOT}/drivers_nrf/gpiote/nrf_drv_gpiote.c
				${NORDIC_SDK_ROOT}/libraries/button/app_button.c
				${NORDIC_SDK_ROOT}/libraries/fifo/app_fifo.c
				${NORDIC_SDK_ROOT}/libraries/sensorsim/sensorsim.c
				${NORDIC_SDK_ROOT}/libraries/util/app_util_platform.c)

# Timer support
if(SUPPORT_RTOS STREQUAL "RTX")
  add_definitions(-DRTC1_CONFIG_FREQUENCY=${RTOS_CLOCK})

  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/libraries/timer/app_timer_rtx.c)
else()
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/libraries/timer/app_timer.c)
endif()

# 'pstorage' depends on SoftDevice API
if (NORDIC_SOFT_DEVICE_VERSION)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/drivers_nrf/pstorage/pstorage.c)
endif()

#
# Nordic SoftDevice Support
#
if (NORDIC_SOFT_DEVICE_VERSION)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/softdevice/common/softdevice_handler/softdevice_handler.c)
endif()

# BLE support
if (SUPPORT_BLE_PERIPHERAL OR SUPPORT_BLE_CENTRAL)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/ble/common/ble_advdata.c
	  					  ${NORDIC_SDK_ROOT}/ble/common/ble_conn_params.c
	  					  ${NORDIC_SDK_ROOT}/ble/common/ble_srv_common.c)

  if (SUPPORT_BLE_PERIPHERAL)
    list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/ble/ble_advertising/ble_advertising.c
	  					    ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bas/ble_bas.c
	  					    ${NORDIC_SDK_ROOT}/ble/ble_services/ble_dis/ble_dis.c
	  					    ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hrs/ble_hrs.c
	  					    ${NORDIC_SDK_ROOT}/ble/device_manager/device_manager_peripheral.c)
  endif()

  if (SUPPORT_BLE_CENTRAL)
    list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/ble/ble_db_discovery/ble_db_discovery.c
	  					    ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bas_c/ble_bas_c.c
	  					    ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hrs_c/ble_hrs_c.c
	  					    ${NORDIC_SDK_ROOT}/ble/device_manager/device_manager_central.c)
  endif()
endif()

#
# Debug UART support
#
add_definitions(-DUART0_ENABLED=1)

# Use UART FIFO by default
if(NOT DEFINED NORDIC_UART_FIFO)
  set(NORDIC_UART_FIFO 1)
endif()

if(NORDIC_UART_FIFO)
  add_definitions(-DNORDIC_UART_FIFO)
  list(APPEND nordic_SRCS Driver/uart_debug/Driver_USART.c
						  ${NORDIC_SDK_ROOT}/drivers_nrf/uart/nrf_drv_uart.c
						  ${NORDIC_SDK_ROOT}/libraries/uart/app_uart_fifo.c)
else()
  list(APPEND nordic_SRCS Driver/uart_debug/Driver_USART.c
						  ${NORDIC_SDK_ROOT}/drivers_nrf/uart/nrf_drv_uart.c
						  ${NORDIC_SDK_ROOT}/libraries/uart/app_uart.c)
endif()

#
# Heap & Stack support
#
if(FIRMWARE_HEAP)
  add_definitions(-D__HEAP_SIZE=${FIRMWARE_HEAP})
endif()
if(FIRMWARE_STACK)
  add_definitions(-D__STACK_SIZE=${FIRMWARE_STACK})
endif()

add_library(device_nordic STATIC ${nordic_SRCS})
