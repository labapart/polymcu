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

#include "ble.h"
#include "ble_gap_evt_conn.h"
#include <string.h>
#include "ble_serialization.h"
#include "cond_field_serialization.h"
#include "ble_gap_struct_serialization.h"
#include "conn_ble_gap_sec_keys.h"
#include "app_util.h"

extern ser_ble_gap_conn_keyset_t m_conn_keys_table[];

uint32_t ble_gap_evt_adv_report_enc(ble_evt_t const * const p_event,
                                    uint32_t                event_len,
                                    uint8_t * const         p_buf,
                                    uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_ADV_REPORT);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.adv_report, ble_gap_evt_adv_report_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_auth_key_request_enc(ble_evt_t const * const p_event,
                                          uint32_t                event_len,
                                          uint8_t * const         p_buf,
                                          uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_AUTH_KEY_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_uint8(&p_event->evt.gap_evt.params.auth_key_request.key_type);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_auth_status_enc(ble_evt_t const * const p_event,
                                     uint32_t                event_len,
                                     uint8_t * const         p_buf,
                                     uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_AUTH_STATUS);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.auth_status, ble_gap_evt_auth_status_t_enc);

    // keyset is an extension of standard event data - used to synchronize keys at application
    uint32_t conn_index;
    err_code = conn_ble_gap_sec_context_find(p_event->evt.gap_evt.conn_handle, &conn_index);
    if (err_code == NRF_SUCCESS)
    {
        SER_PUSH_FIELD(&(m_conn_keys_table[conn_index].keyset), ble_gap_sec_keyset_t_enc);

        err_code = conn_ble_gap_sec_context_destroy(p_event->evt.gap_evt.conn_handle);
        SER_ASSERT(err_code == NRF_SUCCESS, err_code);
    }
    else
    {
        err_code = NRF_SUCCESS;
    }

    SER_EVT_ENC_END;
}


uint32_t ble_gap_evt_conn_param_update_enc(ble_evt_t const * const p_event,
                                           uint32_t                event_len,
                                           uint8_t * const         p_buf,
                                           uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_CONN_PARAM_UPDATE);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.conn_param_update, ble_gap_evt_conn_param_update_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_conn_param_update_request_enc(ble_evt_t const * const p_event,
                                                   uint32_t                event_len,
                                                   uint8_t * const         p_buf,
                                                   uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.conn_param_update_request,
                   ble_gap_evt_conn_param_update_request_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_conn_sec_update_enc(ble_evt_t const * const p_event,
                                         uint32_t                event_len,
                                         uint8_t * const         p_buf,
                                         uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_CONN_SEC_UPDATE);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.conn_sec_update, ble_gap_evt_conn_sec_update_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_connected_enc(ble_evt_t const * const p_event,
                                   uint32_t                event_len,
                                   uint8_t * const         p_buf,
                                   uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_CONNECTED);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.connected, ble_gap_evt_connected_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_disconnected_enc(ble_evt_t const * const p_event,
                                      uint32_t                event_len,
                                      uint8_t * const         p_buf,
                                      uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_DISCONNECTED);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.disconnected, ble_gap_evt_disconnected_t_enc);

    // If disconnected and context is not yet destroyed, destroy it now
    uint32_t conn_index;
	err_code = conn_ble_gap_sec_context_find(p_event->evt.gap_evt.conn_handle, &conn_index);
	if (err_code == NRF_SUCCESS)
	{
		err_code = conn_ble_gap_sec_context_destroy(p_event->evt.gap_evt.conn_handle);
		SER_ASSERT(err_code == NRF_SUCCESS, err_code);
	}
    err_code = NRF_SUCCESS;

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_key_pressed_enc(ble_evt_t const * const p_event,
                                   uint32_t                event_len,
                                   uint8_t * const         p_buf,
                                   uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_KEY_PRESSED);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_uint8(&p_event->evt.gap_evt.params.key_pressed.kp_not);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_lesc_dhkey_request_enc(ble_evt_t const * const p_event,
                                            uint32_t                event_len,
                                            uint8_t * const         p_buf,
                                            uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_LESC_DHKEY_REQUEST);

    uint8_t ser_data = p_event->evt.gap_evt.params.lesc_dhkey_request.oobd_req & 0x01;
    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_COND(p_event->evt.gap_evt.params.lesc_dhkey_request.p_pk_peer, ble_gap_lesc_p256_pk_t_enc);
    SER_PUSH_uint8(&ser_data);

    SER_EVT_ENC_END;
}

#define PASSKEY_LEN sizeof (p_event->evt.gap_evt.params.passkey_display.passkey)


uint32_t ble_gap_evt_passkey_display_enc(ble_evt_t const * const p_event,
                                         uint32_t                event_len,
                                         uint8_t * const         p_buf,
                                         uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_PASSKEY_DISPLAY);

    uint8_t ser_data = p_event->evt.gap_evt.params.passkey_display.match_request & 0x01;
    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_uint8array(p_event->evt.gap_evt.params.passkey_display.passkey, BLE_GAP_PASSKEY_LEN);
    SER_PUSH_uint8(&ser_data);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_rssi_changed_enc(ble_evt_t const * const p_event,
                                      uint32_t                event_len,
                                      uint8_t * const         p_buf,
                                      uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_RSSI_CHANGED);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_int8(&p_event->evt.gap_evt.params.rssi_changed.rssi);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_scan_req_report_enc(ble_evt_t const * const p_event,
                                         uint32_t                event_len,
                                         uint8_t * const         p_buf,
                                         uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_SCAN_REQ_REPORT);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.scan_req_report.peer_addr, ble_gap_addr_t_enc);
    SER_PUSH_int8(&p_event->evt.gap_evt.params.scan_req_report.rssi);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_sec_info_request_enc(ble_evt_t const * const p_event,
                                          uint32_t                event_len,
                                          uint8_t * const         p_buf,
                                          uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_SEC_INFO_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.sec_info_request, ble_gap_evt_sec_info_request_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_sec_params_request_enc(ble_evt_t const * const p_event,
                                            uint32_t                event_len,
                                            uint8_t * const         p_buf,
                                            uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_SEC_PARAMS_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.sec_params_request, ble_gap_evt_sec_params_request_t_enc);
    
    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_sec_request_enc(ble_evt_t const * const p_event,
                                     uint32_t                event_len,
                                     uint8_t * const         p_buf,
                                     uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_SEC_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gap_evt.params.sec_request, ble_gap_evt_sec_request_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gap_evt_timeout_enc(ble_evt_t const * const p_event,
                                 uint32_t                event_len,
                                 uint8_t * const         p_buf,
                                 uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GAP_EVT_TIMEOUT);

    SER_PUSH_uint16(&p_event->evt.gap_evt.conn_handle);
    SER_PUSH_uint8(&p_event->evt.gap_evt.params.timeout.src);

    SER_EVT_ENC_END;
}
