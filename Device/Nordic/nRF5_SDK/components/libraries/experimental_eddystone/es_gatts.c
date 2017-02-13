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

#include "es_gatts.h"
#include "es_app_config.h"
#include "es_security.h"
#include "es_tlm.h"
#include "es_flash.h"
#include "es_slot.h"
#include "es_adv.h"

#define NRF_LOG_MODULE_NAME "EBLEH"
#include "nrf_log.h"

/**@brief Function checking if length of event is correct, given the frame data.
 *
 * @param[in] p_data Written ADV Slot data.
 * @param[in] length Written length.
 *
 * @retval true If length is valid.
 * @retval false If length is not valid.
 */
static bool length_is_valid(uint8_t * p_data, uint8_t length)
{
    if(length == 0 || (length == 1 && p_data[0] == 0))
    {
        return true;
    }
    
    else
    {
        switch ((es_frame_type_t)p_data[0])
        {
            case ES_FRAME_TYPE_UID:
                return length == ESCS_UID_WRITE_LENGTH;
                
            case ES_FRAME_TYPE_URL:
                return ((length >= 4) && (length <= ESCS_URL_WRITE_LENGTH));

            case ES_FRAME_TYPE_TLM:
                return (length == ESCS_TLM_WRITE_LENGTH);
                
            case ES_FRAME_TYPE_EID:
                return ((length == ESCS_EID_WRITE_ECDH_LENGTH) || (length == ESCS_EID_WRITE_IDK_LENGTH));
            
            default:
                // Fail as all enum values are handled above.
                APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE);
                return false;
        }
    }
}


/**@brief Function checking if beacon is unlocked.
 *
 * @param[in] p_escs Pointer to Eddystone Configuration Service.
 *
 * @retval true  If beacon is unlocked.
 * @retval false If beacon is locked.
 */
static bool is_beacon_unlocked(const nrf_ble_escs_t * p_escs)
{
    uint32_t                        err_code;
    nrf_ble_escs_lock_state_read_t  lock_state;
    
    err_code = nrf_ble_escs_get_lock_state(p_escs, &lock_state);
    APP_ERROR_CHECK(err_code);
    
    return lock_state == NRF_BLE_ESCS_LOCK_STATE_UNLOCKED;
}


void es_gatts_write_evt_handler(nrf_ble_escs_t        * p_escs,
                                nrf_ble_escs_evt_type_t evt_type,
                                uint16_t                val_handle,
                                uint8_t               * p_data,
                                uint16_t                length)
{
    ret_code_t                            err_code;
    ble_gatts_rw_authorize_reply_params_t reply                     = {0};
    bool                                  long_write_overwrite_flag = false; // Used for handling ECDH key long writes.
    bool                                  unlocked                  = is_beacon_unlocked(p_escs);
        

    reply.type                = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
    reply.params.write.update = 1;
    reply.params.write.offset = 0;

    
    // Allow the write request when unlocked, otherwise deny access.
    if (unlocked && (evt_type != NRF_BLE_ESCS_EVT_UNLOCK))
    {
        uint8_t slot_no;
        
        err_code = nrf_ble_escs_active_slot_get(p_escs, &slot_no);
        APP_ERROR_CHECK(err_code);

        reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;

        // Used in lock state case.
        uint8_t           value_buffer[ESCS_ADV_SLOT_CHAR_LENGTH_MAX] = {0};
        ble_gatts_value_t value =
        {
            .len = sizeof(value_buffer),
            .offset = 0,
            .p_value = &(value_buffer[0])
        };

        switch (evt_type)
        {
            case NRF_BLE_ESCS_EVT_ACTIVE_SLOT:
                // Boundary checking.
                if (*p_data > APP_MAX_ADV_SLOTS - 1)
                {
                    reply.params.write.gatt_status = BLE_GATT_STATUS_ATTERR_INVALID_ATT_VAL_LENGTH;
                    length                         = 0;
                }
                break;

            case NRF_BLE_ESCS_EVT_ADV_INTERVAL:
                es_adv_interval_set(BYTES_SWAP_16BIT(*(nrf_ble_escs_adv_interval_t *)p_data));
                break;

            case NRF_BLE_ESCS_EVT_RADIO_TX_PWR:
                es_slot_radio_tx_pwr_set(slot_no, *(nrf_ble_escs_radio_tx_pwr_t *)(p_data));
                break;

            case NRF_BLE_ESCS_EVT_ADV_TX_PWR:
                // Update slot info so that ADV data will only be read from what is written by client.
                es_slot_set_adv_custom_tx_power(slot_no, *(nrf_ble_escs_radio_tx_pwr_t *)(p_data));
                break;

            case NRF_BLE_ESCS_EVT_LOCK_STATE:
                if (length == 1 &&
                    (*p_data == NRF_BLE_ESCS_LOCK_BYTE_LOCK || *p_data ==
                     NRF_BLE_ESCS_LOCK_BYTE_DISABLE_AUTO_RELOCK))
                {
                    // Do nothing special, allow the write.
                }
                else if (length == sizeof(nrf_ble_escs_lock_state_write_t))
                {
                    // 0x00 + key[16] : transition to lock state and update the lock code.
                    (void)es_security_lock_code_update((p_data) + 1);
                    // Only write the lock byte (0x00) to the characteristic, so set length to 1.
                    length = 1;
                }
                else
                {
                    // Any invalid values locks the characteristic by default.
                    *p_data = NRF_BLE_ESCS_LOCK_BYTE_LOCK;
                }
                break;

            case NRF_BLE_ESCS_EVT_RW_ADV_SLOT:
                reply.params.read.gatt_status = length_is_valid(p_data, length) ? \
                                                    BLE_GATT_STATUS_SUCCESS : \
                                                    BLE_GATT_STATUS_ATTERR_INVALID_ATT_VAL_LENGTH;
                if (reply.params.read.gatt_status == BLE_GATT_STATUS_SUCCESS)
                {
                    es_slot_on_write(slot_no, length, p_data);
                }
                break;

            // Long writes to the RW ADV Slot characteristic for configuring an EID frame
            // with an ECDH key exchange
            case NRF_BLE_ESCS_EVT_RW_ADV_SLOT_PREP:
                long_write_overwrite_flag = true;

                reply.type                     = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
                reply.params.write.update      = 0;
                reply.params.write.offset      = 0;
                reply.params.write.len         = length;
                reply.params.write.p_data      = NULL;

                err_code = sd_ble_gatts_rw_authorize_reply(p_escs->conn_handle, &reply);
                APP_ERROR_CHECK(err_code);
                break;

            // Long writes to the RW ADV Slot characteristic for configuring an EID frame
            // with an ECDH key exchange.
            case NRF_BLE_ESCS_EVT_RW_ADV_SLOT_EXEC:
                long_write_overwrite_flag = true;

                reply.type                     = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
                reply.params.write.update      = 0;
                reply.params.write.offset      = 0;
                reply.params.write.len         = length;
                reply.params.write.p_data      = NULL;

                err_code = sd_ble_gatts_rw_authorize_reply(p_escs->conn_handle, &reply);
                APP_ERROR_CHECK(err_code);

                err_code = sd_ble_gatts_value_get(p_escs->conn_handle,
                                                  p_escs->rw_adv_slot_handles.value_handle,
                                                  &value);
                APP_ERROR_CHECK(err_code);

                es_slot_on_write(slot_no, value.len, value.p_value);
                break;

            case NRF_BLE_ESCS_EVT_FACTORY_RESET:
                if (*p_data == 0x0B)
                {
                    err_code = es_flash_factory_reset();
                    APP_ERROR_CHECK(err_code);
                }
                break;

            case NRF_BLE_ESCS_EVT_REMAIN_CONNECTABLE:
            #if APP_IS_REMAIN_CONNECTABLE_SUPPORTED == ESCS_FUNCT_REMAIN_CONNECTABLE_SUPPORTED_Yes
                if (*p_data != 0)
                {
                    es_adv_remain_connectable_set(true);
                }

                else
                {
                    es_adv_remain_connectable_set(false);
                }
            #endif
                break;

            default:
                break;
        }
        reply.params.write.len    = length;
        reply.params.write.p_data = p_data;
    }

    // When the beacon is locked and the client is trying to access characteristics other than
    // the lock characteristic, or when the beacon is unlocked and the client is trying to access
    // the unlock characteristic, deny access.
    else if ((!unlocked && (evt_type != NRF_BLE_ESCS_EVT_UNLOCK))
             || (unlocked && (evt_type == NRF_BLE_ESCS_EVT_UNLOCK)))
    {
        reply.params.write.gatt_status = BLE_GATT_STATUS_ATTERR_WRITE_NOT_PERMITTED;
        reply.params.write.update      = 1;
        reply.params.write.offset      = 0;
        reply.params.write.len         = 0;
        reply.params.write.p_data      = NULL;
    }

    // When the beacon is locked and the client is trying to access the unlock
    // characteristic accept the write and call the crypto functions to check the validity.
    else if (!unlocked && (evt_type == NRF_BLE_ESCS_EVT_UNLOCK))
    {
        uint8_t value_buffer[ESCS_AES_KEY_SIZE] = {0};
        memcpy(value_buffer, p_data, length);

        ble_gatts_value_t value = {.len = length, .offset = 0, .p_value = &(value_buffer[0])};

        err_code = sd_ble_gatts_value_set(p_escs->conn_handle, val_handle, &value);
        if (err_code == NRF_SUCCESS)
        {
            reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
            es_security_unlock_verify((value.p_value));
        }
        else
        {
            NRF_LOG_ERROR("Ble Gatts Value Set Error\r\n");
            reply.params.write.gatt_status = BLE_GATT_STATUS_ATTERR_WRITE_NOT_PERMITTED;
        }
        reply.params.write.len    = length;
        reply.params.write.p_data = (const uint8_t *)value.p_value;
    }
    else
    {
        reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
    }

    if (p_escs->conn_handle != BLE_CONN_HANDLE_INVALID && !long_write_overwrite_flag )
    {
        err_code = sd_ble_gatts_rw_authorize_reply(p_escs->conn_handle, &reply);
        APP_ERROR_CHECK(err_code);
    }
}


void es_gatts_read_evt_handler(nrf_ble_escs_t        * p_escs,
                               nrf_ble_escs_evt_type_t evt_type,
                               uint16_t                val_handle)
{
    ret_code_t                            err_code;
    ble_gatts_rw_authorize_reply_params_t reply     = {0};
    const es_slot_reg_t                 * p_reg     = es_slot_get_registry();
    bool                                  unlocked  = is_beacon_unlocked(p_escs);
    uint8_t                               eid_buf[14];
    

    reply.type               = BLE_GATTS_AUTHORIZE_TYPE_READ;
    reply.params.read.update = 1;
    reply.params.read.offset = 0;

    // Lock state can be read regardless the beacon's lock state
    if (evt_type == NRF_BLE_ESCS_EVT_LOCK_STATE)
    {
        uint8_t           value_buffer[1] = {0};
        ble_gatts_value_t value           =
        {.len = sizeof(value_buffer), .offset = 0, .p_value = &(value_buffer[0])};

        err_code = sd_ble_gatts_value_get(p_escs->conn_handle, val_handle, &value);
        APP_ERROR_CHECK(err_code);

        reply.params.read.len    = value.len;
        reply.params.read.p_data = (const uint8_t *)value.p_value;
    }

    else if (evt_type == NRF_BLE_ESCS_EVT_REMAIN_CONNECTABLE)
    {
        uint8_t retval = APP_IS_REMAIN_CONNECTABLE_SUPPORTED;
        reply.params.read.len         = 1;
        reply.params.read.p_data      = &retval;
        reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
    }

    else if (unlocked && (evt_type != NRF_BLE_ESCS_EVT_UNLOCK))
    {
        bool override_flag = false; // When true, overrides a direct read of the characteristic:
                                    // used for per slot properties.
        uint8_t slot_no;
        
        err_code = nrf_ble_escs_active_slot_get(p_escs, &slot_no);
        APP_ERROR_CHECK(err_code);
        
        NRF_LOG_INFO("ACTIVE SLOT: %d\r\n", slot_no);

        switch (evt_type)
        {
            case NRF_BLE_ESCS_EVT_BROADCAST_CAP:
                // Go straight to the characteristic.
                break;

            case NRF_BLE_ESCS_EVT_ACTIVE_SLOT:
                // Go straight to the characteristic
                break;

            case NRF_BLE_ESCS_EVT_ADV_INTERVAL:
            {
                nrf_ble_escs_adv_interval_t adv_interval = es_adv_interval_get();
                override_flag                 = true;
                reply.params.read.p_data      = (const uint8_t *) &adv_interval;
                reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
                break;
            }

            case NRF_BLE_ESCS_EVT_RADIO_TX_PWR:
                override_flag            = true;
                reply.params.read.len    = sizeof(nrf_ble_escs_radio_tx_pwr_t);
                reply.params.read.p_data =
                    (const uint8_t *)(&p_reg->slots[slot_no].radio_tx_pwr);
                reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
                break;

            case NRF_BLE_ESCS_EVT_ADV_TX_PWR:
                override_flag            = true;
                reply.params.read.len    = sizeof(nrf_ble_escs_radio_tx_pwr_t);
                reply.params.read.p_data = p_reg->slots[slot_no].adv_custom_tx_power ? \
                                           (const uint8_t *)(&p_reg->slots[slot_no].
                                                             custom_tx_power) : \
                                           (const uint8_t *)(&p_reg->slots[slot_no].radio_tx_pwr);

                reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
                break;

            case NRF_BLE_ESCS_EVT_UNLOCK:
                // Go straight to the characteristic.
                break;

            case NRF_BLE_ESCS_EVT_PUBLIC_ECDH_KEY:
                // Go straight to the characteristic
                break;

            case NRF_BLE_ESCS_EVT_EID_ID_KEY:
                override_flag = true;
                if (p_reg->slots[slot_no].configured && (p_reg->slots[slot_no].adv_frame.type == ES_FRAME_TYPE_EID))
                {
                    nrf_ble_escs_eid_id_key_t eid_id_key;
                    memcpy(&eid_id_key, &p_reg->slots[slot_no].encrypted_eid_id_key, sizeof(eid_id_key));
                    reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
                    reply.params.read.len         = sizeof(nrf_ble_escs_eid_id_key_t);
                    reply.params.read.p_data      = (uint8_t *)eid_id_key.key;
                }
                else
                {
                    reply.params.read.gatt_status = BLE_GATT_STATUS_ATTERR_READ_NOT_PERMITTED;
                    reply.params.read.update      = 1;
                    reply.params.read.offset      = 0;
                    reply.params.read.len         = 0;
                    reply.params.read.p_data      = NULL;
                }
                break;

            case NRF_BLE_ESCS_EVT_RW_ADV_SLOT:
                override_flag = true;

                // If an EID slot is read, load scaler, clock value and ephemeral ID.
                if (p_reg->slots[slot_no].adv_frame.type == ES_FRAME_TYPE_EID)
                {
                    /*lint -save -e666 */
                    uint32_t clock_value = BYTES_REVERSE_32BIT(es_security_clock_get(slot_no));
                    /*lint -restore */

                    reply.params.read.len = ES_EID_GATTS_READ_LENGTH;
                    
                    // Fill EID buffer with data
                    eid_buf[ES_EID_GATTS_READ_FRAME_TYPE_IDX] = ES_FRAME_TYPE_EID;
                    eid_buf[ES_EID_GATTS_READ_EXPONENT_IDX] = es_security_scaler_get(slot_no);
                    
                    memcpy(&eid_buf[ES_EID_GATTS_READ_CLCK_VALUE_IDX], 
                           &clock_value, 
                           sizeof(clock_value));
                    /*lint -save -e545 */
                    memcpy(&eid_buf[ES_EID_GATTS_READ_EID_IDX], 
                           &p_reg->slots[slot_no].adv_frame.frame.eid.eid, 
                           ES_EID_ID_LENGTH);
                    /*lint -restore */
                    reply.params.read.p_data = eid_buf;
                }

                // Otherwise, simply load the contents of the frame.
                else
                {
                    // Check if slot being read is an eTLM slot.
                    if (p_reg->num_configured_eid_slots > 0 && p_reg->tlm_configured &&
                        p_reg->tlm_slot == slot_no)
                    {
                        // Fill eTLM slot using EID key from first EID slot.
                        es_slot_etlm_update(p_reg->eid_slots_configured[0]);
                    }
                    reply.params.read.len    = p_reg->slots[slot_no].adv_frame.length;
                    reply.params.read.p_data = (uint8_t *)&p_reg->slots[slot_no].adv_frame.frame;
                }

                reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
                break;

            default:
                break;
        }

        if (!override_flag)
        {
            uint8_t           value_buffer[ESCS_ADV_SLOT_CHAR_LENGTH_MAX] = {0};
            ble_gatts_value_t value =
            {.len = sizeof(value_buffer), .offset = 0, .p_value = &(value_buffer[0])};

            err_code = sd_ble_gatts_value_get(p_escs->conn_handle, val_handle, &value);
            APP_ERROR_CHECK(err_code);

            reply.params.read.len         = value.len;
            reply.params.read.p_data      = (const uint8_t *)value.p_value;
            reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
        }
    }
    // When the beacon is locked and the client is trying to access characteristics other than
    // the lock characteristic, or when the beacon is unlocked and the client is trying to access
    // the unlock characteristic, deny access.
    else if ((!unlocked && (evt_type != NRF_BLE_ESCS_EVT_UNLOCK))
             || (unlocked && (evt_type == NRF_BLE_ESCS_EVT_UNLOCK)))
    {
        reply.params.read.gatt_status = BLE_GATT_STATUS_ATTERR_READ_NOT_PERMITTED;
        reply.params.read.update      = 1;
        reply.params.read.offset      = 0;
        reply.params.read.len         = 0;
        reply.params.read.p_data      = NULL;
    }

    // When the beacon is locked and the client is trying to access the unlock
    // characteristic accept the read and call the cryptography function to prepare for unlock.
    else if (!unlocked && (evt_type == NRF_BLE_ESCS_EVT_UNLOCK))
    {
        uint8_t  key_buff[ESCS_AES_KEY_SIZE];

        (void)es_security_random_challenge_generate(key_buff);
        es_security_unlock_prepare(key_buff);

        reply.params.read.gatt_status = BLE_GATT_STATUS_SUCCESS;
        reply.params.read.update      = 1;
        reply.params.read.offset      = 0;
        reply.params.read.len         = ESCS_AES_KEY_SIZE;
        reply.params.read.p_data      = key_buff;
    }

    if (p_escs->conn_handle != BLE_CONN_HANDLE_INVALID )
    {
        err_code = sd_ble_gatts_rw_authorize_reply(p_escs->conn_handle, &reply);
        if (err_code != NRF_SUCCESS)
        {
            APP_ERROR_CHECK(err_code);
        }
    }
}
