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

set(MCU_DEVICE_ROOT ${CMAKE_CURRENT_LIST_DIR}/devices/${MCU_DEVICE})

include_directories(${MCU_DEVICE_ROOT}
                    ${MCU_DEVICE_ROOT}/drivers
                    ${MCU_DEVICE_ROOT}/utilities)

# Freescale Kinetis SDK depends on CMSIS
find_package(CMSIS)

# This package will generate the library `device_freescale`
set(Freescale_LIBRARIES device_freescale)

#
# Freescale MKL25Z4
#
if(MCU_DEVICE STREQUAL "MKL25Z4")
  if(SUPPORT_RUN_FROM_RAM)
    set(LINKER_SCRIPT ${MCU_DEVICE_ROOT}/gcc/MKL25Z128xxx4_ram.ld)
  else()
    set(LINKER_SCRIPT ${MCU_DEVICE_ROOT}/gcc/MKL25Z128xxx4_flash.ld)
  endif()
endif()

#
# Freescale MKL27Z644
#
if(MCU_DEVICE STREQUAL "MKL27Z644")
  # Check which KL27 is used (64KB or 32KB of flash) by checking the build flags
  get_directory_property(defs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
  if (defs MATCHES "MKL27Z64")
    set(KL27_LINKER_SCRIPT "MKL27Z64xxx4")
  else()
    set(KL27_LINKER_SCRIPT "MKL27Z32xxx4")
  endif()

  if(SUPPORT_RUN_FROM_RAM)
    set(LINKER_SCRIPT ${MCU_DEVICE_ROOT}/gcc/${KL27_LINKER_SCRIPT}_ram.ld)
  else()
    set(LINKER_SCRIPT ${MCU_DEVICE_ROOT}/gcc/${KL27_LINKER_SCRIPT}_flash.ld)
  endif()
endif()

set(MCU_EXE_LINKER_FLAGS "-T \"${LINKER_SCRIPT}\"")

# Tune Stack & Heap to make the application running
if(FIRMWARE_HEAP)
  set(MCU_EXE_LINKER_FLAGS "${MCU_EXE_LINKER_FLAGS} -Xlinker --defsym=__heap_size__=${FIRMWARE_HEAP}")
endif()
if(FIRMWARE_STACK)
  set(MCU_EXE_LINKER_FLAGS "${MCU_EXE_LINKER_FLAGS} -Xlinker --defsym=__stack_size__=${FIRMWARE_STACK}")
endif()

#
# RTOS Support
#
if(SUPPORT_RTOS)
  if ((SUPPORT_RTOS STREQUAL "FreeRTOS") AND (SUPPORT_RTOS_NO_CMSIS))
    add_definitions(-DUSB_STACK_FREERTOS)
  else()
    add_definitions(-DUSB_STACK_CMSIS)
  endif()
endif()

#
# USB Support
#
if (SUPPORT_DEVICE_USB)
  include_directories(${CMAKE_CURRENT_LIST_DIR}/middleware/usb_1.1.0/device
                      ${CMAKE_CURRENT_LIST_DIR}/middleware/usb_1.1.0/include
                      ${CMAKE_CURRENT_LIST_DIR}/middleware/usb_1.1.0/osa)

  if (NOT SUPPORT_RTOS)
    add_definitions(-DUSB_STACK_BM)
  endif()
else()
  # Remove USB memory from the binary
  set(MCU_EXE_LINKER_FLAGS "${MCU_EXE_LINKER_FLAGS} -Xlinker --defsym=__usb_ram_size__=0")
endif()
