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

if(MCU_DEVICE STREQUAL lpc_chip_11uxx)
  add_definitions(-DCORE_M0 -DCHIP_LPC11UXX)

  set(LINKER_SCRIPT lpc11uxx.ld)
elseif(MCU_DEVICE STREQUAL lpc_chip_175x_6x)
  add_definitions(-DCORE_M3 -DCHIP_LPC175X_6X)

  if(SUPPORT_RUN_FROM_RAM)
    set(LINKER_SCRIPT LPC175x_6x_ram.ld)
  else()
    set(LINKER_SCRIPT LPC175x_6x.ld)
  endif()
else()
  message(FATAL_ERROR "MCU_DEVICE must be defined.")
endif()

find_package(CMSIS)

# MCU specific definitions
ADD_DEFINITIONS(-DNO_BOARD_LIB -D__USE_CMSIS)
set(MCU_EXE_LINKER_FLAGS "-L${CMAKE_CURRENT_LIST_DIR}/${MCU_DEVICE}/linker -T \"${CMAKE_CURRENT_LIST_DIR}/${MCU_DEVICE}/linker/${LINKER_SCRIPT}\"")

# Tune Stack & Heap to make the application running
if(NOT FIRMWARE_STACK)
  set(FIRMWARE_STACK 0x280)
endif()
set(MCU_EXE_LINKER_FLAGS "${MCU_EXE_LINKER_FLAGS} -Xlinker --defsym=__StackSize=${FIRMWARE_STACK}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/${MCU_DEVICE}/inc ${CMAKE_CURRENT_LIST_DIR}/Include)
set(NXP_LIBRARIES device_nxp)

#
# USB Support
#
if(SUPPORT_DEVICE_USB)
  if(MCU_DEVICE STREQUAL lpc_chip_175x_6x)
    list(APPEND NXP_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/lpc_chip_175x_6x/libs/libusbd_175x_6x_lib.a)
  endif()
endif()
