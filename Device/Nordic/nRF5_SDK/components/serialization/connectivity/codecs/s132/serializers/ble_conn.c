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

#include <string.h>
#include "ble_conn.h"
#include "ble_serialization.h"
#include "cond_field_serialization.h"
#include "ble_struct_serialization.h"
#include "ble_gap_struct_serialization.h"
#include "app_util.h"


uint32_t ble_enable_req_dec(uint8_t const * const         p_buf,
                            uint32_t                      packet_len,
                            ble_enable_params_t * * const pp_ble_enable_params)
{
    SER_REQ_DEC_BEGIN(SD_BLE_ENABLE);
    SER_PULL_COND(pp_ble_enable_params, ble_enable_params_t_dec);
    SER_REQ_DEC_END;
}


uint32_t ble_enable_rsp_enc(uint32_t         return_code,
                            uint8_t * const  p_buf,
                            uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_ENABLE);
}


uint32_t ble_opt_get_req_dec(uint8_t const * const   p_buf,
                             uint16_t                packet_len,
                             uint32_t *  const       p_opt_id,
                             ble_opt_t **const       pp_opt )
{
    SER_REQ_DEC_BEGIN(SD_BLE_OPT_GET);

    SER_ASSERT_NOT_NULL(p_opt_id);
    SER_ASSERT_NOT_NULL(pp_opt);
    SER_ASSERT_NOT_NULL(*pp_opt);

    SER_PULL_uint32(p_opt_id);
    SER_ASSERT(((*p_opt_id == BLE_COMMON_OPT_CONN_BW)           ||
                (*p_opt_id == BLE_COMMON_OPT_PA_LNA)            ||
                (*p_opt_id == BLE_COMMON_OPT_CONN_EVT_EXT)      ||
                (*p_opt_id == BLE_GAP_OPT_CH_MAP)               ||
                (*p_opt_id == BLE_GAP_OPT_LOCAL_CONN_LATENCY)   ||
                (*p_opt_id == BLE_GAP_OPT_PASSKEY)              ||
                (*p_opt_id == BLE_GAP_OPT_SCAN_REQ_REPORT)      ||
                (*p_opt_id == BLE_GAP_OPT_COMPAT_MODE)          ||
                (*p_opt_id == BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT) ||
                (*p_opt_id == BLE_GAP_OPT_EXT_LEN)), NRF_ERROR_INVALID_PARAM);

    SER_PULL_COND(pp_opt, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_opt_get_rsp_enc(uint32_t                return_code,
                             uint8_t * const         p_buf,
                             uint32_t * const        p_buf_len,
                             uint32_t                opt_id,
                             ble_opt_t const * const p_opt)
{
    SER_RSP_ENC_BEGIN(SD_BLE_OPT_GET);

    SER_ASSERT_NOT_NULL(p_opt);
    SER_ASSERT(((opt_id == BLE_COMMON_OPT_CONN_BW)           ||
                (opt_id == BLE_COMMON_OPT_PA_LNA)            ||
                (opt_id == BLE_COMMON_OPT_CONN_EVT_EXT)      ||
                (opt_id == BLE_GAP_OPT_CH_MAP)               ||
                (opt_id == BLE_GAP_OPT_LOCAL_CONN_LATENCY)   ||
                (opt_id == BLE_GAP_OPT_PASSKEY)              ||
                (opt_id == BLE_GAP_OPT_SCAN_REQ_REPORT)      ||
                (opt_id == BLE_GAP_OPT_COMPAT_MODE)          ||
                (opt_id == BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT) ||
                (opt_id == BLE_GAP_OPT_EXT_LEN)), NRF_ERROR_INVALID_PARAM);

    SER_PUSH_uint32(&opt_id);

    field_encoder_handler_t fp_encoder = NULL;
    void const *            p_struct = NULL;

    switch (opt_id)
    {
        case BLE_COMMON_OPT_CONN_BW:
            fp_encoder = ble_common_opt_conn_bw_t_enc;
            p_struct   = &p_opt->common_opt.conn_bw;
        break;
        case BLE_COMMON_OPT_PA_LNA:
            fp_encoder = ble_common_opt_pa_lna_t_enc;
            p_struct   = &p_opt->common_opt.pa_lna;
        break;
        case BLE_COMMON_OPT_CONN_EVT_EXT:
            fp_encoder = ble_common_opt_conn_evt_ext_t_enc;
            p_struct   = &p_opt->common_opt.conn_evt_ext;
        break;
        case BLE_GAP_OPT_CH_MAP:
            fp_encoder = ble_gap_opt_ch_map_t_enc;
            p_struct   = &p_opt->gap_opt.ch_map;
        break;
        case BLE_GAP_OPT_LOCAL_CONN_LATENCY:
            fp_encoder = ble_gap_opt_local_conn_latency_t_enc;
            p_struct   = &p_opt->gap_opt.local_conn_latency;
        break;
        case BLE_GAP_OPT_PASSKEY:
            fp_encoder = ble_gap_opt_passkey_t_enc;
            p_struct   = &p_opt->gap_opt.passkey;
        break;
        case BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT:
            fp_encoder = ble_gap_opt_auth_payload_timeout_t_enc;
            p_struct   = &p_opt->gap_opt.auth_payload_timeout;
        break;
        case BLE_GAP_OPT_EXT_LEN:
            fp_encoder = ble_gap_opt_ext_len_t_enc;
            p_struct   = &p_opt->gap_opt.ext_len;
        break;
        case BLE_GAP_OPT_SCAN_REQ_REPORT:
            fp_encoder = ble_gap_opt_scan_req_report_t_enc;
            p_struct   = &p_opt->gap_opt.scan_req_report;
        break;
        case BLE_GAP_OPT_COMPAT_MODE:
            fp_encoder = ble_gap_opt_compat_mode_t_enc;
            p_struct   = &p_opt->gap_opt.compat_mode;
        break;
        default:
            SER_ASSERT(NRF_ERROR_INVALID_PARAM, NRF_ERROR_INVALID_PARAM);
        break;
    }

    SER_PUSH_FIELD(p_struct, fp_encoder);

    SER_RSP_ENC_END;
}


uint32_t ble_opt_set_req_dec(uint8_t const * const   p_buf,
                             uint16_t                packet_len,
                             uint32_t *  const       p_opt_id,
                             ble_opt_t **const       pp_opt )
{
    SER_REQ_DEC_BEGIN(SD_BLE_OPT_SET);

    SER_ASSERT_NOT_NULL(p_opt_id);
    SER_ASSERT_NOT_NULL(pp_opt);
    SER_ASSERT_NOT_NULL(*pp_opt);

    SER_PULL_uint32(p_opt_id);
    SER_ASSERT(((*p_opt_id == BLE_COMMON_OPT_CONN_BW)           ||
                (*p_opt_id == BLE_COMMON_OPT_PA_LNA)            ||
                (*p_opt_id == BLE_COMMON_OPT_CONN_EVT_EXT)      ||
                (*p_opt_id == BLE_GAP_OPT_CH_MAP)               ||
                (*p_opt_id == BLE_GAP_OPT_LOCAL_CONN_LATENCY)   ||
                (*p_opt_id == BLE_GAP_OPT_PASSKEY)              ||
                (*p_opt_id == BLE_GAP_OPT_SCAN_REQ_REPORT)      ||
                (*p_opt_id == BLE_GAP_OPT_COMPAT_MODE)          ||
                (*p_opt_id == BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT) ||
                (*p_opt_id == BLE_GAP_OPT_EXT_LEN)), NRF_ERROR_INVALID_PARAM);

    SER_PULL_COND(pp_opt, NULL);
    if (*pp_opt)
    {
        field_decoder_handler_t fp_decoder = NULL;
        void * p_struct = NULL;

        switch(*p_opt_id)
        {
            case BLE_COMMON_OPT_CONN_BW:
                fp_decoder = ble_common_opt_conn_bw_t_dec;
                p_struct   = &((*pp_opt)->common_opt.conn_bw);
            break;
            case BLE_COMMON_OPT_PA_LNA:
                fp_decoder = ble_common_opt_pa_lna_t_dec;
                p_struct   = &((*pp_opt)->common_opt.pa_lna);
            break;
            case BLE_COMMON_OPT_CONN_EVT_EXT:
                fp_decoder = ble_common_opt_conn_evt_ext_t_dec;
                p_struct   = &((*pp_opt)->common_opt.conn_evt_ext);
            break;
            case BLE_GAP_OPT_CH_MAP:
                fp_decoder = ble_gap_opt_ch_map_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.ch_map);
            break;
            case BLE_GAP_OPT_LOCAL_CONN_LATENCY:
                fp_decoder = ble_gap_opt_local_conn_latency_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.local_conn_latency);
            break;
            case BLE_GAP_OPT_PASSKEY:
                fp_decoder = ble_gap_opt_passkey_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.passkey);
            break;
            case BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT:
                fp_decoder = ble_gap_opt_auth_payload_timeout_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.auth_payload_timeout);
            break;
            case BLE_GAP_OPT_EXT_LEN:
                fp_decoder = ble_gap_opt_ext_len_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.ext_len);
            break;
            case BLE_GAP_OPT_SCAN_REQ_REPORT:
                fp_decoder = ble_gap_opt_scan_req_report_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.scan_req_report);
            break;
            case BLE_GAP_OPT_COMPAT_MODE:
                fp_decoder = ble_gap_opt_compat_mode_t_dec;
                p_struct   = &((*pp_opt)->gap_opt.compat_mode);
            break;
            default:
                SER_ASSERT(NRF_ERROR_INVALID_PARAM,NRF_ERROR_INVALID_PARAM);
            break;
        }
        
        SER_PULL_FIELD(p_struct, fp_decoder);
    }

    SER_REQ_DEC_END;
}


uint32_t ble_opt_set_rsp_enc(uint32_t                return_code,
                             uint8_t * const         p_buf,
                             uint32_t * const        p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_OPT_SET);
}

uint32_t ble_tx_packet_count_get_req_dec(uint8_t const * const p_buf,
                                         uint16_t              packet_len,
                                         uint16_t      * const p_conn_handle,
                                         uint8_t * * const     pp_count)
{
    SER_REQ_DEC_BEGIN(SD_BLE_TX_PACKET_COUNT_GET);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_count, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_tx_packet_count_get_rsp_enc(uint32_t              return_code,
                                         uint8_t * const       p_buf,
                                         uint32_t * const      p_buf_len,
                                         uint8_t const * const p_count)
{
    SER_RSP_ENC_BEGIN(SD_BLE_TX_PACKET_COUNT_GET);
    SER_PUSH_COND(p_count, uint8_t_enc);
    SER_RSP_ENC_END;
}

uint32_t ble_user_mem_reply_req_dec(uint8_t const * const          p_buf,
                                    uint32_t                       packet_len,
                                    uint16_t * const               p_conn_handle,
                                    ble_user_mem_block_t * * const pp_mem_block)
{
    SER_REQ_DEC_BEGIN(SD_BLE_USER_MEM_REPLY);

    SER_ASSERT_NOT_NULL(p_conn_handle);
    SER_ASSERT_NOT_NULL(pp_mem_block);
    SER_ASSERT_NOT_NULL(*pp_mem_block);

    SER_PULL_uint16(p_conn_handle);
    SER_PULL_COND(pp_mem_block, ble_user_mem_block_t_dec);

    SER_REQ_DEC_END;
}


uint32_t ble_user_mem_reply_rsp_enc(uint32_t         return_code,
                                    uint8_t * const  p_buf,
                                    uint32_t * const p_buf_len)
{
    SER_RSP_ENC_RESULT_ONLY(SD_BLE_USER_MEM_REPLY);
}

uint32_t ble_uuid_decode_req_dec(uint8_t const * const p_buf,
                                 uint32_t const        packet_len,
                                 uint8_t *             p_uuid_le_len,
                                 uint8_t * * const     pp_uuid_le,
                                 ble_uuid_t * * const  pp_uuid)
{
    SER_REQ_DEC_BEGIN(SD_BLE_UUID_DECODE);

    SER_PULL_len8data(pp_uuid_le, p_uuid_le_len);
    SER_PULL_COND(pp_uuid, NULL);

    SER_REQ_DEC_END;
}

uint32_t ble_uuid_decode_rsp_enc(uint32_t                 return_code,
                                 uint8_t * const          p_buf,
                                 uint32_t * const         p_buf_len,
                                 ble_uuid_t const * const p_uuid)
{
    SER_RSP_ENC_BEGIN(SD_BLE_UUID_DECODE);
    SER_PUSH_COND(p_uuid, ble_uuid_t_enc);
    SER_RSP_ENC_END;
}

uint32_t ble_uuid_encode_req_dec(uint8_t const * const p_buf,
                                 uint16_t              packet_len,
                                 ble_uuid_t * * const  pp_uuid,
                                 uint8_t * * const     pp_uuid_le_len,
                                 uint8_t * * const     pp_uuid_le)
{
    SER_REQ_DEC_BEGIN(SD_BLE_UUID_ENCODE);

    SER_ASSERT_NOT_NULL(pp_uuid);
    SER_ASSERT_NOT_NULL(pp_uuid_le_len);
    SER_ASSERT_NOT_NULL(pp_uuid_le);
    SER_ASSERT_NOT_NULL(*pp_uuid);
    SER_ASSERT_NOT_NULL(*pp_uuid_le_len);
    SER_ASSERT_NOT_NULL(*pp_uuid_le);

    SER_PULL_COND(pp_uuid, ble_uuid_t_dec);
    SER_PULL_COND(pp_uuid_le_len, NULL);
    SER_PULL_COND(pp_uuid_le, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_uuid_encode_rsp_enc(uint32_t              return_code,
                                 uint8_t * const       p_buf,
                                 uint32_t * const      p_buf_len,
                                 uint8_t               uuid_le_len,
                                 uint8_t const * const p_uuid_le)
{
    SER_RSP_ENC_BEGIN(SD_BLE_UUID_ENCODE);

    SER_PUSH_uint8(&uuid_le_len);    
    if (p_uuid_le != NULL)
    {
        SER_PUSH_uint8array(p_uuid_le, uuid_le_len);
    }

    SER_RSP_ENC_END;
}

uint32_t ble_uuid_vs_add_req_dec(uint8_t const * const   p_buf,
                                 uint16_t                packet_len,
                                 ble_uuid128_t * * const pp_uuid,
                                 uint8_t * * const       pp_uuid_type)
{
    SER_REQ_DEC_BEGIN(SD_BLE_UUID_VS_ADD);

    SER_ASSERT_NOT_NULL(pp_uuid);
    SER_ASSERT_NOT_NULL(pp_uuid_type);
    SER_ASSERT_NOT_NULL(*pp_uuid);
    SER_ASSERT_NOT_NULL(*pp_uuid_type);

    SER_PULL_COND(pp_uuid, ble_uuid128_t_dec);
    SER_PULL_COND(pp_uuid_type, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_uuid_vs_add_rsp_enc(uint32_t              return_code,
                                 uint8_t * const       p_buf,
                                 uint32_t * const      p_buf_len,
                                 uint8_t const * const p_uuid_type)
{
    SER_RSP_ENC_BEGIN(SD_BLE_UUID_VS_ADD);
    SER_PUSH_COND(p_uuid_type, uint8_t_enc);
    SER_RSP_ENC_END;
}

uint32_t ble_version_get_req_dec(uint8_t const * const   p_buf,
                                 uint16_t                packet_len,
                                 ble_version_t * * const pp_version)
{
    SER_REQ_DEC_BEGIN(SD_BLE_VERSION_GET);

    SER_ASSERT_NOT_NULL(pp_version);
    SER_ASSERT_NOT_NULL(*pp_version);

    SER_PULL_COND(pp_version, NULL);

    SER_REQ_DEC_END;
}


uint32_t ble_version_get_rsp_enc(uint32_t                    return_code,
                                 uint8_t * const             p_buf,
                                 uint32_t * const            p_buf_len,
                                 ble_version_t const * const p_version)
{
    SER_RSP_ENC_BEGIN(SD_BLE_VERSION_GET);
    SER_PUSH_FIELD(p_version, ble_version_t_enc);
    SER_RSP_ENC_END;
}



uint32_t ble_opt_id_pre_dec(uint8_t const * const   p_buf,
                            uint16_t                packet_len,
                            uint32_t *  const       p_opt_id)
{
    SER_REQ_DEC_BEGIN(SD_BLE_OPT_SET);

    SER_ASSERT_NOT_NULL(p_opt_id);
    SER_PULL_uint32(p_opt_id);

    // Pre-decoding; do not check if the whole packet was processed.
    return NRF_SUCCESS;
}
