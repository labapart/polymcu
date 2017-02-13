/**
 *
 * @defgroup nrf_balloc_config Block allocator module configuration
 * @{
 * @ingroup nrf_balloc
 */
/** @brief Enabling Balloc Module
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_ENABLED

/** @brief Enables logging in the module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_LOG_ENABLED

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
#define NRF_BALLOC_CONFIG_LOG_LEVEL


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
#define NRF_BALLOC_CONFIG_INFO_COLOR


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
#define NRF_BALLOC_CONFIG_DEBUG_COLOR



/** @brief Enables debug mode in the module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_DEBUG_ENABLED

/** @brief Number of words used as head guard.
 *
 *  Minimum value: 0
 *  Maximum value: 255
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_HEAD_GUARD_WORDS


/** @brief Number of words used as tail guard.
 *
 *  Minimum value: 0
 *  Maximum value: 255
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_TAIL_WORDS


/** @brief Enables basic checks in this module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_BASIC_CHECKS_ENABLED


/** @brief Enables double memory free check in this module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_DOUBLE_FREE_CHECK_ENABLED


/** @brief Enables free memory corruption check in this module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_BALLOC_CONFIG_DATA_TRASHING_CHECK_ENABLED




/** @} */
