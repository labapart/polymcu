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

#include "ble_l2cap_evt_conn.h"
#include <string.h>
#include "ble_serialization.h"
#include "ble_struct_serialization.h"
#include "app_util.h"


uint32_t ble_l2cap_evt_rx_enc(ble_evt_t const * const p_event,
                              uint32_t                event_len,
                              uint8_t * const         p_buf,
                              uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_L2CAP_EVT_RX);

    SER_PUSH_uint16(&p_event->evt.l2cap_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.l2cap_evt.params.rx, ble_l2cap_evt_rx_t_enc);

    SER_EVT_ENC_END;
}
