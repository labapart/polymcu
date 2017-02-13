/**
 *
 * @defgroup nrf_drv_clock_config CLOCK peripheral driver configuration
 * @{
 * @ingroup nrf_drv_clock
 */
/** @brief Enable CLOCK driver
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_ENABLED

/** @brief HF XTAL Frequency
 *
 *  Following options are available:
 * - 0 - Default (64 MHz)
 * - 255 - Default (16 MHz) (nRF51 family only)
 * - 0 - 32 MHz (nRF51 family only)
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_XTAL_FREQ


/** @brief LF Clock Source
 *
 *  Following options are available:
 * - 0 - RC
 * - 1 - XTAL
 * - 2 - Synth
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_LF_SRC


/** @brief Interrupt priority
 *
 * Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
 *
 *  Following options are available:
 * - 0 - 0 (highest)
 * - 1 - 1
 * - 2 - 2
 * - 3 - 3
 * - 4 - 4 (except nRF51 family)
 * - 5 - 5 (except nRF51 family)
 * - 6 - 6 (except nRF51 family)
 * - 7 - 7 (except nRF51 family)
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_IRQ_PRIORITY


/** @brief Enables logging in the module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_LOG_ENABLED

/** @brief Default Severity level
 *
 *  Following options are available:
 * - 0 - Off
 * - 1 - Error
 * - 2 - Warning
 * - 3 - Info
 * - 4 - Debug
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_LOG_LEVEL


/** @brief ANSI escape code prefix.
 *
 *  Following options are available:
 * - 0 - Default
 * - 1 - Black
 * - 2 - Red
 * - 3 - Green
 * - 4 - Yellow
 * - 5 - Blue
 * - 6 - Magenta
 * - 7 - Cyan
 * - 8 - White
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_INFO_COLOR


/** @brief ANSI escape code prefix.
 *
 *  Following options are available:
 * - 0 - Default
 * - 1 - Black
 * - 2 - Red
 * - 3 - Green
 * - 4 - Yellow
 * - 5 - Blue
 * - 6 - Magenta
 * - 7 - Cyan
 * - 8 - White
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CLOCK_CONFIG_DEBUG_COLOR




/** @} */
