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

#include "es_adv_frame.h"
#include "es_app_config.h"
#include "es_tlm.h"
#include "es_slot.h"

/**@brief Function for setting advertisement data, using 'ble_advdata_set'.
 *
 * @param[in] p_scrsp_data      Scan response data. 
 * @param[in] p_es_data_array   Eddystone service data array.
 */
static void fill_adv_data(ble_advdata_t * p_scrsp_data, uint8_array_t * p_es_data_array)
{
    ble_advdata_t adv_data;
    uint32_t      err_code;
    ble_uuid_t    adv_uuids[]   = {{ES_UUID, BLE_UUID_TYPE_BLE}};
    uint8_array_t es_data_array = {0};

    ble_advdata_service_data_t service_data;        // Structure to hold Service Data.

    service_data.service_uuid = APP_ES_UUID; // Eddystone UUID to allow discoverability on iOS devices.

    service_data.data = (p_es_data_array != NULL) ? *p_es_data_array : es_data_array;

    // Build and set advertising data.
    memset(&adv_data, 0, sizeof(ble_advdata_t));

    adv_data.name_type               = BLE_ADVDATA_NO_NAME;
    adv_data.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    adv_data.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    adv_data.uuids_complete.p_uuids  = adv_uuids;
    adv_data.p_service_data_array    = &service_data;
    adv_data.service_data_count      = (p_es_data_array != NULL) ? 1 : 0;

    err_code = ble_advdata_set(&adv_data, p_scrsp_data);
    APP_ERROR_CHECK(err_code);
}


void es_adv_frame_fill_connectable_adv_data(ble_advdata_t * p_scrsp_data)
{
    fill_adv_data(p_scrsp_data, NULL);
}


void es_adv_frame_fill_non_connectable_adv_data(uint8_t slot_no, bool etlm)
{
    uint8_array_t         es_data_array = {0};
    const es_slot_reg_t * p_reg         = es_slot_get_registry();

    if (etlm)
    {
        es_slot_etlm_update(slot_no);

        // If eTLM, the incoming slot_no points to the corresponding EID slot, update to point to TLM slot.
        slot_no = p_reg->tlm_slot;
    }

    // If TLM, update the TLM data.
    else if(p_reg->slots[slot_no].adv_frame.type == ES_FRAME_TYPE_TLM)
    {
        es_slot_tlm_update();
    }
    
    es_data_array.p_data = (uint8_t *) &p_reg->slots[slot_no].adv_frame.frame;
    es_data_array.size   = p_reg->slots[slot_no].adv_frame.length;

    fill_adv_data(NULL, &es_data_array);
}
