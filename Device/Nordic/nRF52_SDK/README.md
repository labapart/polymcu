### Version

The current Nordic SDK is based on the revision 0.9.2 that can be downloaded at https://developer.nordicsemi.com/nRF52_SDK/nRF52_SDK_v0.x.x/nRF52_SDK_0.9.2_dbc28c9.zip

### Instructions to explain how Nordic SDK has been installed

1. Download Nordic SDK at https://developer.nordicsemi.com/nRF52_SDK/nRF52_SDK_v0.x.x/nRF52_SDK_0.9.2_dbc28c9.zip

2. Unpack `nRF52_SDK_0.9.2_dbc28c9.zip` into `<PolyMCU_ROOT>/Device/Nordic/nRF52_SDK`

3. Copy the examples into the Application folder

```
cp <PolyMCU_ROOT>/Device/Nordic/nRF52_SDK/examples/ble_central/ble_app_hrs_c/main.c <PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_central/ble_central_heart_monitor.c
cp <PolyMCU_ROOT>/Device/Nordic/nRF52_SDK/examples/ble_peripheral/ble_app_hrs/main.c <PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_peripheral/ble_peripheral_heart_monitor.c
```

4. Copy `<PolyMCU_ROOT>/Device/Nordic/nRF52_SDK/examples/bsp` to `<PolyMCU_ROOT>/Board/Nordic/`

5. Remove `<PolyMCU_ROOT>/Device/Nordic/nRF52_SDK/examples`
