/**
 *
 * @defgroup app_usbd_config USB Device library configuration
 * @{
 * @ingroup app_usbd
 */
/** @brief Enabling USBD library
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_USBD_ENABLED

/** @brief Vendor ID
 *
 * Vendor ID ordered from USB IF: http://www.usb.org/developers/vendor/
 *
 *  Minimum value: 0x0000
 *  Maximum value: 0xFFFF
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_USBD_VID


/** @brief Product ID
 *
 * Selected Product ID
 *
 *  Minimum value: 0x0000
 *  Maximum value: 0xFFFF
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_USBD_PID


/** @brief Device version, major part
 *
 * Device version, will be converted automatically to BCD notation. Use just decimal values.
 *
 *  Minimum value: 0
 *  Maximum value: 99
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_USBD_DEVICE_VER_MAJOR


/** @brief Device version, minor part
 *
 * Device version, will be converted automatically to BCD notation. Use just decimal values.
 *
 *  Minimum value: 0
 *  Maximum value: 99
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_USBD_DEVICE_VER_MINOR



/** @} */
