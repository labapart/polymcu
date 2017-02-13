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

#include "es_slot.h"
#include "es_slot_reg.h"
#include "es_app_config.h"
#include "nrf_error.h"
#include "ble_gap.h"
#include "es_security.h"
#include "es_flash.h"
#include "es_tlm.h"
#include <stdint.h>
#include <string.h>
#include "fds.h"

static es_slot_reg_t m_reg;             //!< Slot registry.
static bool m_eid_loaded_from_flash;    //!< Set to true if EID slot has been loaded from flash.

#define RANGING_DATA_INDEX  (1)         //!< Index of ranging data within frames that contain ranging data.
#define RANGING_DATA_LENGTH (1)         //!< Length of ranging data.

/**@brief Enforce legal slot number.
 *
 * @param[in] p_slot Pointer to the slot number variable to check.
 */
static void slot_boundary_check(uint8_t * p_slot)
{
    if (*p_slot > (APP_MAX_ADV_SLOTS - 1))
    {
        *p_slot = (APP_MAX_ADV_SLOTS - 1);
    }
}


/**@brief Function loading slot data from flash.
 *
 * @param[in] slot_no       Slot number to be used.
 */
static void load_slot_from_flash(uint8_t slot_no)
{
    ret_code_t err_code;

    err_code = es_flash_access_slot_configs(slot_no,
                                            &m_reg.slots[slot_no],
                                            ES_FLASH_ACCESS_READ);
    if (err_code != FDS_ERR_NOT_FOUND)
    {
        APP_ERROR_CHECK(err_code);

        if (m_reg.slots[slot_no].adv_frame.type == ES_FRAME_TYPE_EID)
        {
            m_eid_loaded_from_flash = true;
            
            es_security_eid_slots_restore(slot_no,
                                          m_reg.slots[slot_no].k_scaler,
                                          m_reg.slots[slot_no].seconds,
                                          (const uint8_t *)m_reg.slots[slot_no].ik);
        }

        else
        {
            // If a non-EID slot has been loaded, update the state of m_reg immediately.
            es_slot_reg_update_slot_list_info_on_add(&m_reg, slot_no, m_reg.slots[slot_no].adv_frame.type, true);
        }

    }
}


/**@brief Function for setting the ranging data field to be broadcast in the frame.
 *
 * @param[in]       slot_no         The slot index.
 * @param[in]       tx_power        The radio tx power to be calibrated to ranging data.
 */
static void set_ranging_data_for_slot(uint8_t slot_no, nrf_ble_escs_radio_tx_pwr_t tx_power)
{
    int8_t ranging_data_array[ESCS_NUM_OF_SUPPORTED_TX_POWER] =
        APP_CONFIG_CALIBRATED_RANGING_DATA;
    nrf_ble_escs_radio_tx_pwr_t supported_tx[ESCS_NUM_OF_SUPPORTED_TX_POWER] =
        ESCS_SUPPORTED_TX_POWER;

    int8_t ranging_data = 0;

    if (m_reg.slots[slot_no].adv_custom_tx_power)
    {
        ranging_data = m_reg.slots[slot_no].custom_tx_power;
    }

    else
    {
        for (uint8_t i = 0; i < ESCS_NUM_OF_SUPPORTED_TX_POWER; i++)
        {
            if (supported_tx[i] >= tx_power)
            {
                ranging_data = ranging_data_array[i];
                break;
            }
        }
    }

    uint8_t * p_ranging_data = ((uint8_t *)(&m_reg.slots[slot_no].adv_frame.frame) + 1);
    *p_ranging_data = ranging_data;
}


/**@brief Function configuring a non-EID slot.
 *
 * @param[in] slot_no       Slot number to be used.
 * @param[in] length        Length of write operation.
 * @param[in] p_frame_data  Pointer to written data.
 */
static void configure_slot(uint8_t slot_no, uint8_t length, uint8_t * p_frame_data)
{
    // If a TLM slot is being configured and there already exists a TLM.
    if ((es_frame_type_t)p_frame_data[0] == ES_FRAME_TYPE_TLM && m_reg.tlm_configured)
    {
        return; // Silently ignore any attempts to create more than one TLM slot as there is no point.
    }

    es_slot_reg_update_slot_list_info_on_add(&m_reg, slot_no, (es_frame_type_t)p_frame_data[0], false);

    // For convenience, frame_type is stored in two places, set both.
    m_reg.slots[slot_no].adv_frame.type = (es_frame_type_t)p_frame_data[0];
    memcpy(&m_reg.slots[slot_no].adv_frame.frame, &m_reg.slots[slot_no].adv_frame.type, 1);

    uint8_t * p_data_after_ranging_data = \
        ((uint8_t *)(&m_reg.slots[slot_no].adv_frame.frame) + RANGING_DATA_INDEX + RANGING_DATA_LENGTH);

    switch (m_reg.slots[slot_no].adv_frame.type)
    {
        case ES_FRAME_TYPE_UID:
            // Fall through.
        case ES_FRAME_TYPE_URL:
            memcpy(p_data_after_ranging_data, &p_frame_data[1], length);
            set_ranging_data_for_slot(slot_no, APP_CFG_DEFAULT_RADIO_TX_POWER);
            m_reg.slots[slot_no].adv_frame.length = length + 1; // + 1 for ranging data
            break;

        case ES_FRAME_TYPE_TLM:
            es_tlm_tlm_get(&m_reg.slots[slot_no].adv_frame.frame.tlm);
            m_reg.slots[slot_no].adv_frame.length = ES_TLM_LENGTH;
            break;

        default:
            break;
    }
}


/**@brief Function configuring an EID slot.
 *
 * @param[in] slot_no       Slot number to be used.
 * @param[in] length        Length of write operation.
 * @param[in] p_frame_data  Pointer to written data.
 */
static void configure_eid_slot(uint8_t slot_no, uint8_t length, uint8_t * p_frame_data)
{
    ret_code_t err_code;
    bool clear_eid_slot = false;
    
    // Do not update slot count, as this will be done when in the callback invoked when the EID data is ready.
    // As it takes a while to do the calculation, temporarily remove the slot being overwritten.
    // The slot will be re-added in the callback invoked when the EID data is ready.
    clear_eid_slot = es_slot_reg_clear_slot(&m_reg, slot_no);
    
    if(clear_eid_slot)
    {
        es_security_eid_slot_destroy(slot_no);
    }

    if (p_frame_data[0] != ES_FRAME_TYPE_EID)
    {
        APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE);
    }

    if (length == ESCS_EID_WRITE_ECDH_LENGTH)
    {
        err_code = es_security_client_pub_ecdh_receive(slot_no, 
                                                      &p_frame_data[ESCS_EID_WRITE_PUB_KEY_INDEX], 
                                                       p_frame_data[ESCS_EID_WRITE_ECDH_LENGTH -1]);
        APP_ERROR_CHECK(err_code);
    }

    else if (length == ESCS_EID_WRITE_IDK_LENGTH)
    {
        es_security_shared_ik_receive(slot_no, 
                                     &p_frame_data[ESCS_EID_WRITE_ENC_ID_KEY_INDEX], 
                                      p_frame_data[ESCS_EID_WRITE_IDK_LENGTH - 1]);
    }
    
    else
    {
        // Invalid length being written.
        APP_ERROR_CHECK(NRF_ERROR_INVALID_PARAM);
    }
}


ret_code_t es_slot_write_to_flash(uint8_t slot_no)
{
    if (m_reg.slots[slot_no].configured)
    {
        // If its an EID, we need to store some metadata in order to re-initialize the EID.
        if (m_reg.slots[slot_no].adv_frame.type == ES_FRAME_TYPE_EID)
        {
            m_reg.slots[slot_no].seconds  = es_security_clock_get(slot_no);
            m_reg.slots[slot_no].k_scaler = es_security_scaler_get(slot_no);
            es_security_encrypted_eid_id_key_get(slot_no, m_reg.slots[slot_no].ik);
        }
        return es_flash_access_slot_configs(slot_no,
                                            &m_reg.slots[slot_no],
                                            ES_FLASH_ACCESS_WRITE);
    }

    else
    {
        return es_flash_access_slot_configs(slot_no,
                                            NULL,
                                            ES_FLASH_ACCESS_CLEAR);
    }
}


void es_slot_radio_tx_pwr_set(uint8_t slot_no, nrf_ble_escs_radio_tx_pwr_t radio_tx_pwr)
{
    slot_boundary_check(&slot_no);

    m_reg.slots[slot_no].radio_tx_pwr = radio_tx_pwr;

    if (!m_reg.slots[slot_no].adv_custom_tx_power) // Only update TX power in ADV if custom TX power is not set
    {
        set_ranging_data_for_slot(slot_no, radio_tx_pwr);
    }
}


void es_slot_set_adv_custom_tx_power(uint8_t slot_no, nrf_ble_escs_adv_tx_pwr_t tx_pwr)
{
    slot_boundary_check(&slot_no);

    m_reg.slots[slot_no].adv_custom_tx_power = true;
    m_reg.slots[slot_no].custom_tx_power     = tx_pwr;
    set_ranging_data_for_slot(slot_no, tx_pwr);
}


void es_slot_on_write(uint8_t slot_no, uint8_t length, uint8_t * p_frame_data)
{
    slot_boundary_check(&slot_no);
    
    if(p_frame_data == NULL)
    {
        APP_ERROR_CHECK(NRF_ERROR_NULL);
    }

    // If slot is being cleared
    if (length == 0 || (length == 1 && p_frame_data[0] == 0))
    {
        (void)es_slot_reg_clear_slot(&m_reg, slot_no);
    }

    // EID slot being configured
    else if (p_frame_data[0] == ES_FRAME_TYPE_EID && \
             (length == ESCS_EID_WRITE_ECDH_LENGTH || length == ESCS_EID_WRITE_IDK_LENGTH))
    {
        configure_eid_slot(slot_no, length, p_frame_data);
    }

    // Non-EID slot configured.
    else
    {
        configure_slot(slot_no, length, p_frame_data);
    }
}


void es_slot_encrypted_eid_id_key_set(uint8_t slot_no, nrf_ble_escs_eid_id_key_t * p_eid_id_key)
{
    slot_boundary_check(&slot_no);
    if (p_eid_id_key != NULL)
    {
        memcpy(&(m_reg.slots[slot_no].encrypted_eid_id_key), p_eid_id_key,
               sizeof(nrf_ble_escs_eid_id_key_t));
    }
}


void es_slot_eid_ready(uint8_t slot_no)
{
    m_reg.slots[slot_no].adv_frame.type   = ES_FRAME_TYPE_EID;
    m_reg.slots[slot_no].adv_frame.length = ES_EID_LENGTH;
    es_security_eid_get(slot_no, (uint8_t *)m_reg.slots[slot_no].adv_frame.frame.eid.eid);
    m_reg.slots[slot_no].adv_frame.frame.eid.frame_type = ES_FRAME_TYPE_EID;
    set_ranging_data_for_slot(slot_no, m_reg.slots[slot_no].radio_tx_pwr);

    if (m_eid_loaded_from_flash)
    {
        es_slot_reg_update_slot_list_info_on_add(&m_reg, slot_no, ES_FRAME_TYPE_EID, true);
        m_eid_loaded_from_flash = false;
    }

    else
    {
        es_slot_reg_update_slot_list_info_on_add(&m_reg, slot_no, ES_FRAME_TYPE_EID, false);
    }
}


static bool slot_is_eid(uint8_t eid_slot_no)
{
    for (uint8_t i = 0; i < m_reg.num_configured_eid_slots; ++i)
    {
        if (m_reg.eid_slots_configured[i] == eid_slot_no)
        {
            return true;
        }
    }
    
    return false;
}


void es_slot_tlm_update(void)
{
    if(m_reg.tlm_configured)
    {
        es_tlm_tlm_get(&m_reg.slots[m_reg.tlm_slot].adv_frame.frame.tlm);
    }
}


void es_slot_etlm_update(uint8_t eid_slot_no)
{
    es_tlm_frame_t  tlm;
    es_etlm_frame_t etlm;

    // Ignore the request if eTLM is not required or slot no does not correspond to an EID slot.
    if (!es_slot_reg_etlm_required(&m_reg) || !slot_is_eid(eid_slot_no))
    {
        return;
    }

    es_tlm_tlm_get(&tlm);

    es_security_tlm_to_etlm(eid_slot_no, &tlm, &etlm);

    memcpy(&m_reg.slots[m_reg.tlm_slot].adv_frame.frame.etlm, &etlm, sizeof(es_etlm_frame_t));
    m_reg.slots[m_reg.tlm_slot].adv_frame.length = sizeof(es_etlm_frame_t);
}


const es_slot_reg_t * es_slot_get_registry(void)
{
    return (const es_slot_reg_t *)&m_reg;
}



void es_slots_init(const es_slot_t * p_default_slot)
{
    ret_code_t       err_code;
    es_flash_flags_t flash_flags = {{0}};

    es_slot_reg_init(&m_reg);
    
    m_eid_loaded_from_flash = false;
    
    // Read the flash flags to see if there are any previously stored slot configs
    err_code = es_flash_access_flags(&flash_flags, ES_FLASH_ACCESS_READ);

    if (err_code == FDS_ERR_NOT_FOUND)
    {
        // Factory reset or initial boot, load default data
        memcpy(&m_reg.slots[0], p_default_slot, sizeof(*p_default_slot));
        es_slot_reg_update_slot_list_info_on_add(&m_reg, 0, p_default_slot->adv_frame.type, true);
    }

    else
    {
        APP_ERROR_CHECK(err_code);

        for (uint8_t i = 0; i < APP_MAX_ADV_SLOTS; i++)
        {
            if (!flash_flags.slot_is_empty[i])
            {
                load_slot_from_flash(i);
            }
        }
    }
}
