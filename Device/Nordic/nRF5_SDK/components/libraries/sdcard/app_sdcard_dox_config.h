/**
 *
 * @defgroup app_sdcard_config SD/MMC card support using SPI configuration
 * @{
 * @ingroup app_sdcard
 */
/** @brief Enabling the SD card module
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_SDCARD_ENABLED

/** @brief SPI instance used
 *
 *  Following options are available:
 * - 0
 * - 1
 * - 2
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_SDCARD_SPI_INSTANCE


/** @brief SPI frequency
 *
 *  Following options are available:
 * - 33554432 - 125 kHz
 * - 67108864 - 250 kHz
 * - 134217728 - 500 kHz
 * - 268435456 - 1 MHz
 * - 536870912 - 2 MHz
 * - 1073741824 - 4 MHz
 * - 2147483648 - 8 MHz
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_SDCARD_FREQ_INIT


/** @brief SPI frequency
 *
 *  Following options are available:
 * - 33554432 - 125 kHz
 * - 67108864 - 250 kHz
 * - 134217728 - 500 kHz
 * - 268435456 - 1 MHz
 * - 536870912 - 2 MHz
 * - 1073741824 - 4 MHz
 * - 2147483648 - 8 MHz
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_SDCARD_FREQ_DATA



/** @} */
