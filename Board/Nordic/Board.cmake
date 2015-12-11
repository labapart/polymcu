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

# Identify Nordic Board
string(REGEX REPLACE "Nordic/" "" NORDIC_BOARD ${BOARD})

if (NORDIC_BOARD STREQUAL "nRF52DK")
  # Tell RTOS we are running at 64Mhz
  set(RTOS_CLOCK 64000000)

  # Build options
  set(CPU "ARM Cortex-M4F")
  add_definitions(-DNRF52)
  set(NORDIC_SDK nRF52_SDK)
elseif ((NORDIC_BOARD STREQUAL "nRF51DK") OR (NORDIC_BOARD STREQUAL "nRF51Dongle"))
  # Tell RTOS we are running at 32Mhz
  set(RTOS_CLOCK 32000000)

  # Build options
  set(CPU "ARM Cortex-M0")
  add_definitions(-DNRF51)
  set(NORDIC_SDK nRF51_SDK)
elseif (NORDIC_BOARD STREQUAL "nRF51822")
  # Tell RTOS we are running at 16Mhz
  set(RTOS_CLOCK 16000000)

  # Build options
  set(CPU "ARM Cortex-M0")
  add_definitions(-DNRF51)
  set(NORDIC_SDK nRF51_SDK)
else()
  message(FATAL_ERROR "Nordic board '${NORDIC_BOARD}' non supported.")
endif()


# List of HW modules
list(APPEND LIST_MODULES Device/Nordic
                         Board/Nordic
                         Lib/PolyMCU)

# nRF52 has not got Systick
if(SUPPORT_RTOS STREQUAL "RTX")
  add_definitions(-DOS_SYSTICK=0 -DRTC1_ENABLED=1)

  # RTX Timer tick interval value [us]
  set(RTOS_TICK 1000)

  # We use RTC1 for the systick  (1ms counter period - ie. 1000Hz)
  set(RTOS_CLOCK 1000)
endif()

set(POST_BUILD_COMMANDS ${CMAKE_OBJCOPY} "-O ihex $<TARGET_FILE:Firmware> $<TARGET_PROPERTY:Firmware,OUTPUT_NAME>.hex")
