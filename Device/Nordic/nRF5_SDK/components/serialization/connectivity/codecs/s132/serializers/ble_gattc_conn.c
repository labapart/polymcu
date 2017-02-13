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

#include "ble_gattc_conn.h"
#include "ble_serialization.h"
#include "ble_struct_serialization.h"
#include "ble_gattc_struct_serialization.h"
#include "cond_field_serialization.h"
#include "app_util.h"
#include <string.h>


uint32_t ble_gattc_attr_info_discover_req_dec(uint8_t const * const              p_buf,
                                              uint16_t                           packet_len,
                                              uint16_t * const                   p_conn_handle,
                                              ble_gattc_handle_range_t * * const pp_handle_range)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_ATTR_INFO_DISCOVER);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_handle_range);
    SER_ASSERT_NOT_NULL(*pp_handle_range);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_handle_range, ble_gattc_handle_range_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gattc_attr_info_discover_rsp_enc(uint32_t         return_code,
                                              uint8_t * const  p_buf,
                                              uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_ATTR_INFO_DISCOVER);
}

uint32_t ble_gattc_char_value_by_uuid_read_req_dec(uint8_t const * const p_buf,
                                                   uint16_t              packet_len,
                                                   uint16_t * const      p_conn_handle,
                                                   ble_uuid_t * * const  pp_uuid,
                                                   ble_gattc_handle_range_t * * const
                                                   pp_handle_range)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_CHAR_VALUE_BY_UUID_READ);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_uuid);
    SER_ASSERT_NOT_NULL(*pp_uuid);
    SER_ASSERT_NOT_NULL(pp_handle_range);
    SER_ASSERT_NOT_NULL(*pp_handle_range);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_uuid, ble_uuid_t_dec);
    SER_PULL_COND(pp_handle_range, ble_gattc_handle_range_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gattc_char_value_by_uuid_read_rsp_enc(uint32_t         return_code,
                                                   uint8_t * const  p_buf,
                                                   uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_CHAR_VALUE_BY_UUID_READ);
}


uint32_t ble_gattc_char_values_read_req_dec(uint8_t const * const p_buf,
                                            uint16_t              packet_len,
                                            uint16_t * const      p_conn_handle,
                                            uint16_t * * const    pp_handles,
                                            uint16_t * const      p_handle_count)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_CHAR_VALUES_READ);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_handles);
    SER_ASSERT_NOT_NULL(*pp_handles);
    SER_ASSERT_NOT_NULL(p_handle_count);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_len16data16(pp_handles, p_handle_count);

    SER_REQ_DEC_END;
}


uint32_t ble_gattc_char_values_read_rsp_enc(uint32_t         return_code,
                                            uint8_t * const  p_buf,
                                            uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_CHAR_VALUES_READ);
}

uint32_t ble_gattc_characteristics_discover_req_dec(
                uint8_t const * const              p_buf,
                uint16_t                           packet_len,
                uint16_t * const                   p_conn_handle,
                ble_gattc_handle_range_t * * const pp_handle_range)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_CHARACTERISTICS_DISCOVER);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_handle_range, ble_gattc_handle_range_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gattc_characteristics_discover_rsp_enc(uint32_t         return_code,
                                                    uint8_t * const  p_buf,
                                                    uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_CHARACTERISTICS_DISCOVER);
}


uint32_t ble_gattc_descriptors_discover_req_dec(uint8_t const * const              p_buf,
                                                uint16_t                           packet_len,
                                                uint16_t * const                   p_conn_handle,
                                                ble_gattc_handle_range_t * * const pp_handle_range)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_DESCRIPTORS_DISCOVER);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_handle_range, ble_gattc_handle_range_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gattc_descriptors_discover_rsp_enc(uint32_t         return_code,
                                                uint8_t * const  p_buf,
                                                uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_DESCRIPTORS_DISCOVER);
}

uint32_t ble_gattc_hv_confirm_req_dec(uint8_t const * const p_buf,
                                      uint32_t              packet_len,
                                      uint16_t * const      p_conn_handle,
                                      uint16_t * const      p_handle)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_HV_CONFIRM);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint16(p_handle);

    SER_REQ_DEC_END;
}

uint32_t ble_gattc_hv_confirm_rsp_enc(uint32_t         return_code,
                                      uint8_t * const  p_buf,
                                      uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_HV_CONFIRM);
}

uint32_t ble_gattc_primary_services_discover_req_dec(uint8_t const * const p_buf,
                                                     uint16_t              packet_len,
                                                     uint16_t * const      p_conn_handle,
                                                     uint16_t * const      p_start_handle,
                                                     ble_uuid_t * * const  pp_srvc_uuid)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_PRIMARY_SERVICES_DISCOVER);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint16(p_start_handle);
    SER_PULL_COND(pp_srvc_uuid, ble_uuid_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gattc_primary_services_discover_rsp_enc(uint32_t         return_code,
                                                     uint8_t * const  p_buf,
                                                     uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_PRIMARY_SERVICES_DISCOVER);
}

uint32_t ble_gattc_read_req_dec(uint8_t const * const p_buf,
                                uint16_t              packet_len,
                                uint16_t * const      p_conn_handle,
                                uint16_t * const      p_handle,
                                uint16_t * const      p_offset)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_READ);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint16(p_handle);
    SER_PULL_uint16(p_offset);

    SER_REQ_DEC_END;
}

uint32_t ble_gattc_read_rsp_enc(uint32_t         return_code,
                                uint8_t * const  p_buf,
                                uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_READ);
}

uint32_t ble_gattc_relationships_discover_req_dec(uint8_t const * const p_buf,
                                                  uint16_t              packet_len,
                                                  uint16_t * const      p_conn_handle,
                                                  ble_gattc_handle_range_t * * const pp_handle_range)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_RELATIONSHIPS_DISCOVER);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_handle_range, ble_gattc_handle_range_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gattc_relationships_discover_rsp_enc(uint32_t         return_code,
                                                  uint8_t * const  p_buf,
                                                  uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_RELATIONSHIPS_DISCOVER);
}

uint32_t ble_gattc_write_req_dec(uint8_t const * const              p_buf,
                                 uint16_t                           packet_len,
                                 uint16_t * const                   p_conn_handle,
                                 ble_gattc_write_params_t * * const pp_write_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_WRITE);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_write_params, ble_gattc_write_params_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gattc_write_rsp_enc(uint32_t         return_code,
                                 uint8_t * const  p_buf,
                                 uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_WRITE);
}

uint32_t ble_gattc_exchange_mtu_request_req_dec(uint8_t const * const       p_buf,
                                                uint16_t                    packet_len,
                                                uint16_t * const            p_conn_handle,
                                                uint16_t * const            p_client_rx_mtu)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GATTC_EXCHANGE_MTU_REQUEST);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint16(p_client_rx_mtu);

    SER_REQ_DEC_END;
}

uint32_t ble_gattc_exchange_mtu_request_rsp_enc(uint32_t         return_code,
                                                uint8_t * const  p_buf,
                                                uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GATTC_EXCHANGE_MTU_REQUEST);
}
