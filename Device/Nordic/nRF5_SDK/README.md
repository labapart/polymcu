### Version

The current Nordic SDK is based on the revision nRF SDK 11.0.0 that can be downloaded at <https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/nRF5_SDK_11.0.0_89a8197.zip>

### Instructions to explain how Nordic SDK has been installed

1. Download Nordic SDK at <https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/nRF5_SDK_11.0.0_89a8197.zip>

2. Unpack `nRF5_SDK_11.0.0_89a8197.zip` into `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK`

3. Copy the examples into the Application folder

```
cp <PolyMCU_ROOT>/Device/Nordic/nRF5_SDK/examples/ble_central/ble_app_hrs_c/main.c <PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_central/ble_central_heart_monitor.c
cp <PolyMCU_ROOT>/Device/Nordic/nRF5_SDK/examples/ble_peripheral/ble_app_hrs/main.c <PolyMCU_ROOT>/Application/Nordic/ble_app_hrs_peripheral/ble_peripheral_heart_monitor.c
```

4. Move `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/components/libraries/bsp` to `<PolyMCU_ROOT>/Board/Nordic/`
5. Move `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/components/boards` to `<PolyMCU_ROOT>/Board/Nordic/`

6. Remove `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/examples` and `<PolyMCU_ROOT>/Device/Nordic/nRF_SDK/external`
