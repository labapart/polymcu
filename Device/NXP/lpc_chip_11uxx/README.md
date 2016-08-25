### Version

- The current LPC 11uxx SDK is based on LPCOpen v2.03 that can be downloaded at http://www.lpcware.com/system/files/lpcopen_v2_03_lpcxpresso_nxp_lpcxpresso_11u37h.zip

### Instructions to explain how LPC 11uxx SDK has been installed

1. Download LPC 11uxx SDK at http://www.lpcware.com/system/files/lpcopen_v2_03_lpcxpresso_nxp_lpcxpresso_11u37h.zip

2. Unpack `lpcopen_v2_03_lpcxpresso_nxp_lpcxpresso_11u37h.zip` into a temporary folder

3. Copy `<LPC_11uxx_ROOT>/lpc_chip_11uxx_lib` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_11uxx`

4. Copy `<LPC_11uxx_ROOT>/nxp_lpcxpresso_11u37_usbd_rom_hid_generic/example/src/cr_startup_lpc11xx.c` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_11uxx/src`  
   Copy `<LPC_11uxx_ROOT>/nxp_lpcxpresso_11u37_usbd_rom_hid_generic/example/src/sysinit.c` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_11uxx/src`
