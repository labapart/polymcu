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

#include "nrf_soc_app.h"
#include "nrf_soc.h"
#include <stdlib.h>
#include <string.h>
#include "ble_serialization.h"
#include "cond_field_serialization.h"
#include "nrf_soc_struct_serialization.h"
#include "app_util.h"


uint32_t ecb_block_encrypt_req_enc(nrf_ecb_hal_data_t * p_ecb_data,
                                   uint8_t * const      p_buf,
                                   uint32_t * const     p_buf_len)
{
    SER_REQ_ENC_BEGIN(SD_ECB_BLOCK_ENCRYPT);
    SER_PUSH_COND(p_ecb_data, nrf_ecb_hal_data_t_in_enc);
    SER_REQ_ENC_END;
}


uint32_t ecb_block_encrypt_rsp_dec(uint8_t const * const        p_buf,
                                   uint32_t                     packet_len,
                                   nrf_ecb_hal_data_t * * const pp_ecb_data,
                                   uint32_t * const             p_result_code)
{
    SER_RSP_DEC_BEGIN(SD_ECB_BLOCK_ENCRYPT);
    SER_PULL_COND(pp_ecb_data, nrf_ecb_hal_data_t_out_dec);
    SER_RSP_DEC_END;
}


uint32_t power_system_off_req_enc(uint8_t * const p_buf, uint32_t * const p_buf_len)
{
    SER_REQ_ENC_BEGIN(SD_POWER_SYSTEM_OFF);
    SER_REQ_ENC_END;
}


uint32_t temp_get_req_enc(int32_t const * const p_temp,
                          uint8_t * const       p_buf,
                          uint32_t * const      p_buf_len)
{
    SER_REQ_ENC_BEGIN(SD_TEMP_GET);
    SER_PUSH_COND(p_temp, NULL);
    SER_REQ_ENC_END;
}

uint32_t temp_get_rsp_dec(uint8_t const * const p_buf,
                          uint32_t              packet_len,
                          uint32_t * const      p_result_code,
                          int32_t * * const     pp_temp)
{
    SER_RSP_DEC_BEGIN(SD_TEMP_GET);
    SER_PULL_COND(pp_temp, uint32_t_dec);
    SER_RSP_DEC_END;
}
