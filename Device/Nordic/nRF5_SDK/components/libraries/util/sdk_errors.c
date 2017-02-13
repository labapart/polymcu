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
#include "sdk_errors.h"
#include "nordic_common.h"

#if defined(NRF_LOG_ENABLED) && NRF_LOG_ENABLED
/**
 * @defgroup err_strings Arrays with error codes as strings.
 * @{
 */
const char * m_sdk_errors_name[ERR_NAMES_SIZE] = 
{
    "NRF_SUCCESS",
    "NRF_ERROR_SVC_HANDLER_MISSING",
    "NRF_ERROR_SOFTDEVICE_NOT_ENABLED",
    "NRF_ERROR_INTERNAL",
    "NRF_ERROR_NO_MEM",
    "NRF_ERROR_NOT_FOUND",
    "NRF_ERROR_NOT_SUPPORTED",
    "NRF_ERROR_INVALID_PARAM",
    "NRF_ERROR_INVALID_STATE",
    "NRF_ERROR_INVALID_LENGTH",
    "NRF_ERROR_INVALID_FLAGS",
    "NRF_ERROR_INVALID_DATA",
    "NRF_ERROR_DATA_SIZE",
    "NRF_ERROR_TIMEOUT",
    "NRF_ERROR_NULL",
    "NRF_ERROR_FORBIDDEN",
    "NRF_ERROR_INVALID_ADDR",
    "NRF_ERROR_BUSY"
};

const char * m_sdk_errors_name_common[ERR_NAMES_COMMON_SIZE] =
{
    "NRF_ERROR_MODULE_NOT_INITIALZED",
    "NRF_ERROR_MUTEX_INIT_FAILED",
    "NRF_ERROR_MUTEX_LOCK_FAILED",
    "NRF_ERROR_MUTEX_UNLOCK_FAILED",
    "NRF_ERROR_MUTEX_COND_INIT_FAILED",
    "NRF_ERROR_MODULE_ALREADY_INITIALIZED",
    "NRF_ERROR_STORAGE_FULL",
    "NRF_ERROR_API_NOT_IMPLEMENTED",
    "NRF_ERROR_FEATURE_NOT_ENABLED"
};

const char * m_sdk_errors_name_twi[ERR_NAMES_TWI_SIZE] =
{
    "NRF_ERROR_DRV_TWI_ERR_OVERRUN",
    "NRF_ERROR_DRV_TWI_ERR_ANACK",
    "NRF_ERROR_DRV_TWI_ERR_DNACK"
};
/* @} */
#endif // NRF_LOG_ENABLED
