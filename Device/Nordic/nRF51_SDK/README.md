### Version

The current Nordic SDK is based on the revision V10 that can be downloaded at http://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v10.x.x/nRF51_SDK_10.0.0_dc26b5e.zip

### Instructions to explain how Nordic SDK has been installed

1. Download Nordic SDK at http://developer.nordicsemi.com/nRF51_SDK/nRF51_SDK_v10.x.x/nRF51_SDK_10.0.0_dc26b5e.zip

2. Unpack `nRF51_SDK_10.0.0_dc26b5e.zip` into `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK`

3. Compare `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK/examples/ble_central/ble_app_hrs_c/main.c` with `<PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_central/ble_central_heart_monitor.c` 
   Compare `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK/examples/ble_peripheral/ble_app_hrs/main.c` with `<PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_peripheral/ble_peripheral_heart_monitor.c`

4. Compare `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK/examples/bsp` with `<PolyMCU_ROOT>/Board/Nordic/bsp`

5. Remove `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK/examples`, `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK/external`, `<PolyMCU_ROOT>/Device/Nordic/nRF51_SDK/*.msi`
