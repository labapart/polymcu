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

find_package(NXP)

include_directories(${CMAKE_CURRENT_LIST_DIR}/MicroNFCBoard)

# If USB support
if(SUPPORT_DEVICE_USB)
  if(SUPPORT_DEVICE_USB_SERIAL)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/MicroNFCBoard/vcom)
  endif()

  if(SUPPORT_DEVICE_USB_HID)
    add_definitions(-DSUPPORT_DEBUG_UART_NONE)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/MicroNFCBoard/generic_hid)
  endif()
endif()

set(Board_LIBRARIES board_appnearme ${NXP_LIBRARIES})
set(Board_INSTALL_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/install_binary_to_board.sh)
