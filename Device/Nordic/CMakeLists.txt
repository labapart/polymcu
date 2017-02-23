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

cmake_minimum_required(VERSION 2.8)

find_package(Board)
find_package(CMSIS)
find_package(PolyMCU)
find_package(Nordic)

include_directories(${NORDIC_SDK_ROOT}
					${NORDIC_SDK_ROOT}/drivers_nrf/gpiote
                    ${NORDIC_SDK_ROOT}/drivers_nrf/hal
                    ${NORDIC_SDK_ROOT}/drivers_nrf/uart
                    ${NORDIC_SDK_ROOT}/libraries/fstorage/config
                    ${NORDIC_SDK_ROOT}/libraries/util)

set(nordic_SRCS ${NORDIC_SDK_ROOT}/drivers_nrf/common/nrf_drv_common.c
				${NORDIC_SDK_ROOT}/drivers_nrf/clock/nrf_drv_clock.c
				${NORDIC_SDK_ROOT}/drivers_nrf/delay/nrf_delay.c
				${NORDIC_SDK_ROOT}/drivers_nrf/rtc/nrf_drv_rtc.c
				${NORDIC_SDK_ROOT}/drivers_nrf/gpiote/nrf_drv_gpiote.c
				${NORDIC_SDK_ROOT}/drivers_nrf/timer/nrf_drv_timer.c
				${NORDIC_SDK_ROOT}/libraries/button/app_button.c
				${NORDIC_SDK_ROOT}/libraries/fifo/app_fifo.c
				${NORDIC_SDK_ROOT}/libraries/scheduler/app_scheduler.c
				${NORDIC_SDK_ROOT}/libraries/sensorsim/sensorsim.c
				${NORDIC_SDK_ROOT}/libraries/util/app_util_platform.c
				${NORDIC_SDK_ROOT}/libraries/util/nrf_log.c)

if (SUPPORT_I2C)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/drivers_nrf/twi_master/nrf_drv_twi.c)
  list(APPEND nordic_SRCS Driver/CMSIS_I2C/Driver_I2C.c)
endif()

if (SUPPORT_SPI)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/drivers_nrf/spi_master/nrf_drv_spi.c)
endif()

if (NORDIC_NRF51)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/toolchain/gcc/gcc_startup_nrf51.s
				          ${NORDIC_SDK_ROOT}/toolchain/system_nrf51.c)
elseif (NORDIC_NRF52)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/toolchain/gcc/gcc_startup_nrf52.s
				          ${NORDIC_SDK_ROOT}/toolchain/system_nrf52.c)
endif()

# Timer support
if(SUPPORT_RTOS STREQUAL "RTX")
  add_definitions(-DRTC1_CONFIG_FREQUENCY=${RTOS_CLOCK})

  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/libraries/timer/app_timer_rtx.c
                          ${NORDIC_SDK_ROOT}/libraries/timer/app_timer_appsh.c)
else()
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/libraries/timer/app_timer.c
                          ${NORDIC_SDK_ROOT}/libraries/timer/app_timer_appsh.c)
endif()

# 'pstorage' depends on SoftDevice API
if (NORDIC_SOFT_DEVICE_VERSION)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/drivers_nrf/pstorage/pstorage.c)
endif()

#
# BLE support
#
if (SUPPORT_BLE_PERIPHERAL OR SUPPORT_BLE_CENTRAL)
  list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/libraries/fstorage/fstorage.c
                          ${NORDIC_SDK_ROOT}/softdevice/common/softdevice_handler/softdevice_handler.c
                          ${NORDIC_SDK_ROOT}/softdevice/common/softdevice_handler/softdevice_handler_appsh.c
                          ${NORDIC_SDK_ROOT}/ble/common/ble_advdata.c
                          ${NORDIC_SDK_ROOT}/ble/common/ble_conn_params.c
                          ${NORDIC_SDK_ROOT}/ble/common/ble_srv_common.c)

  #
  # Generate Linker file
  #
  if (NOT DEFINED NORDIC_SOFTDEVICE_RAM_SIZE)
    message(FATAL_ERROR "Set 'NORDIC_SOFTDEVICE_RAM_SIZE' with the size of the memory consumed by SoftDevice.")
  endif()

  if(BOARD_PCA10028)
    configure_file(${NORDIC_SOFT_DEVICE_ROOT}/toolchain/armgcc/${NORDIC_NRF51_SOFTDEVICE_LINKER_SCRIPT}.in ${CMAKE_BINARY_DIR}/${NORDIC_NRF51_SOFTDEVICE_LINKER_SCRIPT})
  elseif(BOARD_PCA10036)
    configure_file(${NORDIC_SOFT_DEVICE_ROOT}/toolchain/armgcc/armgcc_s132_nrf52832_xxaa_pca10036.ld.in ${CMAKE_BINARY_DIR}/armgcc_s132_nrf52832_xxaa.ld)
  elseif(BOARD_PCA10040)
    configure_file(${NORDIC_SOFT_DEVICE_ROOT}/toolchain/armgcc/armgcc_s132_nrf52832_xxaa.ld.in ${CMAKE_BINARY_DIR}/armgcc_s132_nrf52832_xxaa.ld)
  endif()

  if (SUPPORT_BLE_PERIPHERAL)
    include_directories(${NORDIC_SDK_ROOT}/ble/ble_racp)

    list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/ble/ble_advertising/ble_advertising.c
                            ${NORDIC_SDK_ROOT}/ble/ble_racp/ble_racp.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bas/ble_bas.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bps/ble_bps.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_cscs/ble_cscs.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_dfu/ble_dfu.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_dis/ble_dis.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_gls/ble_gls.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hids/ble_hids.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hrs/ble_hrs.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hts/ble_hts.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ias/ble_ias.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_lls/ble_lls.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_nus/ble_nus.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_rscs/ble_rscs.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_tps/ble_tps.c
                            ${NORDIC_SDK_ROOT}/ble/device_manager/device_manager_peripheral.c)
  endif()

  if (SUPPORT_BLE_CENTRAL)
    list(APPEND nordic_SRCS ${NORDIC_SDK_ROOT}/ble/ble_db_discovery/ble_db_discovery.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ancs_c/ble_ancs_c.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ans_c/ble_ans_c.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bas_c/ble_bas_c.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_cts_c/ble_cts_c.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hrs_c/ble_hrs_c.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ias_c/ble_ias_c.c
                            ${NORDIC_SDK_ROOT}/ble/ble_services/ble_rscs_c/ble_rscs_c.c
                            ${NORDIC_SDK_ROOT}/ble/device_manager/device_manager_central.c)
  endif()
endif()

#
# Debug UART support
#
add_definitions(-DUART0_ENABLED=1)

# Use UART FIFO by default
set(NORDIC_UART_FIFO TRUE CACHE BOOL "Use UART FIFO")

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
