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

#ifndef ES_GATTS_H__
#define ES_GATTS_H__

#include <stdint.h>
#include "nrf_ble_escs.h"

/**
 * @file
 * @defgroup eddystone_gatts GATTS
 * @brief Functions for handling GATTS write and read requests.
 * @ingroup eddystone
 * @{
 */

/**@brief Function for handling all write requests from the Central.
 *
 * @param[in]   p_escs      Pointer to the Eddystone Configuration Service.
 * @param[in]   evt_type    Type of event (corresponding to each characteristic in the service that is being written to).
 * @param[in]   val_handle  Value handle field of the characteristic handle of the characteristic that is being written to.
 * @param[in]   p_data      Pointer to the data to be written.
 * @param[in]   length      Length of the data to be written.
 *
 */
void es_gatts_write_evt_handler(nrf_ble_escs_t        * p_escs,
                                nrf_ble_escs_evt_type_t evt_type,
                                uint16_t                val_handle,
                                uint8_t               * p_data,
                                uint16_t                length);


/**@brief Function for handling all read requests from the Central.
 *
 * @param[in]   p_escs      Pointer to the Eddystone Configuration Service.
 * @param[in]   evt_type    Type of event (corresponding to each characteristic in the service that is being read from).
 * @param[in]   val_handle  Value handle field of the characteristic handle of the characteristic that is being read from.
 *
 */
void es_gatts_read_evt_handler(nrf_ble_escs_t        * p_escs,
                               nrf_ble_escs_evt_type_t evt_type,
                               uint16_t                val_handle);
/**
 * @}
 */

#endif // ES_GATTS_H__
