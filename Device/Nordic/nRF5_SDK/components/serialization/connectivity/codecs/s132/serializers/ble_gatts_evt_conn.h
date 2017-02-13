/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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
#ifndef BLE_GATTS_EVT_CONN_H__
#define BLE_GATTS_EVT_CONN_H__

/**
 * @addtogroup ser_codecs Serialization codecs
 * @ingroup ble_sdk_lib_serialization
 */

/**
 * @addtogroup ser_conn_s130_codecs Connectivity S132 codecs
 * @ingroup ser_codecs
 */

/**@file
 *
 * @defgroup ble_gatts_evt_conn GATTS Connectivity event encoders
 * @{
 * @ingroup  ser_conn_s130_codecs
 *
 * @brief    GATTS Connectivity event encoders.
 */
#include "ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encodes @ref BLE_GATTS_EVT_HVC event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_hvc_enc(ble_evt_t const * const p_event,
                               uint32_t                event_len,
                               uint8_t * const         p_buf,
                               uint32_t * const        p_buf_len);

/**
 * @brief Encodes @ref BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_rw_authorize_request_enc(ble_evt_t const * const p_event,
                                                uint32_t                event_len,
                                                uint8_t * const         p_buf,
                                                uint32_t * const        p_buf_len);

/**
 * @brief Encodes @ref BLE_GATTS_EVT_SC_CONFIRM event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_sc_confirm_enc(ble_evt_t const * const p_event,
                                      uint32_t                event_len,
                                      uint8_t * const         p_buf,
                                      uint32_t * const        p_buf_len);

/**
 * @brief Encodes @ref BLE_GATTS_EVT_SYS_ATTR_MISSING event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_sys_attr_missing_enc(ble_evt_t const * const p_event,
                                            uint32_t                event_len,
                                            uint8_t * const         p_buf,
                                            uint32_t * const        p_buf_len);

/**
 * @brief Encodes @ref BLE_GATTS_EVT_TIMEOUT event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_timeout_enc(ble_evt_t const * const p_event,
                                   uint32_t                event_len,
                                   uint8_t * const         p_buf,
                                   uint32_t * const        p_buf_len);

/**
 * @brief Encodes @ref BLE_GATTS_EVT_WRITE event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_write_enc(ble_evt_t const * const p_event,
                                 uint32_t                event_len,
                                 uint8_t * const         p_buf,
                                 uint32_t * const        p_buf_len);

/**
 * @brief Encodes @ref BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event.
 *
 * @param[in] p_event          Pointer to the \ref ble_evt_t buffer that shall be encoded.
 * @param[in] event_len        Size (in bytes) of \p p_event buffer.
 * @param[out] p_buf           Pointer to the beginning of a buffer for encoded event packet.
 * @param[in,out] p_buf_len    \c in: Size (in bytes) of \p p_buf buffer.
 *                             \c out: Length of encoded contents in \p p_buf.
 *
 * @retval NRF_SUCCESS               Encoding success.
 * @retval NRF_ERROR_NULL            Encoding failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_LENGTH  Encoding failure. Incorrect buffer length.
 */
uint32_t ble_gatts_evt_exchange_mtu_request_enc(ble_evt_t const * const p_event,
                                                uint32_t                event_len,
                                                uint8_t * const         p_buf,
                                                uint32_t * const        p_buf_len);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
