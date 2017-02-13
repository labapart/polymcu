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

/**
 * @file
 * @brief Gazell error API.
 */

#ifndef NRF_GZLL_ERROR_H__
#define NRF_GZLL_ERROR_H__

#include "sdk_errors.h"
#include "nrf_gzll.h"
#include "app_error.h"

#define GAZELLE_ERROR_CODE_CHECK(GZLL_RESULT)                                       \
    do                                                                              \
    {                                                                               \
        if((GZLL_RESULT) == false)                                                  \
        {                                                                           \
            nrf_gzll_error_code_t gzll_error_code = nrf_gzll_get_error_code();      \
            ret_code_t error_code = gzll_error_code + NRF_ERROR_GAZELLE_ERR_BASE;   \
            APP_ERROR_HANDLER(error_code);                                          \
        }                                                                           \
    } while(0)


#ifdef __cplusplus
}
#endif

#endif
