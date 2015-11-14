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

# We disable USB Virtual COM on MicroNFCBoard to avoid to use USB composite
if (BOARD STREQUAL "AppNearMe/MicroNFCBoard")
  set(SUPPORT_DEBUG_UART none)
endif()

# List of modules needed by the application
set(LIST_MODULES CMSIS Lib/PolyMCU)

set(SUPPORT_DEVICE_USB 1)
set(SUPPORT_DEVICE_USB_HID 1)

set(DEVICE_USB_VENDOR_ID       0x123)
set(DEVICE_USB_PRODUCT_ID      0x456)
set(DEVICE_USB_DEVICE_REVISION 0x789)

set(DEVICE_USB_DEVICE_MANUFACTURER "'l', 0, 'a', 0, 'b', 0, 'a', 0, 'p', 0, 'a', 0, 'r', 0, 't', 0")
set(DEVICE_USB_DEVICE_PRODUCT      "'e', 0, 'x', 0, 'a', 0, 'm', 0, 'p', 0, 'l', 0, 'e', 0")
set(DEVICE_USB_DEVICE_SERIAL       "'s', 0, 'e', 0, 'r', 0, 'i', 0, 'a', 0, 'l', 0, '0', 0, '0', 0")

set(DEVICE_USB_HID_INPUT_REPORT_SIZE  4)
set(DEVICE_USB_HID_OUTPUT_REPORT_SIZE 4)
