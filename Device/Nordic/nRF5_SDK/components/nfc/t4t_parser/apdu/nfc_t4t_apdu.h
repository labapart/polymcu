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

#ifndef NFC_T4T_APDU_H__
#define NFC_T4T_APDU_H__

/**@file
 *
 * @defgroup nfc_t4t_apdu APDU reader/writer
 * @{
 * @ingroup  nfc_t4t_parser
 *
 * @brief    APDU reader/writer for Type 4 Tag communication.
 *
 */

#include <stdint.h>
#include <string.h>
#include "sdk_errors.h"
#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLASS_BYTE_NO_SECURE_MSG 0x00 ///< Class byte indicating no secure messaging, used in C-APDU.

/**
 * @name Parameters used when selecting instruction code in C-APDU.
 * @{
 */
#define SELECT_BY_FILE_ID 0x000C ///< Select by file identifier, first or only occurence.
#define SELECT_BY_NAME    0x0400 ///< Select by name, first or only occurence.
/** @} */

/**
 * @name Status codes contained in R-APDU.
 * @{
 */
#define RAPDU_STATUS_CMD_COMPLETED      0x9000 ///< Command completed successfully.
#define RAPDU_STATUS_SEL_ITEM_NOT_FOUND 0x6A82 ///< Selected item has not been found.
/** @} */

/**
 * @brief Possible instruction codes in C-APDU.
 */
typedef enum
{
    NFC_T4T_CAPDU_SELECT_INS = 0xA4, ///< Code used for selecting EF or NDEF application.
    NFC_T4T_CAPDU_READ_INS   = 0xB0, ///< Code used for selecting EF or NDEF application.
    NFC_T4T_CAPDU_UPDATE_INS = 0xD6  ///< Code used for selecting EF or NDEF application.
} nfc_t4t_comm_apdu_ins_type_t;

/**
 * @brief APDU data field descriptor.
 */
typedef struct
{
    uint16_t  len;    ///< Data field length.
    uint8_t * p_buff; ///< Pointer to data field.
} nfc_t4t_apdu_data_t;

/**
 * @brief Command Application Protocol Data Unit (C-APDU) descriptor.
 */
typedef struct
{
    uint8_t                      class_byte;  ///< Class byte.
    nfc_t4t_comm_apdu_ins_type_t instruction; ///< The chosen code of instruction.
    uint16_t                     parameter;   ///< Parameters associated with the instruction code.
    nfc_t4t_apdu_data_t          data;        ///< Optional data fields (Lc + data bytes).
    uint16_t                     resp_len;    ///< Optional response length field (Le).
} nfc_t4t_comm_apdu_t;

/**
 * @brief Response Application Protocol Data Unit (R-APDU) descriptor.
 */
typedef struct
{
    uint16_t            status; ///< Mandatory status field.
    nfc_t4t_apdu_data_t data;   ///< Optional data field.
} nfc_t4t_resp_apdu_t;

/**
 * @brief Macro for verifying R-APDU descriptor status.
 *
 * This macro verifies R-APDU descriptor status. It will cause the exterior
 * function to return nrf_error translated from R-APDU status, if the status is
 * not equal to @ref RAPDU_STATUS_CMD_COMPLETED.
 *
 * @param[in] P_RAPDU                  Pointer to R-APDU descriptor.
 *
 * @retval    NRF_ERROR_NOT_FOUND      If C-APDU select command could not find the selected item.
 * @retval    NRF_ERROR_INTERNAL       Unknown R-APDU error.
 */
#define VERIFY_RAPDU_SUCCESS(P_RAPDU)                       \
    if (P_RAPDU->status == RAPDU_STATUS_SEL_ITEM_NOT_FOUND) \
    {                                                       \
        return NRF_ERROR_NOT_FOUND;                         \
    }                                                       \
    if (P_RAPDU->status != RAPDU_STATUS_CMD_COMPLETED)      \
    {                                                       \
        return NRF_ERROR_INTERNAL;                          \
    }

/**
 * @brief Function for clearing C-APDU descriptor and restoring its default values.
 *
 * @param[in]  p_cmd_apdu   Pointer to C-APDU descriptor.
 */
__STATIC_INLINE void nfc_t4t_comm_apdu_clear(nfc_t4t_comm_apdu_t * const p_cmd_apdu);

/**
 * @brief Function for clearing R-APDU descriptor and restoring its default values.
 *
 * @param[in]  p_resp_apdu   Pointer to R-APDU descriptor.
 */
__STATIC_INLINE void nfc_t4t_resp_apdu_clear(nfc_t4t_resp_apdu_t * const p_resp_apdu);

/**
 * @brief Function for encoding C-APDU.
 *
 * This function encodes C-APDU according to the provided descriptor.
 *
 * @param[in]     p_cmd_apdu      Pointer to the C-APDU descriptor.
 * @param[out]    p_raw_data      Pointer to the buffer with encoded C-APDU.
 * @param[in,out] p_len           Size of the available memory for the C-APDU as input.
 *                                Size of the generated C-APDU as output.
 *
 * @retval NRF_SUCCESS              If C-APDU was encoded successfully.
 * @retval NRF_ERROR_NO_MEM         If the predicted C-APDU size is bigger than the provided buffer space.
 * @retval NRF_ERROR_INVALID_PARAM  If C-APDU descriptor is invalid.
 * @retval NRF_ERROR_NULL           If any passed argument is NULL.
 */
ret_code_t nfc_t4t_comm_apdu_encode(nfc_t4t_comm_apdu_t const * const p_cmd_apdu,
                                    uint8_t                         * p_raw_data, 
                                    uint16_t                  * const p_len);

/**
 * @brief Function for decoding R-APDU.
 *
 * This function decodes buffer with encoded R-APDU and stores results in the R-APDU descriptor.
 *
 * @param[out]     p_resp_apdu      Pointer to the R-APDU descriptor.
 * @param[in]      p_raw_data       Pointer to the buffer with encoded R-APDU.
 * @param[in]      len              Size of of the buffer with encoded R-APDU.
 *
 * @retval NRF_SUCCESS              If R-APDU was encoded successfully.
 * @retval NRF_ERROR_INVALID_LENGTH If the buffer is too small to hold a valid R-APDU.
 * @retval NRF_ERROR_NULL           If any passed argument is NULL.
 */
ret_code_t nfc_t4t_resp_apdu_decode(nfc_t4t_resp_apdu_t * const p_resp_apdu,
                                    uint8_t       const * const p_raw_data,
                                    uint16_t                    len);

/**
 * @brief Function for printing a R-APDU descriptor.
 *
 * This function prints a R-APDU descriptor.
 *
 * @param[in]     p_resp_apdu      Pointer to the R-APDU descriptor.
 */
void nfc_t4t_resp_apdu_printout(nfc_t4t_resp_apdu_t * p_resp_apdu);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE void nfc_t4t_comm_apdu_clear(nfc_t4t_comm_apdu_t * const p_cmd_apdu)
{
    memset(p_cmd_apdu, 0, sizeof(nfc_t4t_comm_apdu_t));
}

__STATIC_INLINE void nfc_t4t_resp_apdu_clear(nfc_t4t_resp_apdu_t * const p_resp_apdu)
{
    memset(p_resp_apdu, 0, sizeof(nfc_t4t_resp_apdu_t));
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NFC_T4T_APDU_H__ */
