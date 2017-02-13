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

#ifndef ES_ADV_FRAME_H__
#define ES_ADV_FRAME_H__

#include <stdint.h>
#include "ble_advdata.h"

/**
 * @file
 * @addtogroup eddystone_adv
 * @{
 */

/**@brief Function for setting up connectable advertisement data using @ref ble_advdata_set.
 *
 * @param[in]   p_scrsp_data       Pointer to the scan response data.
 */
void es_adv_frame_fill_connectable_adv_data(ble_advdata_t * p_scrsp_data);

/**@brief Function for setting up non-connectable advertisement data using @ref ble_advdata_set.
 *
 * @param[in]   slot_no Slot to fill in data for.
 * @param[in]   etlm    Flag that specifies if Eddystone-TLM is required.
 */
void es_adv_frame_fill_non_connectable_adv_data(uint8_t slot_no, bool etlm);

/**
 * @}
 */

#endif // ES_ADV_FRAME_H__
