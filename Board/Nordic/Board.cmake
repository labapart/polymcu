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

# List of HW modules
list(APPEND LIST_MODULES Device/Nordic
                         Board/Nordic
                         Lib/PolyMCU)

if (BOARD STREQUAL "Nordic/nRF51DK")
  set(BOARD_PCA10028 1)
  set(NORDIC_NRF51 1)
  set(NORDIC_NRF51_SOFTDEVICE_LINKER_SCRIPT armgcc_s130_nrf51422_xxaa.ld)
  set(NORDIC_SOFT_DEVICE_VERSION s130)

  # Tell RTOS we are running at 64Mhz
  set(RTOS_CLOCK 16000000)

  #
  # Build options
  #
  set(CPU "ARM Cortex-M0")
  add_definitions(-DBOARD_PCA10028)

  # SysTick cannot be used for PolyMCU Timer API - Systick is switched off during WFI/WFE
  set(SUPPORT_TIMER_SYSTICK 0)
elseif (BOARD STREQUAL "Nordic/nRF52DK")
  set(NORDIC_NRF52 1)
  set(NORDIC_SOFT_DEVICE_VERSION s132)

  # Tell RTOS we are running at 64Mhz
  set(RTOS_CLOCK 64000000)

  #
  # Build options
  #
  set(CPU "ARM Cortex-M4F")
  set(BOARD_PCA10040 1)
  add_definitions(-DBOARD_PCA10040 -DNRF52832)

  # SysTick cannot be used for PolyMCU Timer API - Systick is switched off during WFI/WFE
  set(SUPPORT_TIMER_SYSTICK 0)
endif()


# In case we do not use SysTick, use an alternative for RTOS
if (SUPPORT_TIMER_SYSTICK EQUAL 0)
  if(SUPPORT_RTOS STREQUAL "RTX")
    add_definitions(-DOS_SYSTICK=0 -DRTC1_ENABLED=1)

    # RTX Timer tick interval value [us]
    set(RTOS_TICK 1000)

    # We use RTC1 for the systick  (1ms counter period - ie. 1000Hz)
    set(RTOS_CLOCK 1000)
  endif()
endif()

set(POST_BUILD_COMMANDS ${CMAKE_OBJCOPY} "-O ihex $<TARGET_FILE:Firmware> $<TARGET_PROPERTY:Firmware,OUTPUT_NAME>.hex")
