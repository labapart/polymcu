/**
 *
 * @defgroup nrf_pwr_mgmt_config Power management module configuration
 * @{
 * @ingroup nrf_pwr_mgmt
 */
/** @brief Enabling Power Management Module
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_ENABLED

/** @brief Enables logging in the module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_LOG_ENABLED

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
#define NRF_PWR_MGMT_CONFIG_LOG_LEVEL


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
#define NRF_PWR_MGMT_CONFIG_INFO_COLOR


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
#define NRF_PWR_MGMT_CONFIG_DEBUG_COLOR



/** @brief Enables pin debug in the module.
 *
 * Selected pin will be set when CPU is in sleep mode.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_DEBUG_PIN_ENABLED

/** @brief Pin number
 *
 *  Minimum value: 0
 *  Maximum value: 31
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_SLEEP_DEBUG_PIN



/** @brief Enables CPU usage monitor.
 *
 * Module will trace percentage of CPU usage in one second intervals.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_CPU_USAGE_MONITOR_ENABLED


/** @brief Enable standby timeout.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_STANDBY_TIMEOUT_ENABLED

/** @brief Standby timeout (in seconds).
 *
 * Shutdown procedure will begin no earlier than after this number of seconds.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_STANDBY_TIMEOUT_S



/** @brief Enables FPU event cleaning.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_FPU_SUPPORT_ENABLED


/** @brief Blocked shutdown procedure will be retried every second.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_AUTO_SHUTDOWN_RETRY


/** @brief Module will use @ref app_scheduler.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_PWR_MGMT_CONFIG_USE_SCHEDULER



/** @} */
