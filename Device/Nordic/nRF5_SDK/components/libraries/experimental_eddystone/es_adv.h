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

#ifndef ES_ADV_H__
#define ES_ADV_H__

#include "es_slot.h"
#include "nrf_ble_escs.h"

/**
 * @file
 * @defgroup eddystone_adv Eddystone advertising module
 * @brief Types and functions for handling advertising in Eddystone beacons.
 * @ingroup eddystone
 * @{
 */

/** @brief Eddystone Advertiser events. */
typedef enum
{
    ES_ADV_EVT_NON_CONN_ADV,
    ES_ADV_EVT_CONNECTABLE_ADV_STARTED,
}es_adv_evt_t;

/** @brief Eddystone Advertiser event handler. */
typedef void (*es_adv_evt_handler_t)(es_adv_evt_t evt);

/** @brief Function for initializing the module.
 *
 * @param[in] ecs_uuid_type         ECS UUID type used for advertising the Eddystone Configuration Service UUID.
 * @param[in] adv_event_handler     Eddystone advertiser event handler.
 * @param[in] adv_interval          Advertisement interval to use.
 * @param[in] remain_connectable    Flag that specifies if advertisements should remain connectable.
 */
void es_adv_init(uint8_t                     ecs_uuid_type,
                 es_adv_evt_handler_t        adv_event_handler,
                 nrf_ble_escs_adv_interval_t adv_interval,
                 bool                        remain_connectable);


/** @brief Function for passing BLE events to this module.
 * @param[in] p_ble_evt     Pointer to the BLE evt.
 */
void es_adv_on_ble_evt(ble_evt_t * p_ble_evt);


/** @brief Function for starting the advertisements.
 */
void es_adv_start_non_connctable_adv(void);


/** @brief Function for specifying if the beacon should remain connectable.
 * @param[in] remain_connectable     Value to be set.
 */
void es_adv_remain_connectable_set(bool remain_connectable);


/** @brief Function for starting connectable advertisements.
 */
void es_adv_start_connectable_adv(void);


/** @brief Function for setting the base advertisement interval for non-connectable advertisements.
 *
 * @param interval The new advertisement interval.
 */
void es_adv_interval_set(nrf_ble_escs_adv_interval_t interval);


/** @brief Function for getting a pointer to the current advertisement interval.
 *
 * @retval Pointer to the current advertisement interval.
 */
nrf_ble_escs_adv_interval_t es_adv_interval_get(void);

/** @brief Function for getting the value of the 'remain_connectable' field.
 *
 * @retval Value of 'remain_connectable'.
 */
bool es_adv_remain_connectable_get(void);

/**
 * @}
 */

#endif // ES_ADV_H__
