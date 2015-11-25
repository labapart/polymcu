Conformance test for CMSIS RTOS
===============================

Status
------

Tested with NXP LPC1768 board on Linux.

Build & Install: Linux instructions
-----------------------------------

Set CROSS_COMPILE environment variable.

```
mkdir Build && cd Build
cmake -DBOARD=NXP/LPC1768mbed -DAPPLICATION=LabAPart/CMSIS_RTOS_Conformance
make
make install
```

More information [here](/README.md)

Build & Install: Windows instructions
-------------------------------------

Add CMake, MinGW to your PATH and set CROSS_COMPILE environment variable.

```
mkdir Build
cd Build
cmake -G "MinGW Makefiles" -DAPPLICATION=<application_vendor/application_name> -DBOARD=<board_vendor/board_name> ..
mingw32-make
```

Copy `Application\LabAPart\CMSIS_RTOS_Conformance\CMSIS_RTOS_Conformance.bin` to your development board.

More information [here](/README.md)

Results
-------

Results should be printed on the default board UART.

You should see something like that:

```
(...)
 - Test osMessagePut() (3) : PASS
 - Test osMessagePut() when there is no space : PASS
# Test osMailCreate() / osMailAlloc() / osMailCAlloc() / osMailPut() / osMailGet() / osMailFree()
 - Test osMailAlloc() when out of resource : PASS
 - Test osMailPut() when out of resource : PASS
 - Test osMailFree() with message which has not been removed from the mailbox (status != osOK) : FAIL
 - Test osMailGet() : PASS
 - Test osMailFree() (status == osOK) : FAIL
 - Test osMailFree() with message already freed : PASS
=> Test completed - pass:90 fail:3
```

- Latest [ARM RTX / CMSIS RTOS results](http://labapart.com/products/polymcu/cmsis-rtos-compliance-rtx)
- Latest [FreeRTOS results](http://labapart.com/products/polymcu/cmsis-rtos-compliance-freertos)

FreeRTOS
--------

To run the CMSIS RTOS on FreeRTOS add the arguments `-DSUPPORT_RTOS=FreeRTOS` to your CMake command line.
For instance:

```
cmake -DBOARD=NXP/LPC1768mbed -DAPPLICATION=LabAPart/CMSIS_RTOS_Conformance -DSUPPORT_RTOS=FreeRTOS ..
make
make install
```
