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

# Tell RTOS we are running at 96Mhz
set(RTOS_CLOCK 96000000)

#
# USB Support
#
if (SUPPORT_DEVICE_USB)
  # Use the external oscillator to derive it and get a 48Mhz oscillator for the USB PLL
  set(SUPPORT_NXP_USE_XTAL 1)
endif()

# Default USB Settings
set(DEVICE_USB_VENDOR_ID 0x1FC9 CACHE STRING "Set USB Vendor ID")
set(DEVICE_USB_PRODUCT_ID 0x8039 CACHE STRING "Set USB Product ID")
set(DEVICE_USB_DEVICE_REVISION 0x0101 CACHE STRING "Set USB Device Revision")

# List of HW modules
list(APPEND LIST_MODULES Device/NXP
                         Board/NXP
                         Lib/PolyMCU)

#
# Build options
#
set(CPU "ARM Cortex-M3")
set(MCU_DEVICE lpc_chip_175x_6x)

set(POST_BUILD_COMMANDS lpcrc $<TARGET_PROPERTY:Firmware,OUTPUT_NAME>.bin)
