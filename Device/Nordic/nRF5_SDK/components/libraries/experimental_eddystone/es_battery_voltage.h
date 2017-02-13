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

#ifndef ES_BATTERY_VOLTAGE_H__
#define ES_BATTERY_VOLTAGE_H__

#include <stdint.h>

/**
 * @file
 *
 * @addtogroup eddystone_tlm
 * @{
 */

/**@brief Function for initializing the battery voltage module.
 */
void es_battery_voltage_init(void);


/**@brief Function for reading the battery voltage.
 *
 * @param[out]   p_vbatt       Pointer to the battery voltage value.
 */
void es_battery_voltage_get(uint16_t * p_vbatt);

/**
 * @}
 */

#endif // ES_BATTERY_VOLTAGE_H__
