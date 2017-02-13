/**
 *
 * @defgroup ant_fs_config ANT File Share module. configuration
 * @{
 * @ingroup ant_fs
 */
/** @brief 
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_ENABLED

/** @brief ANT-FS network number.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_NETWORK_NUMBER


/** @brief ANT-FS channel number.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_CHANNEL_NUMBER


/** @brief Pairing timeout - how long the UI will wait for a response to a pairing request before switching to the link layer, in seconds.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_PAIRING_TIMEOUT


/** @brief Command timeout - how long the client will wait without receiving any commands before switching to the link layer, in seconds.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_LINK_COMMAND_TIMEOUT


/** @brief ANT-FS Transmission Type.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_TRANS_TYPE


/** @brief ANT device type for channel configuration.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_DEVICE_TYPE


/** @brief ANT-FS Beacon Message Period.
 *
 *  Following options are available:
 * - 0 - 0.5 Hz
 * - 1 - 1 Hz
 * - 2 - 2 Hz
 * - 3 - 4 Hz
 * - 4 - 8 Hz
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_BEACON_STATUS_PERIOD


/** @brief ANT Transmit Power.
 *
 *  Following options are available:
 * - 0 - Lowest ANT Tx power level setting. (-20dBm)
 * - 1 - ANT Tx power > Lvl 0. (-12dBm)
 * - 2 - ANT Tx power > Lvl 1. (-4dBm)
 * - 3 - ANT Tx power > Lvl 2. Default tx power level. (0dBm)
 * - 4 - ANT Tx power > Lvl 3. (+4dBm)
 * - 128 - Custom tx power selection
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_TRANSMIT_POWER


/** @brief ANT Custom Transmit Power.
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_CUSTOM_TRANSMIT_POWER


/** @brief Use pairing and key exchange authentication.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_AUTH_TYPE_PAIRING_ENABLED


/** @brief Use passkey authentication.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_AUTH_TYPE_PASSKEY_ENABLED


/** @brief Allow host to bypass authentication.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_AUTH_TYPE_PASSTHROUGH_ENABLED


/** @brief Support upload operation.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_UPLOAD_ENABLED


/** @brief Enables LED debug in the module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define ANTFS_CONFIG_DEBUG_LED_ENABLED



/** @} */
