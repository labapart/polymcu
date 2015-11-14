#!/usr/bin/python

import hid, sys, time

START = 1
STOP  = 2
RESET = 3

# USB Device HID
VENDOR_ID = 0x0123
PRODUCT_ID = 0x0456

devices = hid.enumerate(VENDOR_ID, PRODUCT_ID)
if len(devices) != 1:
    sys.exit(1)

h = hid.device()
h.open(VENDOR_ID, PRODUCT_ID)

print "Manufacturer: %s" % h.get_manufacturer_string()
print "Product: %s" % h.get_product_string()
print "Serial Number: %s" % h.get_serial_number_string()

# Write 32-bit command
h.write([START, 0, 0, 0])

# We should get some data every second
for k in range(5):
    # Read 32-bit
    d = h.read(4, 2000)
    if d:
        print d

# Write 32-bit command
h.write([STOP, 0, 0, 0])

# We should get some data every second
for k in range(5):
    # Read 32-bit
    d = h.read(4, 1000)
    if d:
        print d
    else:
        print "Time out"

# Write 32-bit command
h.write([RESET, 0, 0, 0])

# We should get some data every second
for k in range(5):
    # Read 32-bit
    d = h.read(4, 2000)
    if d:
        print d

# Write 32-bit command
h.write([STOP, 0, 0, 0])
time.sleep(5)


# try writing some data to the device
#for k in range(10):
#    # Read 32-bit
#    d = h.read(4)
#    if d:
#        print d
#    time.sleep(1)

h.close()
