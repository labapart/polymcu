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

#include "ble_gattc_evt_conn.h"
#include "ble_serialization.h"
#include "ble_gattc_struct_serialization.h"
#include "app_util.h"
#include <string.h>

uint32_t ble_gattc_evt_attr_info_disc_rsp_enc(ble_evt_t const * const p_event,
                                              uint32_t                event_len,
                                              uint8_t * const         p_buf,
                                              uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_ATTR_INFO_DISC_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.attr_info_disc_rsp,
                   ble_gattc_evt_attr_info_disc_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_char_disc_rsp_enc(ble_evt_t const * const p_event,
                                         uint32_t                event_len,
                                         uint8_t * const         p_buf,
                                         uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_CHAR_DISC_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.char_disc_rsp,
                   ble_gattc_evt_char_disc_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_char_val_by_uuid_read_rsp_enc(ble_evt_t const * const p_event,
                                                     uint32_t                event_len,
                                                     uint8_t * const         p_buf,
                                                     uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.char_val_by_uuid_read_rsp,
                   ble_gattc_evt_char_val_by_uuid_read_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_char_vals_read_rsp_enc(ble_evt_t const * const p_event,
                                              uint32_t                event_len,
                                              uint8_t * const         p_buf,
                                              uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_CHAR_VALS_READ_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.char_vals_read_rsp,
                   ble_gattc_evt_char_vals_read_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_desc_disc_rsp_enc(ble_evt_t const * const p_event,
                                         uint32_t                event_len,
                                         uint8_t * const         p_buf,
                                         uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_DESC_DISC_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.desc_disc_rsp,
                   ble_gattc_evt_desc_disc_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_hvx_enc(ble_evt_t const * const p_event,
                               uint32_t                event_len,
                               uint8_t * const         p_buf,
                               uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_HVX);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.hvx, ble_gattc_evt_hvx_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_prim_srvc_disc_rsp_enc(ble_evt_t const * const p_event,
                                              uint32_t                event_len,
                                              uint8_t * const         p_buf,
                                              uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.prim_srvc_disc_rsp,
                   ble_gattc_evt_prim_srvc_disc_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_read_rsp_enc(ble_evt_t const * const p_event,
                                    uint32_t                event_len,
                                    uint8_t * const         p_buf,
                                    uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_READ_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.read_rsp,
                   ble_gattc_evt_read_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_rel_disc_rsp_enc(ble_evt_t const * const p_event,
                                        uint32_t                event_len,
                                        uint8_t * const         p_buf,
                                        uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_REL_DISC_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.rel_disc_rsp,
                   ble_gattc_evt_rel_disc_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_timeout_enc(ble_evt_t const * const p_event,
                                   uint32_t                event_len,
                                   uint8_t * const         p_buf,
                                   uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_TIMEOUT);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.timeout,
                   ble_gattc_evt_timeout_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_write_rsp_enc(ble_evt_t const * const p_event,
                                     uint32_t                event_len,
                                     uint8_t * const         p_buf,
                                     uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_WRITE_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.write_rsp,
                   ble_gattc_evt_write_rsp_t_enc);

    SER_EVT_ENC_END;
}

uint32_t ble_gattc_evt_exchange_mtu_rsp_enc(ble_evt_t const * const p_event,
                                            uint32_t                event_len,
                                            uint8_t * const         p_buf,
                                            uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTC_EVT_EXCHANGE_MTU_RSP);

    SER_PUSH_uint16(&p_event->evt.gattc_evt.conn_handle);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.gatt_status);
    SER_PUSH_uint16(&p_event->evt.gattc_evt.error_handle);
    SER_PUSH_FIELD(&p_event->evt.gattc_evt.params.exchange_mtu_rsp,
                   ble_gattc_evt_exchange_mtu_rsp_t_enc);

    SER_EVT_ENC_END;
}
