This example application demonstrate PolyMCU Timer API.

You can change the period of the Timer API by building the application with `-DPERIOD_IN_SECOND=N`. 
For instance to have 10 periods every second:

    cmake -DPERIOD_IN_SECOND=10 -DAPPLICATION=Examples/BaremetalTimer -DBOARD=NXP/LPC1768mbed ..
    make install

By default, the example use a period every millisecond.

**Note:** The period resolution is limited to the hardware.
