#include "nrf_ble_es.h"
#include <string.h>
#include "es_slot.h"
#include "es_battery_voltage.h"
#include "es_flash.h"
#include "app_error.h"
#include "escs_defs.h"
#include "es_adv.h"
#include "es_security.h"
#include "es_gatts.h"

#define NRF_LOG_MODULE_NAME "NRF_BLE_ES"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

static uint16_t                       m_conn_handle = BLE_CONN_HANDLE_INVALID;  //!< Connection handle.
static nrf_ble_escs_t                 m_ble_ecs;                                //!< Struct identifying the Eddystone Config Service.
static nrf_ble_es_evt_handler_t       m_evt_handler;                            //!< Event handler.


/**@brief Function for invoking registered callback.  
 *
 * @param[in] evt Event to issue to callback.
 */
static void handle_evt(nrf_ble_es_evt_t evt)
{
    if (m_evt_handler != NULL)
    {
        m_evt_handler(evt);
    }
}


/**@brief Function resetting MAC address. */
static void new_address_set(void)
{
    
    uint32_t        err_code;
    uint8_t         bytes_available;
    ble_gap_addr_t  new_address;

    new_address.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
    
    // Randomize the MAC address on every EID generation
    (void)sd_rand_application_bytes_available_get(&bytes_available);

    while (bytes_available < BLE_GAP_ADDR_LEN)
    {
        // wait for SD to acquire enough RNs
        (void)sd_rand_application_bytes_available_get(&bytes_available);
    }

    (void)sd_rand_application_vector_get(new_address.addr, BLE_GAP_ADDR_LEN);

#if NRF_SD_BLE_API_VERSION == 3
    err_code = sd_ble_gap_addr_set(&new_address);
#else
    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &new_address);
#endif // NRF_SD_BLE_API_VERSION
    APP_ERROR_CHECK(err_code);
}


/**@brief Function updating MAC address if required.
 *
 * @param[in] demand_new_mac If 'true', mac address will be updated on next invocation when not connected.
 *                           If 'false', simply check if we have an outstanding demand for new MAC and update if not connected.
 */
static void check_and_update_mac_address(bool demand_new_mac)
{
    static bool    reset_mac_address = false;
    
    if (demand_new_mac)
    {
        reset_mac_address = true;
    }

    // Not possible to update MAC address while in a connection
    if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        return;
    }

    else if (reset_mac_address)
    {
        reset_mac_address = false;

        new_address_set();
    }
}


/**@brief Function to lock the beacon (change lock state characteristic to LOCKED)
 */
static void lock_beacon(void)
{
    ret_code_t err_code;
    nrf_ble_escs_lock_state_read_t unlock = NRF_BLE_ESCS_LOCK_STATE_LOCKED;
    ble_gatts_value_t   value = {.len       = sizeof(nrf_ble_escs_lock_state_read_t), 
                                 .offset    = 0, 
                                 .p_value   = (uint8_t *)&unlock};
    
    err_code = sd_ble_gatts_value_set(m_conn_handle,
                                      m_ble_ecs.lock_state_handles.value_handle,
                                      &value);
   APP_ERROR_CHECK(err_code);
}



/**@brief Function for handling BLE event from the SoftDevice.
 *
 * @param[in] p_ble_evt Pointer to BLE event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                        err_code;
    nrf_ble_escs_lock_state_read_t  lock_state;
    es_flash_flags_t                flash_flag    = {{0}};
    const es_slot_reg_t           * p_reg         = es_slot_get_registry();

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.common_evt.conn_handle;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;

            for (uint8_t i = 0; i < APP_MAX_ADV_SLOTS; i++)
            {
                err_code = es_slot_write_to_flash(i);
                APP_ERROR_CHECK(err_code);

                flash_flag.slot_is_empty[i] = !p_reg->slots[i].configured;
            }

            err_code = es_flash_access_flags(&flash_flag, ES_FLASH_ACCESS_WRITE);
            APP_ERROR_CHECK(err_code);

            es_flash_beacon_config_t beacon_config;
            beacon_config.adv_interval       = es_adv_interval_get();
            beacon_config.remain_connectable = es_adv_remain_connectable_get();

            err_code = es_flash_access_beacon_config(&beacon_config, ES_FLASH_ACCESS_WRITE);
            APP_ERROR_CHECK(err_code);

            // Check if the beacon should be locked on disconnection.
            err_code = nrf_ble_escs_get_lock_state(&m_ble_ecs, &lock_state);
            APP_ERROR_CHECK(err_code);
            
            if (lock_state == NRF_BLE_ESCS_LOCK_STATE_UNLOCKED)
            {
                lock_beacon();
            }

            check_and_update_mac_address(false);

            break;

        default:
            // No implementation needed.
            break;
    }
}



/**@brief Callback function to receive messages from the security module
 *
 * @details Need to be passed in during es_security_init(). The security
 *          module will callback anytime a particular security process is completed
 *
 * @params[in]  slot_no     Index of the slot
 * @params[in]  msg_type    Message type corersponding to different security components
 */
static void nrf_ble_escs_security_cb(uint8_t           slot_no,
                                     es_security_msg_t msg_type)
{
    nrf_ble_escs_eid_id_key_t      encrypted_id_key;
    nrf_ble_escs_public_ecdh_key_t pub_ecdh_key;

    ret_code_t                     err_code;
    static ble_gatts_value_t       value;
    nrf_ble_escs_lock_state_read_t unlock = NRF_BLE_ESCS_LOCK_STATE_UNLOCKED;

    switch (msg_type)
    {
        case ES_SECURITY_MSG_UNLOCKED:
            NRF_LOG_INFO("Beacon has been unlocked.\r\n");
            value.len     = sizeof(nrf_ble_escs_lock_state_read_t);
            value.offset  = 0;
            value.p_value = (uint8_t *)&unlock;

            err_code = sd_ble_gatts_value_set(m_ble_ecs.conn_handle,
                                              m_ble_ecs.lock_state_handles.value_handle,
                                              &value);
            APP_ERROR_CHECK(err_code);
            break;

        case ES_SECURITY_MSG_EID:
            NRF_LOG_INFO("New EID generated.\r\n");
            es_slot_eid_ready(slot_no);
            #ifdef MAC_RANDOMIZED
            check_and_update_mac_address(true);
            #endif // MAC_RANDOMIZED
            break;

        case ES_SECURITY_MSG_IK:
            NRF_LOG_INFO("Encrypted Identity Key Ready!\r\n");
            es_security_encrypted_eid_id_key_get(slot_no, (uint8_t *)encrypted_id_key.key);
            // Set the EID ID key in the slot so it can be exposed in the characteristic
            es_slot_encrypted_eid_id_key_set(slot_no, &encrypted_id_key);
            break;

        case ES_SECURITY_MSG_ECDH:
            NRF_LOG_INFO("ECDH Pub Key Ready!\r\n");
            es_security_pub_ecdh_get(slot_no, (uint8_t *)pub_ecdh_key.key);

            // Set the characteristic to the ECDH key value
            value.len     = sizeof(nrf_ble_escs_public_ecdh_key_t);
            value.offset  = 0,
            value.p_value = (uint8_t *)pub_ecdh_key.key;
            
            if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
            {
                err_code = sd_ble_gatts_value_set(m_ble_ecs.conn_handle,
                                                  m_ble_ecs.pub_ecdh_key_handles.value_handle,
                                                  &value);
                if (err_code != NRF_SUCCESS)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;

        case ES_SECURITY_MSG_STORE_TIME:
            // Every 24 hours any EID slots time is stored to flash to allow for power lock_state_handles
            // recovery. Only time needs to be stored, but just store the entire slot anyway for API simplicity.
            NRF_LOG_INFO("Storing EID Time!\r\n");
            err_code = es_slot_write_to_flash(slot_no);
            APP_ERROR_CHECK(err_code);

            break;

        default:
            APP_ERROR_CHECK(NRF_ERROR_INVALID_PARAM); // Should never happen
            break;
    }
}


/**@brief Function for handling advertisement events from 'es_adv'.
 *
 * @param[in] evt Advertisement event to handle.
 */
static void adv_evt_handler(es_adv_evt_t evt)
{
    switch (evt)
    {
        case ES_ADV_EVT_NON_CONN_ADV:
            handle_evt(NRF_BLE_ES_EVT_ADVERTISEMENT_SENT);
            es_security_update_time();
            break;

        case ES_ADV_EVT_CONNECTABLE_ADV_STARTED:
            handle_evt(NRF_BLE_ES_EVT_CONNECTABLE_ADV_STARTED);
            break;
    }
}


/**@brief Initialize the ECS with initial values for the characteristics and other necessary modules */
static void ble_escs_init(void)
{
    ret_code_t                 err_code;
    nrf_ble_escs_init_t        ecs_init;
    nrf_ble_escs_init_params_t init_params;
    int8_t                     tx_powers[ESCS_NUM_OF_SUPPORTED_TX_POWER] = ESCS_SUPPORTED_TX_POWER;

    /*Init the broadcast capabilities characteristic*/
    memset(&init_params.broadcast_cap, 0, sizeof(init_params.broadcast_cap));
    init_params.broadcast_cap.vers_byte            = ES_SPEC_VERSION_BYTE;
    init_params.broadcast_cap.max_supp_total_slots = APP_MAX_ADV_SLOTS;
    init_params.broadcast_cap.max_supp_eid_slots   = APP_MAX_EID_SLOTS;
    init_params.broadcast_cap.cap_bitfield         = ( (APP_IS_VARIABLE_ADV_SUPPORTED << ESCS_BROADCAST_VAR_ADV_SUPPORTED_Pos)
                                                   | (APP_IS_VARIABLE_TX_POWER_SUPPORTED << ESCS_BROADCAST_VAR_TX_POWER_SUPPORTED_Pos))
                                                   & (ESCS_BROADCAST_VAR_RFU_MASK);
    init_params.broadcast_cap.supp_frame_types     = ( (APP_IS_URL_SUPPORTED << ESCS_FRAME_TYPE_URL_SUPPORTED_Pos)
                                                   | (APP_IS_UID_SUPPORTED << ESCS_FRAME_TYPE_UID_SUPPORTED_Pos)
                                                   | (APP_IS_TLM_SUPPORTED << ESCS_FRAME_TYPE_TLM_SUPPORTED_Pos)
                                                   | (APP_IS_EID_SUPPORTED << ESCS_FRAME_TYPE_EID_SUPPORTED_Pos))
                                                   & (ESCS_FRAME_TYPE_RFU_MASK);
    memcpy(init_params.broadcast_cap.supp_radio_tx_power, tx_powers, ESCS_NUM_OF_SUPPORTED_TX_POWER);

    init_params.active_slot     = 0;
    init_params.adv_interval    = APP_CFG_NON_CONN_ADV_INTERVAL_MS;
    init_params.adv_tx_pwr      = APP_CFG_DEFAULT_RADIO_TX_POWER;
    init_params.radio_tx_pwr    = 0x00;
    init_params.lock_state.read = NRF_BLE_ESCS_LOCK_STATE_LOCKED;
    init_params.factory_reset   = 0;
    init_params.remain_connectable.r_is_non_connectable_supported = APP_IS_REMAIN_CONNECTABLE_SUPPORTED;

    // Initialize evt handlers and the service
    memset(&ecs_init, 0, sizeof(ecs_init));
    ecs_init.write_evt_handler = es_gatts_write_evt_handler;
    ecs_init.read_evt_handler  = es_gatts_read_evt_handler;
    ecs_init.p_init_vals       = &(init_params);

    err_code = nrf_ble_escs_init(&m_ble_ecs, &ecs_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing 'es_adv' module. */
static void adv_init(void)
{
    ret_code_t               err_code;
    es_flash_beacon_config_t beacon_config;

    err_code = es_flash_access_beacon_config(&beacon_config, ES_FLASH_ACCESS_READ);

    if (err_code == FDS_ERR_NOT_FOUND)
    {
        beacon_config.adv_interval       = APP_CFG_NON_CONN_ADV_INTERVAL_MS;
        beacon_config.remain_connectable = false;
    }
    else
    {
        APP_ERROR_CHECK(err_code);
    }

    es_adv_init(m_ble_ecs.uuid_type,
                adv_evt_handler,
                beacon_config.adv_interval,
                beacon_config.remain_connectable);
}


/**@brief Function for initializing es_slots module. */
static void adv_slots_init(void)
{
    uint8_t default_frame_data[DEFAULT_FRAME_LENGTH] = DEFAULT_FRAME_DATA;

    es_slot_t default_adv_slot = {
        .slot_no             = 0,
        .radio_tx_pwr        = 0,
        .adv_frame.type      = DEFAULT_FRAME_TYPE,
        .adv_frame.length    = DEFAULT_FRAME_LENGTH,
        .adv_custom_tx_power = false,
        .configured          = true
    };

    memcpy(&default_adv_slot.adv_frame.frame, default_frame_data, DEFAULT_FRAME_LENGTH);

    es_slots_init(&default_adv_slot);
}


void nrf_ble_es_on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;
    
    es_adv_on_ble_evt(p_ble_evt);
    err_code = nrf_ble_escs_on_ble_evt(&m_ble_ecs, p_ble_evt);
    APP_ERROR_CHECK(err_code);
    on_ble_evt(p_ble_evt);
    es_flash_on_ble_evt(p_ble_evt);
}


void nrf_ble_es_on_start_connectable_advertising(void)
{
    es_adv_start_connectable_adv();
}


void nrf_ble_es_init(nrf_ble_es_evt_handler_t evt_handler)
{
    uint32_t err_code;

    m_evt_handler = evt_handler;
    m_conn_handle = BLE_CONN_HANDLE_INVALID;
    
    err_code = es_flash_init();
    APP_ERROR_CHECK(err_code);

    while (es_flash_num_pending_ops() > 0)
    {
        ; // Busy wait while initialization of FDS module completes
    }

    err_code = es_security_init(nrf_ble_escs_security_cb);
    APP_ERROR_CHECK(err_code);

    ble_escs_init();
    adv_slots_init();
    es_battery_voltage_init();
    adv_init();
    es_adv_start_non_connctable_adv();
}


