/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

#ifndef ANCS_APP_ATTR_GET_H__
#define ANCS_APP_ATTR_GET_H__

#include "nrf_ble_ancs_c.h"
/** @file
 *
 * @addtogroup ble_ancs_c
 * @{
 */

/**@brief Function for requesting attributes for an app.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 * @param[in] app_id  App identifier of the app for which to request app attributes.
 * @param[in] len     Length of the app identifier.
 *
 * @retval NRF_SUCCESS If all operations were successful. Otherwise, an error code is returned.
 */
uint32_t ancs_c_app_attr_request(ble_ancs_c_t  * p_ancs,
                                 const uint8_t * app_id,
                                 uint32_t        len);

/** @} */

#endif // ANCS_APP_ATTR_GET_H__

