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

#ifndef NRF_SOC_STRUCT_SERIALIZATION_H__
#define NRF_SOC_STRUCT_SERIALIZATION_H__

#include "nrf_soc.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t nrf_ecb_hal_data_t_in_enc(void const * const p_void_struct,
                         uint8_t * const    p_buf,
                         uint32_t           buf_len,
                         uint32_t * const   p_index);

uint32_t nrf_ecb_hal_data_t_in_dec(uint8_t const * const p_buf,
                         uint32_t              buf_len,
                         uint32_t * const      p_index,
                         void * const          p_void_struct);

uint32_t nrf_ecb_hal_data_t_out_enc(void const * const p_void_struct,
                         uint8_t * const    p_buf,
                         uint32_t           buf_len,
                         uint32_t * const   p_index);

uint32_t nrf_ecb_hal_data_t_out_dec(uint8_t const * const p_buf,
                         uint32_t              buf_len,
                         uint32_t * const      p_index,
                         void * const          p_void_struct);

#ifdef __cplusplus
}
#endif

#endif // NRF_SOC_STRUCT_SERIALIZATION_H__
