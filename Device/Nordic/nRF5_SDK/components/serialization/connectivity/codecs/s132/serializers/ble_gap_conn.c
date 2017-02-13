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

#include "ble_gap_conn.h"
#include <string.h>
#include "ble_serialization.h"
#include "cond_field_serialization.h"
#include "ble_gap_struct_serialization.h"
#include "app_util.h"

uint32_t ble_gap_adv_data_set_req_dec(uint8_t const * const p_buf,
                                      uint32_t              packet_len,
                                      uint8_t * * const     pp_data,
                                      uint8_t *             p_dlen,
                                      uint8_t * * const     pp_sr_data,
                                      uint8_t *             p_srdlen)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_ADV_DATA_SET);

    SER_ASSERT_NOT_NULL(p_dlen);
    SER_ASSERT_NOT_NULL(p_srdlen);
    SER_PULL_len8data(pp_data, p_dlen);
    SER_PULL_len8data(pp_sr_data, p_srdlen);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_adv_data_set_rsp_enc(uint32_t         return_code,
                                      uint8_t * const  p_buf,
                                      uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_ADV_DATA_SET);
}

uint32_t ble_gap_adv_start_req_dec(uint8_t const * const          p_buf,
                                   uint32_t                       packet_len,
                                   ble_gap_adv_params_t * * const pp_adv_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_ADV_START);

    SER_ASSERT_NOT_NULL(pp_adv_params);
    SER_ASSERT_NOT_NULL(*pp_adv_params);
    SER_ASSERT_NOT_NULL((*pp_adv_params)->p_peer_addr);

    SER_PULL_COND(pp_adv_params, ble_gap_adv_params_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_adv_start_rsp_enc(uint32_t         return_code,
                                   uint8_t * const  p_buf,
                                   uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_ADV_START);
}


uint32_t ble_gap_adv_stop_rsp_enc(uint32_t         return_code,
                                  uint8_t * const  p_buf,
                                  uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_ADV_STOP);
}


uint32_t ble_gap_appearance_get_req_dec(uint8_t const * const p_buf,
                                        uint16_t              packet_len,
                                        uint16_t * * const    pp_appearance)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_APPEARANCE_GET);

    SER_ASSERT_NOT_NULL(pp_appearance);
    SER_ASSERT_NOT_NULL(*pp_appearance);
    SER_PULL_COND(pp_appearance, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_appearance_get_rsp_enc(uint32_t               return_code,
                                        uint8_t * const        p_buf,
                                        uint32_t * const       p_buf_len,
                                        uint16_t const * const p_appearance)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_APPEARANCE_GET);
    
    SER_PUSH_COND(p_appearance, uint16_t_enc);
    SER_RSP_ENC_END;
}

uint32_t ble_gap_appearance_set_req_dec(uint8_t const * const p_buf,
                                        uint32_t              packet_len,
                                        uint16_t * const      p_appearance)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_APPEARANCE_SET);

    SER_ASSERT_NOT_NULL(p_appearance);
    SER_PULL_uint16(p_appearance);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_appearance_set_rsp_enc(uint32_t         return_code,
                                        uint8_t * const  p_buf,
                                        uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_APPEARANCE_SET);
}

uint32_t ble_gap_auth_key_reply_req_dec(uint8_t const * const p_buf,
                                        uint16_t              packet_len,
                                        uint16_t *            p_conn_handle,
                                        uint8_t *             p_key_type,
                                        uint8_t * * const     pp_key)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_AUTH_KEY_REPLY);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(p_key_type);
    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint8(p_key_type);
    
    uint8_t key_len;
    switch (*p_key_type)
    {
        case BLE_GAP_AUTH_KEY_TYPE_NONE:
            key_len = 0;
            break;

        case BLE_GAP_AUTH_KEY_TYPE_PASSKEY:
            key_len = 6;
            break;

        case BLE_GAP_AUTH_KEY_TYPE_OOB:
            key_len = 16;
            break;

        default:
            return NRF_ERROR_INVALID_PARAM;
    }

    SER_PULL_buf(pp_key, key_len, key_len);
    SER_REQ_DEC_END;
}

uint32_t ble_gap_auth_key_reply_rsp_enc(uint32_t         return_code,
                                        uint8_t * const  p_buf,
                                        uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_AUTH_KEY_REPLY);
}


uint32_t ble_gap_authenticate_req_dec(uint8_t const * const          p_buf,
                                      uint32_t                       packet_len,
                                      uint16_t * const               p_conn_handle,
                                      ble_gap_sec_params_t * * const pp_sec_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_AUTHENTICATE);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_sec_params);
    SER_ASSERT_NOT_NULL(*pp_sec_params);
    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_sec_params, ble_gap_sec_params_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_authenticate_rsp_enc(uint32_t         return_code,
                                      uint8_t * const  p_buf,
                                      uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_AUTHENTICATE);
}

uint32_t ble_gap_conn_param_update_req_dec(uint8_t const * const           p_buf,
                                           uint32_t                        packet_len,
                                           uint16_t *                      p_conn_handle,
                                           ble_gap_conn_params_t * * const pp_conn_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_CONN_PARAM_UPDATE);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_conn_params);
    SER_ASSERT_NOT_NULL(*pp_conn_params);
    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_conn_params, ble_gap_conn_params_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_conn_param_update_rsp_enc(uint32_t         return_code,
                                           uint8_t * const  p_buf,
                                           uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_CONN_PARAM_UPDATE);
}

uint32_t ble_gap_conn_sec_get_req_dec(uint8_t const * const        p_buf,
                                      uint32_t                     packet_len,
                                      uint16_t *                   p_conn_handle,
                                      ble_gap_conn_sec_t * * const pp_conn_sec)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_CONN_SEC_GET);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_conn_sec, NULL);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_conn_sec_get_rsp_enc(uint32_t                   return_code,
                                      ble_gap_conn_sec_t * const p_conn_sec,
                                      uint8_t * const            p_buf,
                                      uint32_t * const           p_buf_len)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_CONN_SEC_GET);

    SER_PUSH_COND(p_conn_sec, ble_gap_conn_sec_t_enc);

    SER_RSP_ENC_END;
}

uint32_t ble_gap_connect_req_dec(uint8_t const * const           p_buf,
                                 uint32_t                        packet_len,
                                 ble_gap_addr_t * * const        pp_addr,
                                 ble_gap_scan_params_t * * const pp_scan_params,
                                 ble_gap_conn_params_t * * const pp_conn_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_CONNECT);

    SER_ASSERT_NOT_NULL(pp_addr);
    SER_ASSERT_NOT_NULL(*pp_addr);
    SER_ASSERT_NOT_NULL(pp_scan_params);
    SER_ASSERT_NOT_NULL(*pp_scan_params);
    SER_ASSERT_NOT_NULL(pp_conn_params);
    SER_ASSERT_NOT_NULL(*pp_conn_params);

    SER_PULL_COND(pp_addr, ble_gap_addr_t_dec);
    SER_PULL_COND(pp_scan_params, ble_gap_scan_params_t_dec);
    SER_PULL_COND(pp_conn_params, ble_gap_conn_params_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_connect_rsp_enc(uint32_t         return_code,
                                 uint8_t * const  p_buf,
                                 uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_CONNECT);
}
uint32_t ble_gap_connect_cancel_rsp_enc(uint32_t         return_code,
                                        uint8_t * const  p_buf,
                                        uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_CONNECT_CANCEL);
}

uint32_t ble_gap_device_name_get_req_dec(uint8_t const * const p_buf,
                                         uint32_t              packet_len,
                                         uint8_t * *           pp_name,
                                         uint16_t * *          pp_name_len)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_DEVICE_NAME_GET);

    SER_ASSERT_NOT_NULL(pp_name_len);

    SER_PULL_COND(pp_name_len, uint16_t_dec);
    SER_PULL_COND(pp_name, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_device_name_get_rsp_enc(uint32_t              return_code,
                                         uint8_t const * const p_dev_name,
                                         uint16_t  *           p_dev_name_len,
                                         uint8_t * const       p_buf,
                                         uint32_t * const      p_buf_len)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_DEVICE_NAME_GET);

    SER_PUSH_COND(p_dev_name_len, uint16_t_enc);

    if (p_dev_name_len)
    {
        SER_PUSH_uint8array(p_dev_name, *p_dev_name_len);
    }

    SER_RSP_ENC_END;
}

uint32_t ble_gap_device_name_set_req_dec(uint8_t const * const             p_buf,
                                         uint32_t                          packet_len,
                                         ble_gap_conn_sec_mode_t * * const pp_write_perm,
                                         uint8_t * * const                 pp_dev_name,
                                         uint16_t * const                  p_dev_name_len)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_DEVICE_NAME_SET);

    SER_ASSERT_NOT_NULL(pp_write_perm);
    SER_ASSERT_NOT_NULL(pp_dev_name);
    SER_ASSERT_NOT_NULL(p_dev_name_len);

    SER_PULL_COND(pp_write_perm, ble_gap_conn_sec_mode_t_dec);
    SER_PULL_len16data(pp_dev_name, p_dev_name_len);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_device_name_set_rsp_enc(uint32_t         return_code,
                                         uint8_t * const  p_buf,
                                         uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_DEVICE_NAME_SET);
}

uint32_t ble_gap_disconnect_req_dec(uint8_t const * const p_buf,
                                    uint32_t              packet_len,
                                    uint16_t * const      p_conn_handle,
                                    uint8_t * const       p_hci_status)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_DISCONNECT);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(p_hci_status);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint8(p_hci_status);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_disconnect_rsp_enc(uint32_t         return_code,
                                    uint8_t * const  p_buf,
                                    uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_DISCONNECT);
}

uint32_t ble_gap_encrypt_req_dec(uint8_t        const * const   p_buf,
                                 uint16_t                       packet_len,
                                 uint16_t                     * const p_conn_handle,
                                 ble_gap_master_id_t ** const   pp_master_id,
                                 ble_gap_enc_info_t  ** const   pp_enc_info)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_ENCRYPT);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_enc_info);
    SER_ASSERT_NOT_NULL(pp_master_id);
    SER_ASSERT_NOT_NULL(*pp_enc_info);
    SER_ASSERT_NOT_NULL(*pp_master_id);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_master_id, ble_gap_master_id_t_dec);
    SER_PULL_COND(pp_enc_info, ble_gap_enc_info_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_encrypt_rsp_enc(uint32_t          return_code,
                                 uint8_t   * const p_buf,
                                 uint32_t  * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_ENCRYPT);
}

uint32_t ble_gap_keypress_notify_req_dec(uint8_t const * const          p_buf,
                                          uint32_t                       packet_len,
                                          uint16_t *                     p_conn_handle,
                                          uint8_t *                      p_kp_not)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_KEYPRESS_NOTIFY);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(p_kp_not);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint8(p_kp_not);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_keypress_notify_rsp_enc(uint32_t                     return_code,
                                          uint8_t * const              p_buf,
                                          uint32_t * const             p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_KEYPRESS_NOTIFY);
}

uint32_t ble_gap_lesc_dhkey_reply_req_dec(uint8_t const * const          p_buf,
                                          uint32_t                       packet_len,
                                          uint16_t *                     p_conn_handle,
                                          ble_gap_lesc_dhkey_t * *       pp_dhkey)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_LESC_DHKEY_REPLY);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_dhkey);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_dhkey, ble_gap_lesc_dhkey_t_dec); 

    SER_REQ_DEC_END;
}

uint32_t ble_gap_lesc_dhkey_reply_rsp_enc(uint32_t                     return_code,
                                          uint8_t * const              p_buf,
                                          uint32_t * const             p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_LESC_DHKEY_REPLY);
}

uint32_t ble_gap_lesc_oob_data_get_req_dec(uint8_t const * const         p_buf,
                                          uint32_t                       packet_len,
                                          uint16_t *                     p_conn_handle,
                                          ble_gap_lesc_p256_pk_t * *     pp_pk_own,
                                          ble_gap_lesc_oob_data_t * *    pp_oobd_own)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_LESC_OOB_DATA_GET);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_oobd_own);
    SER_ASSERT_NOT_NULL(pp_pk_own);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_pk_own, ble_gap_lesc_p256_pk_t_dec);
    SER_PULL_COND(pp_oobd_own, NULL);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_lesc_oob_data_get_rsp_enc(uint32_t                     return_code,
                                           ble_gap_lesc_oob_data_t    * p_oobd_own,
                                           uint8_t * const              p_buf,
                                           uint32_t * const             p_buf_len)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_LESC_OOB_DATA_GET);

    SER_PUSH_COND(p_oobd_own, ble_gap_lesc_oob_data_t_enc);

    SER_RSP_ENC_END;
}

uint32_t ble_gap_lesc_oob_data_set_req_dec(uint8_t const * const          p_buf,
                                          uint32_t                       packet_len,
                                          uint16_t *                     p_conn_handle,
                                          ble_gap_lesc_oob_data_t * *       pp_oobd_own,
                                          ble_gap_lesc_oob_data_t * *       pp_oobd_peer)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_LESC_OOB_DATA_SET);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_oobd_own);
    SER_ASSERT_NOT_NULL(pp_oobd_peer);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_oobd_own, ble_gap_lesc_oob_data_t_dec);
    SER_PULL_COND(pp_oobd_peer, ble_gap_lesc_oob_data_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_lesc_oob_data_set_rsp_enc(uint32_t                     return_code,
                                          uint8_t * const              p_buf,
                                          uint32_t * const             p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_LESC_OOB_DATA_SET);
}

uint32_t ble_gap_ppcp_get_req_dec(uint8_t const * const           p_buf,
                                  uint16_t                        packet_len,
                                  ble_gap_conn_params_t * * const pp_conn_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_PPCP_GET);

    SER_ASSERT_NOT_NULL(pp_conn_params);
    SER_ASSERT_NOT_NULL(*pp_conn_params);
    SER_ASSERT_LENGTH_LEQ(2, packet_len);

    SER_PULL_COND(pp_conn_params, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_ppcp_get_rsp_enc(uint32_t                            return_code,
                                  uint8_t * const                     p_buf,
                                  uint32_t * const                    p_buf_len,
                                  ble_gap_conn_params_t const * const p_conn_params)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_PPCP_GET);
    
    SER_PUSH_COND(p_conn_params, ble_gap_conn_params_t_enc);
    
    SER_RSP_ENC_END;
}


uint32_t ble_gap_ppcp_set_req_dec(uint8_t const * const           p_buf,
                                  uint32_t                        packet_len,
                                  ble_gap_conn_params_t * * const pp_conn_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_PPCP_SET);

    SER_ASSERT_NOT_NULL(pp_conn_params);
    SER_PULL_COND(pp_conn_params, ble_gap_conn_params_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_gap_ppcp_set_rsp_enc(uint32_t         return_code,
                                  uint8_t * const  p_buf,
                                  uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_PPCP_SET);
}

uint32_t ble_gap_rssi_get_req_dec(uint8_t const * const p_buf,
                                  uint16_t              packet_len,
                                  uint16_t *            p_conn_handle,
                                  int8_t * * const      pp_rssi)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_RSSI_GET);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_rssi);
    SER_ASSERT_NOT_NULL(*pp_rssi);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_rssi, NULL);
    SER_REQ_DEC_END;
}


uint32_t ble_gap_rssi_get_rsp_enc(uint32_t         return_code,
                                  uint8_t * const  p_buf,
                                  uint32_t * const p_buf_len,
                                  int8_t           rssi)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_RSSI_GET);

    SER_PUSH_int8(&rssi);

    SER_RSP_ENC_END;
}

uint32_t ble_gap_rssi_start_req_dec(uint8_t const * const p_buf,
                                    uint32_t              packet_len,
                                    uint16_t *            p_conn_handle,
                                    uint8_t *             p_threshold_dbm,
                                    uint8_t *             p_skip_count)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_RSSI_START);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(p_threshold_dbm);
    SER_ASSERT_NOT_NULL(p_skip_count);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint8(p_threshold_dbm);
    SER_PULL_uint8(p_skip_count);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_rssi_start_rsp_enc(uint32_t         return_code,
                                    uint8_t * const  p_buf,
                                    uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_RSSI_START);
}

uint32_t ble_gap_rssi_stop_req_dec(uint8_t const * const p_buf,
                                   uint32_t              packet_len,
                                   uint16_t *            p_conn_handle)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_RSSI_STOP);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_PULL_uint16(p_conn_handle);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_rssi_stop_rsp_enc(uint32_t         return_code,
                                   uint8_t * const  p_buf,
                                   uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_RSSI_STOP);
}

uint32_t ble_gap_scan_start_req_dec(uint8_t const * const     p_buf,
                                    uint32_t                  packet_len,
                                    ble_gap_scan_params_t * * const pp_scan_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_SCAN_START);

    SER_ASSERT_NOT_NULL(pp_scan_params);
    SER_ASSERT_NOT_NULL(*pp_scan_params);
    
    SER_PULL_COND(pp_scan_params, ble_gap_scan_params_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_scan_start_rsp_enc(uint32_t         return_code,
                                    uint8_t * const  p_buf,
                                    uint32_t * const p_buf_len)
{
   SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_SCAN_START);
}

uint32_t ble_gap_scan_stop_rsp_enc(uint32_t         return_code,
                                  uint8_t * const  p_buf,
                                  uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_SCAN_STOP);
}

uint32_t ble_gap_sec_info_reply_req_dec(uint8_t const * const         p_buf,
                                        uint16_t                      packet_len,
                                        uint16_t *                    p_conn_handle,
                                        ble_gap_enc_info_t * * const  pp_enc_info,
                                        ble_gap_irk_t * * const       pp_id_info,
                                        ble_gap_sign_info_t * * const pp_sign_info)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_SEC_INFO_REPLY);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_enc_info);
    SER_ASSERT_NOT_NULL(pp_id_info);
    SER_ASSERT_NOT_NULL(pp_sign_info);
    SER_ASSERT_NOT_NULL(*pp_enc_info);
    SER_ASSERT_NOT_NULL(*pp_id_info);
    SER_ASSERT_NOT_NULL(*pp_sign_info);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_enc_info, ble_gap_enc_info_t_dec);
    SER_PULL_COND(pp_id_info, ble_gap_irk_t_dec);
    SER_PULL_COND(pp_sign_info, ble_gap_sign_info_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_sec_info_reply_rsp_enc(uint32_t         return_code,
                                        uint8_t * const  p_buf,
                                        uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_SEC_INFO_REPLY);
}

uint32_t ble_gap_sec_params_reply_req_dec(uint8_t const * const          p_buf,
                                          uint32_t                       packet_len,
                                          uint16_t *                     p_conn_handle,
                                          uint8_t *                      p_sec_status,
                                          ble_gap_sec_params_t * * const pp_sec_params,
                                          ble_gap_sec_keyset_t * * const pp_sec_keyset)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_SEC_PARAMS_REPLY);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(p_sec_status);
    SER_ASSERT_NOT_NULL(pp_sec_params);
    SER_ASSERT_NOT_NULL(*pp_sec_params);
    SER_ASSERT_NOT_NULL(pp_sec_keyset);
    SER_ASSERT_NOT_NULL(*pp_sec_keyset);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_uint8(p_sec_status);
    SER_PULL_COND(pp_sec_params, ble_gap_sec_params_t_dec);
    SER_PULL_COND(pp_sec_keyset, ble_gap_sec_keyset_t_dec);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_sec_params_reply_rsp_enc(uint32_t                     return_code,
                                          uint8_t * const              p_buf,
                                          uint32_t * const             p_buf_len,
                                          ble_gap_sec_keyset_t * const p_sec_keyset)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_SEC_PARAMS_REPLY);
    SER_PUSH_COND(p_sec_keyset, ble_gap_sec_keyset_t_enc);
    SER_RSP_ENC_END;
}

uint32_t ble_gap_tx_power_set_req_dec(uint8_t const * const p_buf,
                                      uint32_t              packet_len,
                                      int8_t *              p_tx_power)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_TX_POWER_SET);

    SER_ASSERT_NOT_NULL(p_tx_power);
    SER_PULL_int8(p_tx_power);

    SER_REQ_DEC_END;
}

uint32_t ble_gap_tx_power_set_rsp_enc(uint32_t         return_code,
                                      uint8_t * const  p_buf,
                                      uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_TX_POWER_SET);
}


uint32_t ble_gap_addr_get_req_dec(uint8_t const * const    p_buf,
                                  uint16_t                 packet_len,
                                  ble_gap_addr_t * * const pp_address)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_ADDR_GET);
    SER_PULL_COND(pp_address, NULL);
    SER_REQ_DEC_END;
}


uint32_t ble_gap_addr_get_rsp_enc(uint32_t                     return_code,
                                  uint8_t * const              p_buf,
                                  uint32_t * const             p_buf_len,
                                  ble_gap_addr_t const * const p_address)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_ADDR_GET);
    SER_PUSH_FIELD(p_address, ble_gap_addr_t_enc);
    SER_RSP_ENC_END;
}


uint32_t ble_gap_addr_set_req_dec(uint8_t const * const    p_buf,
                                  uint32_t                 packet_len,
                                  ble_gap_addr_t * * const pp_addr)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_ADDR_SET);
    SER_PULL_COND(pp_addr, ble_gap_addr_t_dec);
    SER_REQ_DEC_END;
}


uint32_t ble_gap_addr_set_rsp_enc(uint32_t         return_code,
                                     uint8_t * const  p_buf,
                                     uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_ADDR_SET);
}

uint32_t ble_gap_privacy_set_req_dec(uint8_t const * const        p_buf,
                                     uint32_t                     packet_len,
                                     ble_gap_privacy_params_t * * const pp_privacy_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_PRIVACY_SET);
    SER_PULL_COND(pp_privacy_params, ble_gap_privacy_params_t_dec);
    SER_REQ_DEC_END;
}

uint32_t ble_gap_privacy_set_rsp_enc(uint32_t         return_code,
                                     uint8_t * const  p_buf,
                                     uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_PRIVACY_SET);
}

uint32_t ble_gap_privacy_get_req_dec(uint8_t const * const              p_buf,
                                     uint16_t                           packet_len,
                                     ble_gap_privacy_params_t * * const pp_privacy_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_PRIVACY_GET);
    SER_PULL_COND(pp_privacy_params, ble_gap_privacy_params_t_dec);
    SER_REQ_DEC_END;
}

uint32_t ble_gap_privacy_get_rsp_enc(uint32_t                               return_code,
                                     uint8_t * const                        p_buf,
                                     uint32_t * const                       p_buf_len,
                                     ble_gap_privacy_params_t const * const p_privacy_params)
{
    SER_RSP_ENC_BEGIN(SD_BLE_GAP_PRIVACY_GET);
    SER_PUSH_COND(p_privacy_params, ble_gap_privacy_params_t_enc);
    SER_RSP_ENC_END;
}

uint32_t ble_gap_whitelist_set_req_dec(uint8_t const * const      p_buf,
                                       uint32_t                   packet_len,
                                       ble_gap_addr_t * * * const ppp_wl_addrs,
                                       uint8_t * const            p_len)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_WHITELIST_SET);

    SER_ASSERT_NOT_NULL(ppp_wl_addrs);
    SER_ASSERT_NOT_NULL(*ppp_wl_addrs);
    SER_ASSERT_NOT_NULL(**ppp_wl_addrs);
    
    uint8_t presence;
    SER_PULL_uint8(p_len);

    SER_PULL_uint8(&presence);
    if (presence == SER_FIELD_PRESENT)
    {
        ble_gap_addr_t * * const pp_wl_addrs = *ppp_wl_addrs;
        for (uint32_t i = 0; i < *p_len; ++i)
        {
            SER_PULL_COND(&(pp_wl_addrs[i]), ble_gap_addr_t_dec);
        }
    }
    else
    {
        *ppp_wl_addrs = NULL;
    }

    SER_REQ_DEC_END;
}

uint32_t ble_gap_whitelist_set_rsp_enc(uint32_t         return_code,
                                     uint8_t * const  p_buf,
                                     uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_WHITELIST_SET);
}

uint32_t ble_gap_device_identities_set_req_dec(uint8_t const * const        p_buf,
                                               uint32_t                     packet_len,
                                               ble_gap_id_key_t * * * const ppp_id_keys,
                                               ble_gap_irk_t * * * const    ppp_local_irks,
                                               uint8_t * const              p_len)
{
    SER_REQ_DEC_BEGIN(SD_BLE_GAP_DEVICE_IDENTITIES_SET);

    SER_ASSERT_NOT_NULL(ppp_id_keys);
    SER_ASSERT_NOT_NULL(*ppp_id_keys);
    SER_ASSERT_NOT_NULL(**ppp_id_keys);
    SER_ASSERT_NOT_NULL(ppp_local_irks);
    SER_ASSERT_NOT_NULL(*ppp_local_irks);
    SER_ASSERT_NOT_NULL(**ppp_local_irks);

    uint8_t presence;
    SER_PULL_uint8(p_len);

    SER_PULL_uint8(&presence);
    if (presence == SER_FIELD_PRESENT)
    {
        ble_gap_id_key_t * * const pp_id_keys = *ppp_id_keys;
        for (uint32_t i = 0; i < *p_len; ++i)
        {
            SER_PULL_COND(&(pp_id_keys[i]), ble_gap_id_key_t_dec);
        }
    }
    else
    {
        *ppp_id_keys = NULL;
    }
    SER_PULL_uint8(&presence);
    if (presence == SER_FIELD_PRESENT)
    {
        ble_gap_irk_t * * const pp_local_irks = *ppp_local_irks;
        for (uint32_t i = 0; i < *p_len; ++i)
        {
            SER_PULL_COND(&(pp_local_irks[i]), ble_gap_irk_t_dec);
        }
    }
    else
    {
        *ppp_local_irks = NULL;
    }

    SER_REQ_DEC_END;
}

uint32_t ble_gap_device_identities_set_rsp_enc(uint32_t         return_code,
                                               uint8_t * const  p_buf,
                                               uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_GAP_DEVICE_IDENTITIES_SET);
}
