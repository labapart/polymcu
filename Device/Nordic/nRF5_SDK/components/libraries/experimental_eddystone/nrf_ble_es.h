#ifndef NRF_ES_H__
#define NRF_ES_H__

#include <stdint.h>
#include "ble.h"

/**
 * @file
 * @defgroup eddystone Eddystone library
 * @ingroup app_common
 * @{
 *
 * @brief   Library for Eddystone beacons. This library is used in the @ref ble_sdk_app_es. 
 *
 * @note    The API documentation is provided for reference only. You should 
 *          not modify this library, and you should not use any functions 
 *          except for the main level functions defined in @c nrf_ble_es.h 
 *          in different contexts. 
 */

/** @brief Eddystone event types. */
typedef enum {
  NRF_BLE_ES_EVT_ADVERTISEMENT_SENT,        //!< A non-connectable Eddystone frame advertisement was sent.
  NRF_BLE_ES_EVT_CONNECTABLE_ADV_STARTED,   //!< Advertising in connectable mode was started.
}nrf_ble_es_evt_t;

/**@brief Eddystone event handler type. */
typedef void (*nrf_ble_es_evt_handler_t)(nrf_ble_es_evt_t evt);

/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of 
 * interest to the Eddystone library. It must be called from the @ref 
 * softdevice_handler callback for all BLE events that are received from the 
 * SoftDevice.
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void nrf_ble_es_on_ble_evt(ble_evt_t * p_ble_evt);

/**@brief Function for putting the beacon in connectable mode.
 *
 * @details This function makes the beacon advertise connectable advertisements.
 *          If the beacon is in a connected state, the request is ignored.
 */
void nrf_ble_es_on_start_connectable_advertising(void);


/** @brief Function for initializing the Eddystone library. 
 *
 * @param[in]   evt_handler Event handler to be called for handling BLE events.
 */
void nrf_ble_es_init(nrf_ble_es_evt_handler_t evt_handler);

/**
 * @}
 */


#endif

