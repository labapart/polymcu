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

/** @file
 *
 * @defgroup nrf_ble_gatt GATT module
 * @{
 * @ingroup ble_sdk_lib
 * @brief Module for negotiating and keeping track of the maximum ATT_MTU size.
 */

#ifndef NRF_BLE_GATT_H__
#define NRF_BLE_GATT_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ble.h"
#include "app_util.h"
#include "sdk_errors.h"
#include "sdk_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief  Default maximum ATT_MTU size.
 *
 * This define specifies a fallback value for the case where 
 * NRF_BLE_GATT_MAX_MTU_SIZE is not set in the @ref nrf_ble_gatt_config.
 */
#ifndef NRF_BLE_GATT_MAX_MTU_SIZE
    #define NRF_BLE_GATT_MAX_MTU_SIZE GATT_MTU_SIZE_DEFAULT
#endif


/** @brief  Default maximum number of peripheral links.
 *
 * To override this default value, define the maximum number of peripheral
 * links in your module.
 */
#ifndef NRF_BLE_CENTRAL_LINK_COUNT
    #define NRF_BLE_CENTRAL_LINK_COUNT 1
#endif

/** @brief  Default maximum number of central links.
 *
 * To override this default value, define the maximum number of central
 * links in your module.
 */
#ifndef NRF_BLE_PERIPHERAL_LINK_COUNT
    #define NRF_BLE_PERIPHERAL_LINK_COUNT 1
#endif

/** @brief The maximum number of peripheral and central links combined. */
#define NRF_BLE_GATT_LINK_COUNT (NRF_BLE_PERIPHERAL_LINK_COUNT + NRF_BLE_CENTRAL_LINK_COUNT)


/**@brief GATT module event. */
typedef struct
{
    uint16_t conn_handle;                                      //!< Connection handle on which the event happened.
    uint16_t att_mtu_effective;                                //!< Effective MTU after the event.
}nrf_ble_gatt_evt_t;


// Forward declaration of the nrf_ble_gatt_t type.
typedef struct nrf_ble_gatt_s nrf_ble_gatt_t;

/**@brief GATT module event handler type.
 *
 * The GATT module calls a function of this type when the ATT_MTU value is changed.
*/
typedef void (*nrf_ble_gatt_evt_handler_t) (nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t * p_evt);

/**@brief GATT information for each link. */
typedef struct
{
    uint16_t att_mtu_desired;                                  //!< Requested ATT_MTU size for the link.
    uint16_t att_mtu_effective;                                //!< Effective ATT_MTU size for the link.
    bool     is_request_pending;                               //!< Flag that indicates if a request to extend the MTU size is pending for the link (if the call to @ref sd_ble_gattc_exchange_mtu_request returned @ref NRF_ERROR_BUSY).
}nrf_ble_gatt_link_t;


/**@brief GATT structure that contains status information for the GATT module. */
struct nrf_ble_gatt_s
{
    uint16_t                   att_mtu_desired_periph;                //!< Requested ATT_MTU size for the next peripheral link that is established.
    uint16_t                   att_mtu_desired_central;               //!< Requested ATT_MTU size for the next central link that is established.
    nrf_ble_gatt_link_t        links[NRF_BLE_GATT_LINK_COUNT];        //!< GATT related information for all active links.
    nrf_ble_gatt_evt_handler_t evt_handler;                           //!< GATT event handler.
};


/**@brief Function for initializing the GATT module.
 *
 * @param[in]    evt_handler   Event handler.
 * @param[out]   p_gatt        Pointer to the GATT structure.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_NULL          If the pointer to @p p_gatt is NULL.
 *
 */
ret_code_t nrf_ble_gatt_init(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_handler_t evt_handler);


/**@brief Function for setting the ATT_MTU size for the next link that is established as peripheral.
 *
 * @param[in]   p_gatt       Pointer to the GATT structure.
 * @param[in]   desired_mtu  Requested ATT_MTU size.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_NULL          If the pointer to @p p_gatt is NULL.
 * @retval NRF_ERROR_INVALID_PARAM If the size of @p desired_mtu is bigger than @ref NRF_BLE_GATT_MAX_MTU_SIZE or smaller than @ref GATT_MTU_SIZE_DEFAULT.
 *
 */
ret_code_t nrf_ble_gatt_att_mtu_periph_set(nrf_ble_gatt_t * p_gatt, uint16_t desired_mtu);


/**@brief Function for setting the ATT_MTU size for the next link that is established as central.
 *
 * @param[in]   p_gatt      Pointer to the GATT structure.
 * @param[in]   desired_mtu Requested ATT_MTU size.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_NULL          If the pointer to @p p_gatt is NULL.
 * @retval NRF_ERROR_INVALID_PARAM If the size of @p desired_mtu is bigger than @ref NRF_BLE_GATT_MAX_MTU_SIZE or smaller than @ref GATT_MTU_SIZE_DEFAULT.
 *
 */
ret_code_t nrf_ble_gatt_att_mtu_central_set(nrf_ble_gatt_t * p_gatt, uint16_t desired_mtu);


/**@brief Function for handling BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the GATT module.
 *
 * @param[in]  p_gatt     Pointer to the GATT structure.
 * @param[in]  p_ble_evt  Event received from the BLE stack.
 */
void nrf_ble_gatt_on_ble_evt(nrf_ble_gatt_t * p_gatt, ble_evt_t * p_ble_evt);


/**@brief Function for getting the current ATT_MTU size for a connection handle.
 *
 * @param[in]   p_gatt      Pointer to the GATT structure.
 * @param[in]   conn_handle Connection handle of the link.
 *
 * @return ATT_MTU size for the given link.
 * @retval 0       If the pointer to @p p_gatt is NULL or if @p conn_handle is larger than the supported maximum number of connections.
 *
 */
uint16_t nrf_ble_gatt_eff_mtu_get(nrf_ble_gatt_t * p_gatt, uint16_t conn_handle);

#ifdef __cplusplus
}
#endif

#endif // NRF_BLE_GATT_H__

/** @} */
