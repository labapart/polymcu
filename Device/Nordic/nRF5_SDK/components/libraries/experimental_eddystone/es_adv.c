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

#include "es_adv.h"
#include "es_app_config.h"
#include "app_error.h"
#include "es_tlm.h"
#include "es_adv_timing.h"
#include "es_adv_frame.h"

static es_adv_evt_handler_t m_adv_evt_handler;                                       //!< Eddystone advertisement event handler.
static bool                 m_is_connected       = false;                            //!< Is the Eddystone beacon in a connected state.
static bool                 m_remain_connectable = false;                            //!< Should the Eddystone beacon remain connectable.
static uint8_t              m_ecs_uuid_type      = 0;                                //!< UUID type of the Eddystone Configuration Service.
static uint16_t             m_adv_interval       = APP_CFG_NON_CONN_ADV_INTERVAL_MS; //!< Current advertisement interval.

/**@brief Function for invoking registered callback.  
 *
 * @param[in] evt Event to issue to callback.
 */
static void invoke_callback(es_adv_evt_t evt)
{
    if (m_adv_evt_handler != NULL)
    {
        m_adv_evt_handler(evt);
    }
}

/**@brief Starting advertising.
 * @param[in]   p_adv_params  Advertisement parameters to use.
 */
static void adv_start(ble_gap_adv_params_t * p_adv_params)
{
    uint32_t err_code = NRF_SUCCESS;

    es_tlm_adv_cnt_inc();

    err_code = sd_ble_gap_adv_start(p_adv_params);

    if (err_code != NRF_ERROR_BUSY && err_code != NRF_SUCCESS)
    {
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief  Given state of Eddystone beacon, get advertisement parameters. */
static void get_adv_params(ble_gap_adv_params_t * p_adv_params,
                           bool                   non_connectable,
                           bool                   remain_connectable)
{
    // Initialize advertising parameters (used when starting advertising).
    memset(p_adv_params, 0, sizeof(ble_gap_adv_params_t));

    // Non-connectable
    p_adv_params->type =
        non_connectable ? BLE_GAP_ADV_TYPE_ADV_NONCONN_IND : BLE_GAP_ADV_TYPE_ADV_IND;
    p_adv_params->p_peer_addr = NULL; // Undirected advertisement.
    p_adv_params->fp          = BLE_GAP_ADV_FP_ANY;
    p_adv_params->interval    = non_connectable ? BLE_GAP_ADV_INTERVAL_MAX : 1000;
    p_adv_params->timeout     =
        non_connectable ? APP_CFG_NON_CONN_ADV_TIMEOUT : (remain_connectable ? 0 :
                                                          APP_CFG_CONNECTABLE_ADV_TIMEOUT);
}


/**@brief  Update advertisement data and start connectable advertisements. */
static void connectable_adv_start(void)
{
    ble_gap_adv_params_t connectable_adv_params;
    ble_advdata_t        scrsp_data;
    ble_uuid_t           scrp_uuids[] = {{BLE_UUID_ESCS_SERVICE, m_ecs_uuid_type}};

    memset(&scrsp_data, 0, sizeof(scrsp_data));
    scrsp_data.name_type               = BLE_ADVDATA_FULL_NAME;
    scrsp_data.include_appearance      = false;
    scrsp_data.uuids_complete.uuid_cnt = sizeof(scrp_uuids) / sizeof(scrp_uuids[0]);
    scrsp_data.uuids_complete.p_uuids  = scrp_uuids;

    // As the data to be written does not depend on the slot_no, we can safely send
    es_adv_frame_fill_connectable_adv_data(&scrsp_data);

    get_adv_params(&connectable_adv_params, false, m_remain_connectable);
    adv_start(&connectable_adv_params);
    
    invoke_callback(ES_ADV_EVT_CONNECTABLE_ADV_STARTED);
}


static void adv_stop(void)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_stop();
    if (err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    }

    es_adv_timing_stop();
}


static void adv_restart(void)
{
    if (!m_remain_connectable)
    {
        es_adv_start_non_connctable_adv();
    }

    else
    {
        connectable_adv_start();
    }
}


/**@brief Function handling events from @ref es_adv_timing.c.
 *
 * @param[in] p_evt Advertisement timing event.
 */
static void adv_timing_callback(const es_adv_timing_evt_t * p_evt)
{
    uint32_t                   err_code;
    ble_gap_adv_params_t       non_connectable_adv_params;
    const es_slot_reg_t * p_reg = es_slot_get_registry();

    // As new advertisement data will be loaded, stop advertising.
    err_code = sd_ble_gap_adv_stop();
    if (err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    }

    // If a non-eTLM frame is to be advertised.
    if (p_evt->evt_id == ES_ADV_TIMING_EVT_ADV_SLOT)
    {
        err_code = sd_ble_gap_tx_power_set(p_reg->slots[p_evt->slot_no].radio_tx_pwr);
        APP_ERROR_CHECK(err_code);

        es_adv_frame_fill_non_connectable_adv_data(p_evt->slot_no, false);
    }

    // If an eTLM frame is to be advertised
    else if (p_evt->evt_id == ES_ADV_TIMING_EVT_ADV_ETLM)
    {
        err_code = sd_ble_gap_tx_power_set(p_reg->slots[p_reg->tlm_slot].radio_tx_pwr);
        APP_ERROR_CHECK(err_code);

        es_adv_frame_fill_non_connectable_adv_data(p_evt->slot_no, true);
    }

    invoke_callback(ES_ADV_EVT_NON_CONN_ADV);

    get_adv_params(&non_connectable_adv_params, true, m_remain_connectable);
    adv_start(&non_connectable_adv_params);
}


void es_adv_start_connectable_adv(void)
{
    if (!m_is_connected)
    {
        adv_stop();

        connectable_adv_start();
    }
}


void es_adv_start_non_connctable_adv(void)
{
    es_adv_timing_start(m_adv_interval);
}


void es_adv_remain_connectable_set(bool remain_connectable)
{
    m_remain_connectable = remain_connectable;
}


bool es_adv_remain_connectable_get(void)
{
    return m_remain_connectable;
}


void es_adv_on_ble_evt(ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_is_connected = true;

            // The beacon must provide these advertisements for the client to see updated values during the connection.
            es_adv_start_non_connctable_adv();
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            m_is_connected = false;

            // Stop all advertising to give some time for writing to flash.
            adv_stop();
            adv_restart();
            break;

        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING && \
                !m_is_connected)
            {
                adv_restart();
            }
            break;

        default:
            break;
    }
}


void es_adv_interval_set(nrf_ble_escs_adv_interval_t interval)
{
    m_adv_interval = interval;
}


nrf_ble_escs_adv_interval_t es_adv_interval_get(void)
{
    return m_adv_interval;
}


void es_adv_init(uint8_t                     ecs_uuid_type,
                 es_adv_evt_handler_t        adv_event_handler,
                 nrf_ble_escs_adv_interval_t adv_interval,
                 bool                        remain_connectable)
{
    m_ecs_uuid_type      = ecs_uuid_type;
    m_adv_evt_handler    = adv_event_handler;
    m_is_connected       = false;
    m_remain_connectable = remain_connectable;
    m_adv_interval       = adv_interval;

    es_tlm_init();

    es_adv_timing_init(adv_timing_callback);
}
