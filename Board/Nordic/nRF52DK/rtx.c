/*
 * Copyright (c) 2015-2017, Lab A Part
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

#include <assert.h>
#include <cmsis_os.h>
#include "board.h"

#include "nrf.h"
#include "bsp.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_rtc.h"

const nrf_drv_rtc_t rtc1 = NRF_DRV_RTC_INSTANCE(1); /**< Declaring an instance of nrf_drv_rtc for RTC1. */

int os_tick_init(void) {
	uint32_t err_code;

	// Initialize the clock
    err_code = nrf_drv_clock_init();
    assert(err_code == NRF_SUCCESS);
    nrf_drv_clock_lfclk_request(NULL);

    // Initialize RTC instance
    static const nrf_drv_rtc_config_t rtc1_config = NRF_DRV_RTC_DEFAULT_CONFIG(1);
    err_code = nrf_drv_rtc_init(&rtc1, &rtc1_config, NULL);
    assert(err_code == NRF_SUCCESS);

    // Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc1, true);

    // Enable overflow
    nrf_drv_rtc_overflow_enable(&rtc1, false);

    // Power on RTC instance
    nrf_drv_rtc_enable(&rtc1);

    // Return -1 to prevent RTC touching NVIC
    return -1;  /* Return IRQ number of timer (0..239) */
}

/*--------------------------- os_tick_val -----------------------------------*/

/// \brief Get alternative hardware timer's current value (0 .. OS_TRV)
/// \return                             Current value of the alternative hardware timer
uint32_t os_tick_val(void) {
	return nrf_drv_rtc_counter_get(&rtc1);
}

/*--------------------------- os_tick_ovf -----------------------------------*/

/// \brief Get alternative hardware timer's  overflow flag
/// \return                             Overflow flag\n
///                                     - 1 : overflow
///                                     - 0 : no overflow
uint32_t os_tick_ovf(void) {
	if (rtc1.p_reg->EVENTS_OVRFLW) {
		return 1;
	} else {
		return 0;
	}
}

/*--------------------------- os_tick_irqack --------------------------------*/

/// \brief Acknowledge alternative hardware timer interrupt
void os_tick_irqack(void) {
	nrf_rtc_event_clear(rtc1.p_reg, NRF_RTC_EVENT_TICK);
}
