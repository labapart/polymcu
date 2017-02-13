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

#include "nrf_soc.h"
#include "nrf_soc_conn.h"
#include "nrf_error.h"
#include "ble_serialization.h"
#include "cond_field_serialization.h"
#include "nrf_soc_struct_serialization.h"

uint32_t ecb_block_encrypt_req_dec(uint8_t const * const            p_buf,
                                   uint32_t                         packet_len,
                                   nrf_ecb_hal_data_t * * const     pp_ecb_data)
{
    SER_REQ_DEC_BEGIN(SD_ECB_BLOCK_ENCRYPT);
    SER_PULL_COND(pp_ecb_data, nrf_ecb_hal_data_t_in_dec);
    SER_REQ_DEC_END;
}

uint32_t ecb_block_encrypt_rsp_enc(uint32_t         return_code,
                                   uint8_t * const  p_buf,
                                   uint32_t * const p_buf_len,
                                   nrf_ecb_hal_data_t * const  p_ecb_data)
{
    SER_RSP_ENC_BEGIN(SD_ECB_BLOCK_ENCRYPT);
    SER_PUSH_COND(p_ecb_data, nrf_ecb_hal_data_t_out_enc);
    SER_RSP_ENC_END;
}
uint32_t power_system_off_req_dec(uint8_t const * const p_buf,
                                  uint16_t              packet_len)
{
    SER_REQ_DEC_BEGIN(SD_POWER_SYSTEM_OFF);
    SER_REQ_DEC_END;
}

uint32_t temp_get_req_dec(uint8_t const * const p_buf,
                          uint32_t              packet_len,
                          int32_t * * const     pp_temp)
{
    SER_REQ_DEC_BEGIN(SD_TEMP_GET);
    SER_PULL_COND(pp_temp, NULL);
    SER_REQ_DEC_END;
}

uint32_t temp_get_rsp_enc(uint32_t         return_code,
                          uint8_t * const  p_buf,
                          uint32_t * const p_buf_len,
                          int32_t * const  p_temp)
{
    SER_RSP_ENC_BEGIN(SD_TEMP_GET);
    SER_PUSH_COND(p_temp, uint32_t_enc);
    SER_RSP_ENC_END;
}
