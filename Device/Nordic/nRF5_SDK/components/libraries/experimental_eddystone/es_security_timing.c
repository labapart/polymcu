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

#include <stdint.h>
#include <stdbool.h>
#include "app_timer.h"
#include "es_app_config.h"

#define TICKS_1_SECOND APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)

static uint32_t m_ticks_last_returned_second = 0; //!< Tick count at last second returned from 'es_security_timing_seconds_passed'.


uint32_t es_security_timing_seconds_passed(void)
{
    uint32_t ticks_current = app_timer_cnt_get();
    uint32_t ticks_diff;
    uint32_t err_code;
    
    err_code = app_timer_cnt_diff_compute(ticks_current, m_ticks_last_returned_second, &ticks_diff);
    APP_ERROR_CHECK(err_code);
    
    if(ticks_diff > TICKS_1_SECOND)
    {
        // Update 'm_ticks_last_returned_second' so it points to the last 'whole second' as it is this point we are referring to when
        // returning the number of seconds passed since last invocation. 
        // I.E. if this function is called after 1.5 seconds and 2.0 seconds, 'm_ticks_last_returned_second' must point to 1.0 seconds
        // after the first invocation in order to return '1' upon the second invocation.
        uint32_t current_second_ms = (ticks_current / TICKS_1_SECOND) * 1000;
        
        m_ticks_last_returned_second = APP_TIMER_TICKS(current_second_ms, APP_TIMER_PRESCALER);
        
        return ticks_diff / TICKS_1_SECOND;
    }
    
    return 0;
}


void es_security_timing_init(void)
{
    m_ticks_last_returned_second = 0;
}
