### Version

The current ST STM32L4xx SDK is based on STM32Cube v1.1.0 that can be downloaded at http://www.st.com/web/en/catalog/tools/PF261908

### Instructions to explain how LPC 11uxx SDK has been installed

1. Download ST STM32Cube at http://www.st.com/web/en/catalog/tools/PF261908

2. Unpack `stm32cubel4.zip` into a temporary folder

3. Copy `<STM32_CUBE_ROOT>/Drivers/STM32L4xx_HAL_Driver` to `<PolyMCU_ROOT>/Device/ST/`
   Copy `<STM32_CUBE_ROOT>/Middlewares/ST/*` to `<PolyMCU_ROOT>/Device/ST/Middlewares/`

4. Copy `<STM32_CUBE_ROOT>/Drivers/BSP/STM32L4xx_Nucleo` to `<PolyMCU_ROOT>/Board/ST/`

5. Copy `<STM32_CUBE_ROOT>/Drivers/CMSIS/Device/ST/STM32L4xx/Include` to `<PolyMCU_ROOT>/Device/ST/STM32L4xx/`
   Copy `<STM32_CUBE_ROOT>/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/*` into `<PolyMCU_ROOT>/Device/ST/STM32L4xx/Source/`

6. Copy `<STM32_CUBE_ROOT>/Projects/STM32L476RG-Nucleo/Templates/SW4STM32/STM32L476RG_NUCLEO/STM32L476RGTx_FLASH.ld` to `<PolyMCU_ROOT>/Device/ST/STM32L4xx/Linker`

7. Copy `<PolyMCU_ROOT>/Device/ST/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_conf_template.h` into `<PolyMCU_ROOT>/Device/ST/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_conf.h`
