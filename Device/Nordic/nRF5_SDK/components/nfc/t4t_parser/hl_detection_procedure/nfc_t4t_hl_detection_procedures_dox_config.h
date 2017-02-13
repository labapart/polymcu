/**
 *
 * @defgroup nfc_t4t_hl_detection_procedures_config NDEF Detection Procedure for Type 4 Tag configuration
 * @{
 * @ingroup nfc_t4t_hl_detection_procedures
 */
/** @brief 
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NFC_T4T_HL_DETECTION_PROCEDURES_ENABLED

/** @brief Enables logging in the module.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define NFC_T4T_HL_DETECTION_PROCEDURES_LOG_ENABLED

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
#define NFC_T4T_HL_DETECTION_PROCEDURES_LOG_LEVEL


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
#define NFC_T4T_HL_DETECTION_PROCEDURES_LOG_COLOR



/** @brief Size (in bytes) of the buffer for APDU storage
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APDU_BUFF_SIZE


/** @brief Size (in bytes) of the buffer for CC file storage
 *
 *
 * @note This is an NRF_CONFIG macro.
 */
#define CC_STORAGE_BUFF_SIZE



/** @} */
