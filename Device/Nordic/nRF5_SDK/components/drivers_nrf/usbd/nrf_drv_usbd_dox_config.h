/**
 *
 * @defgroup nrf_drv_usbd_config USB driver configuration
 * @{
 * @ingroup nrf_drv_usbd
 */
/** @brief Enable USB driver
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define USBD_ENABLED

/** @brief USBD SMA scheduler working scheme
 *
 *  Following options are available:
 * - 0 - Prioritized access
 * - 1 - Round Robin
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_DRV_USBD_DMASCHEDULER_MODE


/** @brief Enable logging.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NRF_USBD_DRV_LOG_ENABLED



/** @} */
