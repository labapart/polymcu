#!/bin/bash

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

if [ "$#" -ne 1 ]; then
	echo "./install_binary_to_board.sh <image.bin>"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "Binary file '$1' does not exist."
	exit 1
fi

# Get the serial number for the Vendor ID 0x0483
export USB_SERIAL=`lsusb -v -d 0483: | grep iSerial | awk '{print $3}'`

# Find the associated USB disk
export USB_DISK=`ls /dev/disk/by-id/usb-* | grep ${USB_SERIAL}`

# Retrieve the device node
export USB_DEV=`readlink -e ${USB_DISK}`

# Get the USB Media (note USB Media can have spaces in its name)
export USB_MEDIA=`mount | grep ${USB_DEV} | cut -d ' ' -f 3- | sed 's/ type .*//'`

echo "Copy $1 to ${USB_MEDIA}"
cp "$1" ${USB_MEDIA}
