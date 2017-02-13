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

# Debug support
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_NRF -DDEBUG_NRF_USER -DNRF_LOG_USES_UART=1")

set(NORDIC_SDK_ROOT ${CMAKE_CURRENT_LIST_DIR}/nRF5_SDK/components)

if (SUPPORT_BLE_PERIPHERAL OR SUPPORT_BLE_CENTRAL)
  # Enable Nordic SoftDevice
  set(NORDIC_SOFT_DEVICE_VERSION s132)
endif()

set(NORDIC_SOFT_DEVICE_ROOT ${NORDIC_SDK_ROOT}/softdevice/${NORDIC_SOFT_DEVICE_VERSION})
if (NORDIC_SOFT_DEVICE_VERSION)
  add_definitions(-DSOFTDEVICE_PRESENT -DS132)
endif()

if (NORDIC_NRF52)
  # MCU definitions
  add_definitions(-DNRF52)
  
  if (NORDIC_SOFT_DEVICE_VERSION)
    set(LINKER_SCRIPT ${CMAKE_BINARY_DIR}/armgcc_s132_nrf52832_xxaa.ld)
    set(Board_INSTALL_SCRIPT_ARG "${NORDIC_SOFT_DEVICE_ROOT}/hex/s132_nrf52_2.0.0_softdevice.hex")
  else()
    if(BOARD_PCA10036)
      set(LINKER_SCRIPT ${NORDIC_SDK_ROOT}/toolchain/gcc/nrf52_xxaa_pca10036.ld)
    else()
      set(LINKER_SCRIPT ${NORDIC_SDK_ROOT}/toolchain/gcc/nrf52_xxaa.ld)
    endif()
  endif()
endif()

find_package(CMSIS)

# MCU specific definitions
set(MCU_EXE_LINKER_FLAGS "-L${NORDIC_SDK_ROOT}/toolchain/gcc -T \"${LINKER_SCRIPT}\"")
add_definitions(-DCONFIG_GPIO_AS_PINRESET -DGPIOTE_ENABLED=1 -DCLOCK_ENABLED=1 -DTIMER0_ENABLED=1)

include_directories(${NORDIC_SDK_ROOT}/ble/common
					${NORDIC_SDK_ROOT}/ble/ble_advertising
					${NORDIC_SDK_ROOT}/ble/ble_db_discovery
					${NORDIC_SDK_ROOT}/ble/device_manager
					${NORDIC_SDK_ROOT}/ble/device_manager/config
					${NORDIC_SDK_ROOT}/device
					${NORDIC_SDK_ROOT}/drivers_nrf/clock
                    ${NORDIC_SDK_ROOT}/drivers_nrf/common
                    ${NORDIC_SDK_ROOT}/drivers_nrf/config
					${NORDIC_SDK_ROOT}/drivers_nrf/delay
                    ${NORDIC_SDK_ROOT}/drivers_nrf/hal
					${NORDIC_SDK_ROOT}/drivers_nrf/pstorage
					${NORDIC_SDK_ROOT}/drivers_nrf/pstorage/config
					${NORDIC_SDK_ROOT}/drivers_nrf/rtc
					${NORDIC_SDK_ROOT}/drivers_nrf/timer
					${NORDIC_SDK_ROOT}/libraries/button
					${NORDIC_SDK_ROOT}/libraries/fifo
					${NORDIC_SDK_ROOT}/libraries/scheduler
					${NORDIC_SDK_ROOT}/libraries/sensorsim
					${NORDIC_SDK_ROOT}/libraries/timer
					${NORDIC_SDK_ROOT}/libraries/trace
					${NORDIC_SDK_ROOT}/libraries/uart
					${NORDIC_SDK_ROOT}/libraries/util
					${NORDIC_SDK_ROOT}/libraries/experimental_section_vars
                    ${NORDIC_SDK_ROOT}/toolchain)

if (SUPPORT_BLE_PERIPHERAL)
  include_directories(${NORDIC_SDK_ROOT}/ble/ble_services/ble_bas
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bps
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_cscs
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_dfu
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_dis
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_gls
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hids
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hrs
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hts
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ias
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_lbs
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_lls
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_nus
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_rscs
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_tps)
endif()

if (SUPPORT_BLE_CENTRAL)
  include_directories(${NORDIC_SDK_ROOT}/ble/ble_services/ble_ancs_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ans_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_bas_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_cts_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_hrs_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_ias_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_lbs_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_nus_c
                      ${NORDIC_SDK_ROOT}/ble/ble_services/ble_rscs_c)
endif()

if (NORDIC_SOFT_DEVICE_VERSION)
  include_directories(${NORDIC_SDK_ROOT}/libraries/fstorage
                      ${NORDIC_SDK_ROOT}/softdevice/common/softdevice_handler
                      ${NORDIC_SOFT_DEVICE_ROOT}/headers
                      ${NORDIC_SOFT_DEVICE_ROOT}/headers/nrf52)
else()
  include_directories(${NORDIC_SDK_ROOT}/drivers_nrf/nrf_soc_nosd)
endif()

set(NORDIC_LIBRARIES device_nordic)
