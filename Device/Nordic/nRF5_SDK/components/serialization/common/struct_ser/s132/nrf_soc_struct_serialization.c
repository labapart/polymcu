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

#include "nrf_soc_struct_serialization.h"
#include "ble_serialization.h"
#include "cond_field_serialization.h"
#include "app_util.h"
#include "string.h"

uint32_t nrf_ecb_hal_data_t_in_enc(void const * const p_void_struct,
                         uint8_t * const    p_buf,
                         uint32_t           buf_len,
                         uint32_t * const   p_index)
{
    SER_STRUCT_ENC_BEGIN(nrf_ecb_hal_data_t);

    SER_PUSH_uint8array(p_struct->key, SOC_ECB_KEY_LENGTH);
    SER_PUSH_uint8array(p_struct->cleartext, SOC_ECB_CLEARTEXT_LENGTH);

    SER_STRUCT_DEC_END;
}

uint32_t nrf_ecb_hal_data_t_in_dec(uint8_t const * const p_buf,
                         uint32_t              buf_len,
                         uint32_t * const      p_index,
                         void * const          p_void_struct)
{
    SER_STRUCT_DEC_BEGIN(nrf_ecb_hal_data_t);

    SER_PULL_uint8array(p_struct->key, SOC_ECB_KEY_LENGTH);
    SER_PULL_uint8array(p_struct->cleartext, SOC_ECB_CLEARTEXT_LENGTH);

    SER_STRUCT_DEC_END;
}

uint32_t nrf_ecb_hal_data_t_out_enc(void const * const p_void_struct,
                         uint8_t * const    p_buf,
                         uint32_t           buf_len,
                         uint32_t * const   p_index)
{
    SER_STRUCT_ENC_BEGIN(nrf_ecb_hal_data_t);
    SER_PUSH_uint8array(p_struct->ciphertext, SOC_ECB_CIPHERTEXT_LENGTH);
    SER_STRUCT_DEC_END;
}

uint32_t nrf_ecb_hal_data_t_out_dec(uint8_t const * const p_buf,
                         uint32_t              buf_len,
                         uint32_t * const      p_index,
                         void * const          p_void_struct)
{
    SER_STRUCT_DEC_BEGIN(nrf_ecb_hal_data_t);
    SER_PULL_uint8array(p_struct->ciphertext, SOC_ECB_CIPHERTEXT_LENGTH);
    SER_STRUCT_DEC_END;
}
