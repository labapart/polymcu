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

#include "es_adv_timing.h"
#include "es_slot.h"
#include "app_timer.h"
#include "nrf_ble_es.h"
#include "es_app_config.h"
#include "es_adv_timing_resolver.h"

APP_TIMER_DEF(m_es_adv_interval_timer);                         //!< Timer for advertising the set of slots.
APP_TIMER_DEF(m_es_slot_timer);                                 //!< Timer for advertising individual slots.

static nrf_ble_escs_adv_interval_t     m_current_adv_interval;  //!< Current advertisement interval.
static es_adv_timing_callback_t        m_timing_mgr_callback;   //!< Registered callback.
static es_adv_timing_resolver_result_t m_adv_timing_result;     //!< Current advertising timing result.
static bool                            m_non_conn_adv_active;   //!< Is the beacon advertising non-conn advertisements?

/**@brief Function for invoking registered callback.  
 *
 * @param[in] p_evt Event to issue to callback.
 */
static void invoke_callback(const es_adv_timing_evt_t * p_evt)
{
    if(m_timing_mgr_callback != NULL && m_non_conn_adv_active)
    {
        m_timing_mgr_callback(p_evt);
    }
}


/**@brief Timeout handler for the advertisement slot timer. */
static void adv_slot_timeout(void * p_context)
{
    uint32_t err_code;
    uint32_t active_slot_index = (uint32_t)p_context;

    es_adv_timing_evt_t evt;

    evt.slot_no = m_adv_timing_result.timing_results[active_slot_index].slot_no;

    evt.evt_id = m_adv_timing_result.timing_results[active_slot_index].is_etlm ? \
                 ES_ADV_TIMING_EVT_ADV_ETLM : ES_ADV_TIMING_EVT_ADV_SLOT;

    // Trigger an event for the next slot if this slot is not the last to be advertised in this event.
    // Note: since we check 'm_adv_timing_result.len_timing_results > 1' we can safely cast the result of 
    //       the subtraction to a uint32.
    if (m_non_conn_adv_active && \
        m_adv_timing_result.len_timing_results > 1 && \
        active_slot_index < (uint32_t)(m_adv_timing_result.len_timing_results - 1)) 
    {
        err_code = app_timer_start( m_es_slot_timer, 
                                    APP_TIMER_TICKS(m_adv_timing_result.timing_results[active_slot_index].delay_ms, APP_TIMER_PRESCALER), 
                                    (void*)(active_slot_index + 1));
        APP_ERROR_CHECK(err_code);
    }

    invoke_callback(&evt);
}


/**@brief Timeout handler for the advertisement interval timer. */
static void adv_interval_timeout(void * p_context)
{
    if (es_slot_get_registry()->num_configured_slots > 0)
    {
        // Trigger slot timeout for advertising the first slot.
        // Note: The slot number is not the index in the slot registry, it is the index of the active slots.
        adv_slot_timeout(NULL);
    }
    
    if(m_non_conn_adv_active)
    {
        uint32_t err_code = app_timer_start(m_es_adv_interval_timer, 
                                   APP_TIMER_TICKS(m_current_adv_interval, APP_TIMER_PRESCALER), 
                                   NULL);
        APP_ERROR_CHECK(err_code);
    }
}


static void timers_init(void)
{
    ret_code_t err_code;

    err_code = app_timer_create(&m_es_adv_interval_timer,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                adv_interval_timeout);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_es_slot_timer,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                adv_slot_timeout);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for finding and setting advertisement timing configuration. */
static void adv_timing_set(void)
{
    uint32_t err_code;
    const es_slot_reg_t * p_reg = es_slot_get_registry();

    es_adv_timing_resolver_input_t resolver_input = {
        .adv_interval             = m_current_adv_interval,
        .p_result                 = &m_adv_timing_result,
        .num_slots_configured     = p_reg->num_configured_slots,
        .p_slots_configured       = p_reg->slots_configured,
        .num_eid_slots_configured = p_reg->num_configured_eid_slots,
        .p_eid_slots_configured   = p_reg->eid_slots_configured,
        .tlm_configured           = p_reg->tlm_configured,
        .tlm_slot                 = p_reg->tlm_slot
    };

    err_code = es_adv_timing_resolve(&resolver_input);
    APP_ERROR_CHECK(err_code);
}


void es_adv_timing_start(uint16_t adv_interval)
{
    uint32_t err_code;

    const es_slot_reg_t * p_reg = es_slot_get_registry();

    m_non_conn_adv_active = true;
    
    if (p_reg->num_configured_slots > 0)
    {
        m_current_adv_interval = adv_interval;

        err_code = app_timer_start(m_es_adv_interval_timer, 
                                   APP_TIMER_TICKS(m_current_adv_interval, APP_TIMER_PRESCALER), 
                                   NULL);
        APP_ERROR_CHECK(err_code);

        adv_timing_set();
    }
}


void es_adv_timing_stop(void)
{
    m_non_conn_adv_active = false; // Stops timers from being re-fired.
}


void es_adv_timing_init(es_adv_timing_callback_t p_handler)
{
    m_non_conn_adv_active = false;
    m_timing_mgr_callback = p_handler;
    memset(&m_adv_timing_result, 0, sizeof(m_adv_timing_result));
    timers_init();
}
