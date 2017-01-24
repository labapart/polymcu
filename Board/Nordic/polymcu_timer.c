/*
 * Copyright (c) 2017, Lab A Part
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * o list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "PolyMCU.h"

#include "nrf.h"
#include "bsp.h"
#include "nrf_drv_timer.h"

static const nrf_drv_timer_t POLYMCU_HW_TIMER = NRF_DRV_TIMER_INSTANCE(0);

static int g_period = 0;

void polymcu_timer_event_handler(nrf_timer_event_t event_type, void* p_context) {
	polymcu_timer_irq_handler();
}

// Generate an interrupt every 'period'
int polymcu_timer_init(unsigned int period) {
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;

    // Set the frequency
    timer_cfg.frequency = NRF_TIMER_FREQ_1MHz;

    uint32_t err_code = nrf_drv_timer_init(&POLYMCU_HW_TIMER, &timer_cfg, polymcu_timer_event_handler);
    if (err_code != NRF_SUCCESS) {
    	APP_ERROR_CHECK(err_code);
    } else {
    	g_period = period;

    	uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&POLYMCU_HW_TIMER, 1000 / period);

        nrf_drv_timer_extended_compare(
             &POLYMCU_HW_TIMER, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    	nrf_drv_timer_power_on(&POLYMCU_HW_TIMER);
    }
    return err_code;
}

unsigned int polymcu_timer_get_period(void) {
	return g_period;
}

void polymcu_timer_hw_start(void) {
	nrf_drv_timer_resume(&POLYMCU_HW_TIMER);
}

void polymcu_timer_hw_stop(void) {
	nrf_drv_timer_pause(&POLYMCU_HW_TIMER);
}
