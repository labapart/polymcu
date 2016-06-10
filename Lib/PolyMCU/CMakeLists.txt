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

find_package(Board)
find_package(PolyMCU)
if (SUPPORT_RTOS)
  find_package(RTOS)
endif()

set(polymcu_SRCS misc.c mailbox.c)

# UART Support
if(SUPPORT_DEBUG_UART STREQUAL "none")
  list(APPEND polymcu_SRCS uart_none.c)
elseif(SUPPORT_DEBUG_UART STREQUAL "itm")
  list(APPEND polymcu_SRCS uart_itm.c)
else()
  list(APPEND polymcu_SRCS uart_device.c)
endif()

if(SUPPORT_TIMER)
  list(APPEND polymcu_SRCS timer_systick.c)

  set(TIMER_TASK_MAX 5 CACHE STRING "Maximum number of Timer tasks.")
  add_definitions(-DTIMER_TASK_MAX=${TIMER_TASK_MAX})
endif()

#
# Macro to get the length of the USB String
#
MACRO(COUNT_USB_STRING string length)
  string(REPLACE "," ";" usb_string ${string})
  list(LENGTH usb_string usb_string_length)
  set(${length} ${usb_string_length})
ENDMACRO()

# Generate the USB Device constants
if(SUPPORT_DEVICE_USB)
  if(DEVICE_USB_DEVICE_MANUFACTURER)
    COUNT_USB_STRING(${DEVICE_USB_DEVICE_MANUFACTURER} DEVICE_USB_DEVICE_MANUFACTURER_SIZE)
  endif()
  if(DEVICE_USB_DEVICE_PRODUCT)
    COUNT_USB_STRING(${DEVICE_USB_DEVICE_PRODUCT}      DEVICE_USB_DEVICE_PRODUCT_SIZE)
  endif()
  if(DEVICE_USB_DEVICE_SERIAL)
    COUNT_USB_STRING(${DEVICE_USB_DEVICE_SERIAL}       DEVICE_USB_DEVICE_SERIAL_SIZE)
  endif()

  if(NOT DEVICE_USB_HID_FEATURE_REPORT_SIZE)
    set(DEVICE_USB_HID_FEATURE_REPORT_SIZE ${DEVICE_USB_HID_INPUT_REPORT_SIZE})
  endif()

  # Generate Configuration header file
  configure_file(usb_device_definitions.h.in ${CMAKE_BINARY_DIR}/usb_device_definitions.h)
endif()

add_library(polymcu STATIC ${polymcu_SRCS})
