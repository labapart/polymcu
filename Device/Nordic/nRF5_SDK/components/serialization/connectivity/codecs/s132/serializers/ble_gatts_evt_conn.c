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

#include "ble_gatts_evt_conn.h"
#include "ble_serialization.h"
#include "ble_gatts_struct_serialization.h"
#include "conn_ble_user_mem.h"
#include "app_util.h"

extern sercon_ble_user_mem_t m_conn_user_mem_table[];


uint32_t ble_gatts_evt_rw_authorize_request_enc(ble_evt_t const * const p_event,
                                                uint32_t                event_len,
                                                uint8_t * const         p_buf,
                                                uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gatts_evt.params.authorize_request, ble_gatts_evt_rw_authorize_request_t_enc);

    if((p_event->evt.gatts_evt.params.authorize_request.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE) &&
       ((p_event->evt.gatts_evt.params.authorize_request.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) ||
        (p_event->evt.gatts_evt.params.authorize_request.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ)))
    {
        uint32_t conn_index;
        
        if(conn_ble_user_mem_context_find(p_event->evt.gatts_evt.conn_handle, &conn_index) != NRF_ERROR_NOT_FOUND)
        {
            err_code = len16data_enc(m_conn_user_mem_table[conn_index].mem_block.p_mem, m_conn_user_mem_table[conn_index].mem_block.len, p_buf, *p_buf_len, &index);
            SER_ASSERT(err_code == NRF_SUCCESS, err_code);
        }
    }

    SER_EVT_ENC_END;
}


uint32_t ble_gatts_evt_hvc_enc(ble_evt_t const * const p_event,
                               uint32_t                event_len,
                               uint8_t * const         p_buf,
                               uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_HVC);

    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gatts_evt.params.hvc, ble_gatts_evt_hvc_t_enc);

    SER_EVT_ENC_END;
}


uint32_t ble_gatts_evt_sc_confirm_enc(ble_evt_t const * const p_event,
                                      uint32_t                event_len,
                                      uint8_t * const         p_buf,
                                      uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_HVC);
    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_EVT_ENC_END;
}


uint32_t ble_gatts_evt_sys_attr_missing_enc(ble_evt_t const * const p_event,
                                            uint32_t                event_len,
                                            uint8_t * const         p_buf,
                                            uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_SYS_ATTR_MISSING);

    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gatts_evt.params.sys_attr_missing, ble_gatts_evt_sys_attr_missing_t_enc);

    SER_EVT_ENC_END;
}


uint32_t ble_gatts_evt_timeout_enc(ble_evt_t const * const p_event,
                                   uint32_t                event_len,
                                   uint8_t * const         p_buf,
                                   uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_TIMEOUT);

    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gatts_evt.params.timeout, ble_gatts_evt_timeout_t_enc);

    SER_EVT_ENC_END;
}


uint32_t ble_gatts_evt_write_enc(ble_evt_t const * const p_event,
                                 uint32_t                event_len,
                                 uint8_t * const         p_buf,
                                 uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_WRITE);

    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gatts_evt.params.write, ble_gatts_evt_write_t_enc);
    
    if((p_event->evt.gatts_evt.params.write.op == BLE_GATTS_OP_WRITE_REQ) || (p_event->evt.gatts_evt.params.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW))
    {
        uint32_t conn_index;
        if(conn_ble_user_mem_context_find(p_event->evt.gatts_evt.conn_handle, &conn_index) != NRF_ERROR_NOT_FOUND)
        {
            SER_PUSH_len16data(m_conn_user_mem_table[conn_index].mem_block.p_mem, m_conn_user_mem_table[conn_index].mem_block.len);
        }
    }

    SER_EVT_ENC_END;
}


uint32_t ble_gatts_evt_exchange_mtu_request_enc(ble_evt_t const * const p_event,
                                                uint32_t                event_len,
                                                uint8_t * const         p_buf,
                                                uint32_t * const        p_buf_len)
{
    SER_EVT_ENC_BEGIN(BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST);

    SER_PUSH_uint16(&p_event->evt.gatts_evt.conn_handle);
    SER_PUSH_FIELD(&p_event->evt.gatts_evt.params.exchange_mtu_request, ble_gatts_evt_exchange_mtu_request_t_enc);

    SER_EVT_ENC_END;
}
