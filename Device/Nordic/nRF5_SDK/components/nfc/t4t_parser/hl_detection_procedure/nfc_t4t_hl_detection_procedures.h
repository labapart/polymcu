/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef NFC_T4T_HL_DETECTION_PROCEDURES_H__
#define NFC_T4T_HL_DETECTION_PROCEDURES_H__

/**@file
 *
 * @defgroup nfc_t4t_parser NFC Type 4 Tag parser
 * @ingroup  nfc_t4t
 * @brief Parser for Type 4 Tag data.
 *
 * @defgroup nfc_t4t_hl_detection_procedures High-level NDEF Detection Procedure
 * @{
 * @ingroup  nfc_t4t_parser
 *
 * @brief    High-level NDEF Detection Procedure for Type 4 Tag communication.
 *
 */

#include <stdint.h>
#include "sdk_errors.h"
#include "nfc_t4t_cc_file.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function for performing NDEF Tag Application Select Procedure.
 *
 * This function performs NDEF Tag Application Select Procedure according to "Type 4 Tag Operation"
 * (Version 3.0 published on 2014-07-30) chapter 5.5.2.
 *
 * @retval NRF_SUCCESS         If NDEF Tag Application was successfully selected.
 * @retval NRF_ERROR_NOT_FOUND If NDEF Tag Application was not found.
 * @retval NRF_ERROR_NO_MEM    If the APDU buffer is too small.
 * @retval Other               Other error codes may be returned depending on function
 *                             @ref adafruit_pn532_in_data_exchange and on @ref nfc_t4t_apdu
 *                             module functions.
 */
ret_code_t nfc_t4t_ndef_tag_app_select(void);

/**
 * @brief Function for performing Capability Container Select Procedure.
 *
 * This function performs Capability Container Select Procedure according to "Type 4 Tag Operation"
 * (Version 3.0 published on 2014-07-30) chapter 5.5.3.
 *
 * @retval NRF_SUCCESS         If the Capability Container file was successfully selected.
 * @retval NRF_ERROR_NOT_FOUND If the Capability Container file was not found.
 * @retval NRF_ERROR_NO_MEM    If the APDU buffer is too small.
 * @retval Other               Other error codes might be returned depending on function
 *                             @ref adafruit_pn532_in_data_exchange and on @ref nfc_t4t_apdu
 *                             module functions.
 */
ret_code_t nfc_t4t_cc_select(void);

/**
 * @brief Function for performing Capability Container Read Procedure.
 *
 * This function performs Capability Container Read Procedure according to "Type 4 Tag Operation"
 * (Version 3.0 published on 2014-07-30) chapter 5.5.4.
 *
 * @param[out] p_cc_file  Pointer to the Capability Container descriptor.
 *
 * @retval NRF_SUCCESS             If Capability Container file was successfully read.
 * @retval NRF_ERROR_NO_MEM        If APDU buffer or CC file storage buffer is too small.
 * @retval NRF_ERROR_NOT_SUPPORTED If the requested response length in C-APDU is too big.
 * @retval NRF_ERROR_NULL          If R-APDU did not return any data bytes.
 * @retval NRF_ERROR_INVALID_DATA  If CCLEN field is not coherent with R-APDU data length.
 * @retval Other                   Other error codes may be returned depending on functions
 *                                 @ref adafruit_pn532_in_data_exchange, @ref nfc_t4t_cc_file_parse,
 *                                 and on @ref nfc_t4t_apdu module functions.
 */
ret_code_t nfc_t4t_cc_read(nfc_t4t_capability_container_t * const p_cc_file);

/**
 * @brief Function for performing NDEF Select Procedure.
 *
 * This function performs NDEF Select Procedure according to "Type 4 Tag Operation"
 * (Version 3.0 published on 2014-07-30) chapter 5.5.5.
 *
 * @param[in] file_id  File Identifier to choose the correct file.
 *
 * @retval NRF_SUCCESS         If NDEF file was successfully selected.
 * @retval NRF_ERROR_NOT_FOUND If NDEF file was not found.
 * @retval NRF_ERROR_NO_MEM    If APDU buffer is too small.
 * @retval Other               Other error codes may be returned depending on function
 *                             @ref adafruit_pn532_in_data_exchange and on @ref nfc_t4t_apdu
 *                             module functions.
 */
ret_code_t nfc_t4t_file_select(uint16_t file_id);

/**
 * @brief Function for performing NDEF Read Procedure.
 *
 * This function performs NDEF Read Procedure according to "Type 4 Tag Operation"
 * (Version 3.0 published on 2014-07-30) chapter 5.5.6.
 *
 * @param[in,out] p_cc_file          Pointer to the Capability Container descriptor.
 * @param[out]    p_ndef_file_buff   Pointer to the buffer where the NDEF file will be stored.
 * @param[in]     ndef_file_buff_len Length of the provided NDEF file buffer.
 *
 * @retval NRF_SUCCESS             If NDEF file was successfully read.
 * @retval NRF_ERROR_NO_MEM        If APDU buffer or NDEF file buffer is too small.
 * @retval NRF_ERROR_NOT_SUPPORTED If requested response length in C-APDU is too big.
 * @retval NRF_ERROR_NULL          If R-APDU did not return any data bytes.
 * @retval NRF_ERROR_INVALID_DATA  If NLEN field is not coherent with R-APDU data length.
 * @retval Other                   Other error codes may be returned depending on function
 *                                 @ref adafruit_pn532_in_data_exchange, @ref nfc_t4t_file_content_set,
 *                                 and on @ref nfc_t4t_apdu module functions.
 */
ret_code_t nfc_t4t_ndef_read(nfc_t4t_capability_container_t * const p_cc_file,
                             uint8_t                        *       p_ndef_file_buff,
                             uint8_t                                ndef_file_buff_len);

/**
 * @brief Function for performing NDEF Update Procedure.
 *
 * This function performs NDEF Update Procedure according to "Type 4 Tag Operation"
 * (Version 3.0 published on 2014-07-30) chapter 5.5.7.
 *
 * @param[in]     p_cc_file          Pointer to the Capability Container descriptor.
 * @param[in]     p_ndef_file_buff   Pointer to the buffer with NDEF file.
 * @param[in]     ndef_file_buff_len Length of the provided NDEF file.
 *
 * @retval NRF_SUCCESS              If NDEF file was successfully updated.
 * @retval NRF_ERROR_NO_MEM         If APDU buffer or NDEF file buffer is too small.
 * @retval NRF_ERROR_NOT_SUPPORTED  If the requested response length in C-APDU is too big.
 * @retval NRF_ERROR_INVALID_DATA   If NDEF file buffer is smaller than NLEN field size.
 * @retval NRF_ERROR_INVALID_LENGTH If NLEN value is not coherent with NDEF file buffer length
 *                                  or if buffer length is bigger than maximal file size.
 * @retval Other                    Other error codes may be returned depending on function
 *                                  @ref adafruit_pn532_in_data_exchange, @ref nfc_t4t_file_content_get,
 *                                  and on @ref nfc_t4t_apdu module functions.
 */
ret_code_t nfc_t4t_ndef_update(nfc_t4t_capability_container_t * const p_cc_file,
                               uint8_t                        *       p_ndef_file_buff,
                               uint8_t                                ndef_file_buff_len);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NFC_T4T_HL_DETECTION_PROCEDURES_H__ */
