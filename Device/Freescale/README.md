Port the NXP Kinetis SDK 2.0 to PolyMCU
=======================================

1. Generate and Download the SDK package for the targetted platform/MCU

2. Unpack the SDK into a temporary location `$FREESCALE_KINETIS_ROOT`

3. Copy `$FREESCALE_KINETIS_ROOT/devices` to `$POLYMCU_ROOT/Device/Freescale`  
   Copy `$FREESCALE_KINETIS_ROOT/middleware` to `$POLYMCU_ROOT/Device/Freescale`

Freescale Specific Support
==========================

* `SUPPORT_DEBUG_UART` can be either `uart`, `lpuart`, `lpsci`
* `BOARD_DEBUG_UART_INSTANCE` defines which instance of the Debug UART devices to use.

Reduce Kinetis KL27 Footprint
=============================

You can reduce Kinetis KL27 footprint by defining empty IRQ Handlers for devices you 
are not using.  
By default, Kinetis SDK adds all interrupt driver support that could make the size of
the firmware larger.

For instance:

```
void UART2_FLEXIO_DriverIRQHandler(void) { }
void I2C0_DriverIRQHandler(void) { }
void I2C1_DriverIRQHandler(void) { }
void SPI0_DriverIRQHandler(void) { }
void SPI1_DriverIRQHandler(void) { }
void DMA0_DriverIRQHandler(void) { }
void DMA1_DriverIRQHandler(void) { }
void DMA2_DriverIRQHandler(void) { }
void DMA3_DriverIRQHandler(void) { }
```
