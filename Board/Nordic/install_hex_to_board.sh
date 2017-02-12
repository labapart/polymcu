#!/bin/bash

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

if [ "$#" -eq 0 ]; then
	echo "./install_hex_to_board.sh <image.hex> [<softdevice.hex>]"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "Binary file '$1' does not exist."
	exit 1
fi

# In case, we have passed the '.bin' file then try to find '.hex' version
HEX_FILE=`echo "$1" | sed -r 's/\.bin$/.hex/g'`
SOFTDEVICE=$2

echo "Install '$HEX_FILE' and '$SOFTDEVICE'"

# Create script to flash the binaries into the device
TEMP_SCRIPT=`mktemp`
if [ -f "$2" ]; then
echo "device nrf52
speed 1000
w4 4001e504 2
w4 4001e50c 1
sleep 100
r
loadfile $SOFTDEVICE 0x0
loadfile $HEX_FILE 0x1f000
sleep 100
r
g
exit" > $TEMP_SCRIPT
else
echo "device nrf52
speed 1000
w4 4001e504 2
w4 4001e50c 1
sleep 100
r
loadfile $HEX_FILE 0x1f000
sleep 100
r
g
exit" > $TEMP_SCRIPT
fi

if [ -n "$NORDIC_BOARD" ]; then
  JLinkExe -if swd -SelectEmuBySN $NORDIC_BOARD $TEMP_SCRIPT
else
  # Ensure there is only one board connected
  NORDIC_BOARD_COUNT=`lsusb -d 1366: | wc -l`
  if [ $NORDIC_BOARD_COUNT -ne 1 ]; then
    echo "More than one Nordic board are connected."
    echo "Please select the one you want to use by setting NORDIC_BOARD with one of the following serial numbers:"
    lsusb -v -d 1366: | grep iSerial | cut -c 29-
    exit 1
  fi

  JLinkExe -if swd $TEMP_SCRIPT
fi

rm -Rf $TEMP_SCRIPT
