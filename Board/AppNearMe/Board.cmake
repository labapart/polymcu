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

# Use Serial-over-USB if not specified.
# Note: Use `set(SUPPORT_DEBUG_UART 1)` in your application to use
#       UART controller instead of UART-over-USB
if(NOT DEFINED SUPPORT_DEBUG_UART)
  set(SUPPORT_DEBUG_UART        "usb")
  set(SUPPORT_DEVICE_USB        1)
  set(SUPPORT_DEVICE_USB_SERIAL 1)
endif()

# Tell RTOS we are running at 48Mhz
set(RTOS_CLOCK 48000000)

# List of HW modules
list(APPEND LIST_MODULES Device/NXP
                         Board/AppNearMe
                         Lib/PolyMCU)

#
# Build options
#
set(CPU "ARM Cortex-M0")
set(MCU_DEVICE lpc_chip_11uxx)

#
# USB Settings
#
if(NOT DEFINED DEVICE_USB_DEVICE_MANUFACTURER)
  set(DEVICE_USB_DEVICE_MANUFACTURER 0x1FC9)
endif()
if(NOT DEFINED DEVICE_USB_DEVICE_PRODUCT)
  set(DEVICE_USB_DEVICE_PRODUCT 0x8039)
endif()
if(NOT DEFINED DEVICE_USB_DEVICE_SERIAL)
  set(DEVICE_USB_DEVICE_SERIAL 0x0101)
endif()

set(POST_BUILD_COMMANDS lpcrc $<TARGET_PROPERTY:Firmware,OUTPUT_NAME>.bin)
