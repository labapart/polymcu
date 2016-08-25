### Version

- The current LPC 11U6x SDK is based on LPCOpen v2.06 that can be downloaded at http://cache.nxp.com/files/microcontrollers/software/LPCWare/lpcopen_2_06_lpcxpresso_nxp_lpcxpresso_11u68.zip

### Instructions to explain how LPC 11U6x SDK has been installed

1. Download LPC 11U6x SDK at http://cache.nxp.com/files/microcontrollers/software/LPCWare/lpcopen_2_06_lpcxpresso_nxp_lpcxpresso_11u68.zip

2. Unpack `lpcopen_2_06_lpcxpresso_nxp_lpcxpresso_11u68.zip` into a temporary folder

3. Copy `<LPC_11u6x_ROOT>/lpc_chip_11u6x` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_11u6x`

4. Copy `<LPC_11uxx_ROOT>/usbd_rom_hid_generic/example/src/cr_startup_lpc11u6x.c` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_11u6x/src`  
   Copy `<LPC_11uxx_ROOT>/usbd_rom_hid_generic/example/src/sysinit.c` to `<PolyMCU_ROOT>/Device/NXP/lpc_chip_11uxx/src`
