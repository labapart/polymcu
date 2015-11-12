### Version

The current LPC 175x_6x SDK is based on LPCOpen v2.10 that can be downloaded at http://www.lpcware.com/system/files/lpcopen_2_10_lpcxpresso_nxp_lpcxpresso_1769.zip

### Instructions to explain how LPC 175x_6x SDK has been installed

1. Download LPC 175x_6x SDK at http://www.lpcware.com/system/files/lpcopen_2_10_lpcxpresso_nxp_lpcxpresso_1769.zip

2. Unpack `lpcopen_2_10_lpcxpresso_nxp_lpcxpresso_1769.zip` into a temporary folder

3. Copy `<LPC_175x_6x_ROOT>/lpc_chip_175x_6x` into `<PolyMCU_ROOT>/Device/NXP`

4. Copy `<LPC_175x_6x_ROOT>/usbd_lib_hid_generic/example/src/cr_startup_lpc175x_6x.c` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_175x_6x`  
   Copy `<LPC_175x_6x_ROOT>/usbd_lib_hid_generic/example/src/sysinit.c` into `<PolyMCU_ROOT>/Device/NXP/lpc_chip_175x_6x`
