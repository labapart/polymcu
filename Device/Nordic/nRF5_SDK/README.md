### Version

The current Nordic SDK is based on the revision nRF SDK 12.2.0 that can be downloaded at <https://www.nordicsemi.com/eng/nordic/download_resource/54291/51/52566736>

### Instructions to explain how Nordic SDK has been installed

1. Download Nordic SDK at <https://www.nordicsemi.com/eng/nordic/download_resource/54291/51/52566736>

2. Unpack `nRF_SDK_12.2.0_f012efa.zip` into `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK`

3. Copy the examples into the Application folder

```
cp <PolyMCU_ROOT>/Device/Nordic/nRF52_SDK/examples/ble_central/ble_app_hrs_c/main.c <PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_central/ble_central_heart_monitor.c
cp <PolyMCU_ROOT>/Device/Nordic/nRF52_SDK/examples/ble_peripheral/ble_app_hrs/main.c <PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_peripheral/ble_peripheral_heart_monitor.c
```

4. Move `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/components/libraries/bsp` to `<PolyMCU_ROOT>/Board/Nordic/`
5. Move `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/components/boards` to `<PolyMCU_ROOT>/Board/Nordic/`

6. Remove `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/examples` and `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/external`
