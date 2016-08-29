Port the NXP Kinetis SDK 2.0 to PolyMCU
=======================================

1. Generate and Download the SDK package for the targetted platform/MCU

2. Unpack the SDK into a temporary location `$FREESCALE_KINETIS_ROOT`

3. Copy `$FREESCALE_KINETIS_ROOT/devices` to `$POLYMCU_ROOT/Device/Freescale`  
   Copy `$FREESCALE_KINETIS_ROOT/middleware` to `$POLYMCU_ROOT/Device/Freescale`