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

#ifndef NRF_BLE_ESCS_H__
#define NRF_BLE_ESCS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_platform.h"
#include "sdk_common.h"
#include "escs_defs.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @file
 * @defgroup nrf_ble_escs Eddystone Configuration Service
 * @brief Eddystone Configuration Service module.
 * @ingroup ble_sdk_srv
 * @{
 */

#define BLE_UUID_ESCS_SERVICE 0x7500    //!< UUID of the Eddystone Configuration Service.

/**@brief Data fields in the Broadcast Capabilities characteristic. 
 * @note This is a packed structure. Therefore, you should not change it.
 */
typedef PACKED_STRUCT
{
    int8_t  vers_byte;
    int8_t  max_supp_total_slots;
    int8_t  max_supp_eid_slots;
    int8_t  cap_bitfield;
    int16_t supp_frame_types;
    int8_t  supp_radio_tx_power[ESCS_NUM_OF_SUPPORTED_TX_POWER];
} nrf_ble_escs_broadcast_cap_t;

typedef uint8_t     nrf_ble_escs_active_slot_t;
typedef uint16_t    nrf_ble_escs_adv_interval_t;
typedef int8_t      nrf_ble_escs_radio_tx_pwr_t;
typedef int8_t      nrf_ble_escs_adv_tx_pwr_t;

/**@brief Read states of the Lock State characteristic. */
typedef enum
{
    NRF_BLE_ESCS_LOCK_STATE_LOCKED                        = ESCS_LOCK_STATE_LOCKED,
    NRF_BLE_ESCS_LOCK_STATE_UNLOCKED                      = ESCS_LOCK_STATE_UNLOCKED,
    NRF_BLE_ESCS_LOCK_STATE_UNLOCKED_AUTO_RELOCK_DISABLED =
        ESCS_LOCK_STATE_UNLOCKED_AUTO_RELOCK_DISABLED
} nrf_ble_escs_lock_state_read_t;

/**@brief Write bytes of the Lock State characteristic. */
typedef enum
{
    NRF_BLE_ESCS_LOCK_BYTE_LOCK                = ESCS_LOCK_BYTE_LOCK,
    NRF_BLE_ESCS_LOCK_BYTE_DISABLE_AUTO_RELOCK = ESCS_LOCK_BYTE_DISABLE_AUTO_RELOCK
} nrf_ble_escs_lock_byte_t;

/**@brief Write data fields of the Lock State characteristic. 
 * @note This is a packed structure. Therefore, you should not change it.
*/
typedef PACKED_STRUCT
{
    nrf_ble_escs_lock_byte_t lock_byte;
    int8_t                   encrypted_key[ESCS_AES_KEY_SIZE];
} nrf_ble_escs_lock_state_write_t;

/**@brief Lock State characteristic. */
typedef union
{
    nrf_ble_escs_lock_state_read_t  read;
    nrf_ble_escs_lock_state_write_t write;
} nrf_ble_escs_lock_state_t;

/**@brief Unlock characteristic (read/write). */
typedef union
{
    int8_t r_challenge[ESCS_AES_KEY_SIZE];
    int8_t w_unlock_token[ESCS_AES_KEY_SIZE];
} nrf_ble_escs_unlock_t;

/**@brief Public ECDH Key characteristic. 
 * @note This is a packed structure. Therefore, you should not change it.
*/
typedef PACKED_STRUCT
{
    int8_t key[ESCS_ECDH_KEY_SIZE];
} nrf_ble_escs_public_ecdh_key_t;

/**@brief EID Identity Key characteristic. 
 * @note This is a packed structure. Therefore, you should not change it.
*/
typedef PACKED_STRUCT
{
    int8_t key[ESCS_AES_KEY_SIZE];
} nrf_ble_escs_eid_id_key_t;


typedef uint8_t nrf_ble_escs_factory_reset_t;

/**@brief Unlock characteristic (read/write). */
typedef union
{
    uint8_t r_is_non_connectable_supported;
    uint8_t w_remain_connectable_boolean;
} nrf_ble_escs_remain_conntbl_t;

/**@brief Eddystone Configuration Service event types (corresponding to each characteristic). */
typedef enum
{
    NRF_BLE_ESCS_EVT_BROADCAST_CAP,
    NRF_BLE_ESCS_EVT_ACTIVE_SLOT,
    NRF_BLE_ESCS_EVT_ADV_INTERVAL,
    NRF_BLE_ESCS_EVT_RADIO_TX_PWR,
    NRF_BLE_ESCS_EVT_ADV_TX_PWR,
    NRF_BLE_ESCS_EVT_LOCK_STATE,
    NRF_BLE_ESCS_EVT_UNLOCK,
    NRF_BLE_ESCS_EVT_PUBLIC_ECDH_KEY,
    NRF_BLE_ESCS_EVT_EID_ID_KEY,
    NRF_BLE_ESCS_EVT_RW_ADV_SLOT,
    NRF_BLE_ESCS_EVT_RW_ADV_SLOT_PREP, //!< Used for longs writes for EID configuration.
    NRF_BLE_ESCS_EVT_RW_ADV_SLOT_EXEC, //!< Used for longs writes for EID configuration.
    NRF_BLE_ESCS_EVT_FACTORY_RESET,
    NRF_BLE_ESCS_EVT_REMAIN_CONNECTABLE
} nrf_ble_escs_evt_type_t;

/**@brief Eddystone Configuration Service initialization parameters (corresponding to required characteristics). */
typedef struct
{
    nrf_ble_escs_broadcast_cap_t  broadcast_cap;
    nrf_ble_escs_active_slot_t    active_slot;
    nrf_ble_escs_adv_interval_t   adv_interval;
    nrf_ble_escs_radio_tx_pwr_t   radio_tx_pwr;
    nrf_ble_escs_adv_tx_pwr_t     adv_tx_pwr;
    nrf_ble_escs_lock_state_t     lock_state;
    nrf_ble_escs_factory_reset_t  factory_reset;
    nrf_ble_escs_remain_conntbl_t remain_connectable;
} nrf_ble_escs_init_params_t;

// Forward Declaration of nrf_ble_escs_t type.
typedef struct nrf_ble_escs_s nrf_ble_escs_t;

typedef void (*nrf_ble_escs_write_evt_handler_t)(nrf_ble_escs_t        * p_escs,
                                                 nrf_ble_escs_evt_type_t evt_type,
                                                 uint16_t                value_handle,
                                                 uint8_t               * p_data,
                                                 uint16_t                length);

typedef void (*nrf_ble_escs_read_evt_handler_t)(nrf_ble_escs_t        * p_escs,
                                                nrf_ble_escs_evt_type_t evt_type,
                                                uint16_t                value_handle
                                               );

/**@brief Eddystone Configuration Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref nrf_ble_escs_init
 * function.
 */
typedef struct
{
    nrf_ble_escs_init_params_t     * p_init_vals;       //!< Initialization parameters for the service.
    nrf_ble_escs_write_evt_handler_t write_evt_handler; //!< Event handler to be called for authorizing write requests.
    nrf_ble_escs_read_evt_handler_t  read_evt_handler;  //!< Event handler to be called for authorizing read requests.
} nrf_ble_escs_init_t;

struct nrf_ble_escs_s
{
    uint8_t                          uuid_type;                  //!< UUID type for the Eddystone Configuration Service Base UUID.
    uint16_t                         service_handle;             //!< Handle of the Eddystone Configuration Service (as provided by the SoftDevice).
    ble_gatts_char_handles_t         broadcast_cap_handles;      //!< Handles related to the Capabilities characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         active_slot_handles;        //!< Handles related to the Active Slot characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         adv_interval_handles;       //!< Handles related to the Advertising Interval characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         radio_tx_pwr_handles;       //!< Handles related to the Radio Tx Power characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         adv_tx_pwr_handles;         //!< Handles related to the (Advanced) Advertised Tx Power characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         lock_state_handles;         //!< Handles related to the Lock State characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         unlock_handles;             //!< Handles related to the Unlock characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         pub_ecdh_key_handles;       //!< Handles related to the Public ECDH Key characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         eid_id_key_handles;         //!< Handles related to the EID Identity Key characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         rw_adv_slot_handles;        //!< Handles related to the ADV Slot Data characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         factory_reset_handles;      //!< Handles related to the (Advanced) Factory reset characteristic (as provided by the SoftDevice).
    ble_gatts_char_handles_t         remain_connectable_handles; //!< Handles related to the (Advanced) Remain Connectable characteristic (as provided by the SoftDevice).
    uint16_t                         conn_handle;                //!< Handle of the current connection (as provided by the SoftDevice). @ref BLE_CONN_HANDLE_INVALID if not in a connection.
    nrf_ble_escs_write_evt_handler_t write_evt_handler;          //!< Event handler to be called for handling write attempts.
    nrf_ble_escs_read_evt_handler_t  read_evt_handler;           //!< Event handler to be called for handling read attempts.
};

/**@brief Function for initializing the Eddystone Configuration Service.
 *
 * @param[out] p_escs     Eddystone Configuration Service structure. This structure must be supplied
 *                        by the application. It is initialized by this function and will
 *                        later be used to identify this particular service instance.
 * @param[in] p_ecs_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS    If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval NRF_ERROR_NULL If either of the pointers @p p_escs or @p p_ecs_init is NULL.
 */
ret_code_t nrf_ble_escs_init(nrf_ble_escs_t * p_escs, const nrf_ble_escs_init_t * p_ecs_init);

/**@brief Function for handling the Eddystone Configuration Service's BLE events.
 *
 * @details The Eddystone Configuration Service expects the application to call this function each time an
 * event is received from the SoftDevice. This function processes the event if it
 * is relevant and calls the Eddystone Configuration Service event handler of the
 * application if necessary.
 *
 * @param[in] p_escs      Eddystone Configuration Service structure.
 * @param[in] p_ble_evt   Event received from the SoftDevice.
 *
 * @retval                NRF_ERROR_NULL If any of the arguments given are NULL.
 * @retval                NRF_SUCCESS otherwise.
 */
ret_code_t nrf_ble_escs_on_ble_evt(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt);


/**@brief Function for checking if a beacon is locked.
 *
 * @param[in]  p_escs       Eddystone Configuration Service structure.
 * @param[out] p_lock_state Pointer to where result will be stored.
 *
 * @retval                  NRF_SUCCESS If the operation was successful.
 * @retval                  NRF_ERROR_NULL If p_escs is NULL.
 * @return                  See @ref sd_ble_gatts_value_get for other possible return values.
  */
ret_code_t nrf_ble_escs_get_lock_state(const nrf_ble_escs_t * p_escs, nrf_ble_escs_lock_state_read_t * p_lock_state);


/**@brief Function for getting the current active slot.
 * 
 * @param[in]   p_escs          Eddystone Configuration Service structure.
 * @param[out]  p_active_slot   Pointer where resulting value will be stored.
 *
 * @retval                      NRF_SUCCESS If the operation was successful.
 * @retval                      NRF_ERROR_NULL If p_escs is NULL.
 * @return                      See @ref sd_ble_gatts_value_get for other possible return values.
 */
ret_code_t nrf_ble_escs_active_slot_get(const nrf_ble_escs_t * p_escs, uint8_t * p_active_slot);

/** @} */

#endif // NRF_BLE_ESCS_H__
