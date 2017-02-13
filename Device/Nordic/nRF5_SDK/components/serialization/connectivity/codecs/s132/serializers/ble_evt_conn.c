/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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

#include "ble_evt_conn.h"
#include <string.h>
#include "ble_serialization.h"
#include "ble_struct_serialization.h"
#include "app_util.h"
#include "conn_ble_user_mem.h"

uint32_t ble_evt_user_mem_release_enc(ble_evt_t const * const p_event,
                                      uint32_t                event_len,
                                      uint8_t * const         p_buf,
                                      uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_EVT_USER_MEM_RELEASE);

    SER_PUSH_uint16(&p_event->evt.common_evt.conn_handle);
    SER_PUSH_uint8(&p_event->evt.common_evt.params.user_mem_release.type);
    SER_PUSH_uint16(&p_event->evt.common_evt.params.user_mem_release.mem_block.len);
    SER_PUSH_COND(p_event->evt.common_evt.params.user_mem_release.mem_block.p_mem, NULL);

    // Now user memory context can be released
    err_code = conn_ble_user_mem_context_destroy(p_event->evt.common_evt.conn_handle);
    SER_ASSERT(err_code == NRF_SUCCESS, err_code);

    SER_EVT_ENC_END;
}



uint32_t ble_evt_tx_complete_enc(ble_evt_t const * const p_event,
                                 uint32_t                event_len,
                                 uint8_t * const         p_buf,
                                 uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_EVT_TX_COMPLETE);

    SER_PUSH_uint16(&p_event->evt.common_evt.conn_handle);
    SER_PUSH_uint8(&p_event->evt.common_evt.params.tx_complete.count);

    SER_EVT_ENC_END;
}

uint32_t ble_evt_user_mem_request_enc(ble_evt_t const * const p_event,
                                      uint32_t                event_len,
                                      uint8_t * const         p_buf,
                                      uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_EVT_USER_MEM_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_uint8(&p_event->evt.common_evt.params.user_mem_request.type);

    SER_EVT_ENC_END;
}

uint32_t ble_evt_data_length_changed_enc(ble_evt_t const * const p_event,
                                         uint32_t                event_len,
                                         uint8_t * const         p_buf,
                                         uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_EVT_DATA_LENGTH_CHANGED);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.common_evt.params.data_length_changed, ble_evt_data_length_changed_t_enc);

    SER_EVT_ENC_END;
}
