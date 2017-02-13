/**
 *
 * @defgroup nrf_drv_power_config POWER peripheral driver configuration
 * @{
 * @ingroup nrf_drv_power
 */
/** @brief Enable POWER driver
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define POWER_ENABLED

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
#define POWER_CONFIG_IRQ_PRIORITY


/** @brief The default configuration of main DCDC regulator
 *
 * This settings means only that components for DCDC regulator are installed and it can be enabled.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define POWER_CONFIG_DEFAULT_DCDCEN


/** @brief The default configuration of High Voltage DCDC regulator
 *
 * This settings means only that components for DCDC regulator are installed and it can be enabled.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define POWER_CONFIG_DEFAULT_DCDCENHV



/** @} */
