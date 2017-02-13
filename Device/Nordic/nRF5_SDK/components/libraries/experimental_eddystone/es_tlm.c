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

#include "es_battery_voltage.h"
#include "es_tlm.h"
#include "es_app_config.h"
#include "nrf_soc.h"
#include "app_error.h"
#include "app_timer.h"
#include <string.h>

#define MAX_RTC_COUNTER_VAL     0x00FFFFFF                     //!< Maximum value of the RTC counter.
#define TICKS_100_MS APP_TIMER_TICKS(100, APP_TIMER_PRESCALER) //!< Tick count for 100ms.

static es_tlm_frame_t m_tlm;


/**@brief Function for updating the ADV_SEC field of TLM*/
static void update_time(void)
{
    static uint32_t time_bank_100_ms = 0;
    static uint32_t prev_ticks       = 0;
    uint32_t        time_total_100_ms;
    uint32_t        current_ticks;
    uint32_t        be_time_100_ms; // Big endian version of 0.1 second counter.

    current_ticks = app_timer_cnt_get();
    
    // If RTC timer has wrapped around, update the accumulated time.
    if(prev_ticks > current_ticks)
    {
        time_bank_100_ms += MAX_RTC_COUNTER_VAL / TICKS_100_MS;
    }
    
    time_total_100_ms = time_bank_100_ms + (current_ticks / TICKS_100_MS);
    
    be_time_100_ms = BYTES_REVERSE_32BIT(time_total_100_ms);
    
    memcpy(m_tlm.sec_cnt, &be_time_100_ms, ES_TLM_SEC_CNT_LENGTH);
    
    prev_ticks = current_ticks;
}


/**@brief Function for updating the TEMP field of TLM*/
static void update_temp(void)
{
    static uint8_t temp_counter = APP_CONFIG_TLM_TEMP_INTERVAL_SECONDS - 1;

    // No need to get the temperature every time a TLM is broadcast since temperature
    // is assumed to be pretty constant. Thus temperature is sampled every APP_CONFIG_TLM_TEMP_INTERVAL_SECONDS times a
    // TLM is broadcast
    temp_counter++;

    if (temp_counter % APP_CONFIG_TLM_TEMP_INTERVAL_SECONDS == 0)
    {
        int32_t temp;                                        // variable to hold temp reading
        (void)sd_temp_get(&temp);                            // get new temperature
        int16_t temp_new = (int16_t) temp;                   // convert from int32_t to int16_t
        m_tlm.temp[0] = (uint8_t)((temp_new >> 2) & 0xFFUL); // Right-shift by two to remove decimal part
        m_tlm.temp[1] = (uint8_t)((temp_new << 6) & 0xFFUL); // Left-shift 6 to get fractional part with 0.25 degrees C resolution
        temp_counter  = 0;                                   // reset temp counter
    }
}


/**@brief Function for updating the VBATT field of TLM*/
static void update_vbatt(void)
{
    static uint8_t vbatt_counter = APP_CONFIG_TLM_VBATT_INTERVAL_SECONDS - 1;

    // No need to get the battery voltage every time a TLM is broadcast since the voltage
    // is assumed to be pretty constant. Thus the battery voltage is sampled every
    // APP_CONFIG_TLM_VBATT_INTERVAL_SECONDS times a TLM is broadcast
    vbatt_counter++;

    if (vbatt_counter % APP_CONFIG_TLM_VBATT_INTERVAL_SECONDS == 0)
    {
        uint16_t vbatt;                 // Variable to hold voltage reading
        es_battery_voltage_get(&vbatt); // Get new battery voltage
        m_tlm.vbatt[0] = (uint8_t)(vbatt >> 8);
        m_tlm.vbatt[1] = (uint8_t)vbatt;
        vbatt_counter  = 0; // Reset vbatt counter
    }
}


void es_tlm_tlm_get(es_tlm_frame_t * p_tlm_frame)
{
    // Note that frame type and TLM version fields are set in initialization.
    update_time();
    update_temp();
    update_vbatt();
    memcpy(p_tlm_frame, &m_tlm, sizeof(es_tlm_frame_t));
}


void es_tlm_adv_cnt_inc(void)
{
    static uint32_t le_adv_cnt = 0; // Little endian
    uint32_t        be_adv_cnt = 0; // Big endian

    le_adv_cnt++;
    be_adv_cnt  = BYTES_REVERSE_32BIT(le_adv_cnt);

    memcpy(m_tlm.adv_cnt, (uint8_t *)(&be_adv_cnt), ES_TLM_ADV_CNT_LENGTH);
}


void es_tlm_init(void)
{
    memset(&m_tlm, 0, sizeof(m_tlm));
    m_tlm.frame_type = ES_FRAME_TYPE_TLM;
    m_tlm.version    = ES_TLM_VERSION_TLM;
    update_time(); // Increment the time right way during module init.
}


