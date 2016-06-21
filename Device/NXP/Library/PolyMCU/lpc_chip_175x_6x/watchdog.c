/*
 * Copyright (c) 2016, Lab A Part
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

#include "board.h"
#include "PolyMCU.h"

uint32_t g_watchdog_frequency;

void polymcu_watchdog_init(void) {
	// Initialize WWDT and event router
	Chip_WWDT_Init(LPC_WWDT);

#if defined(WATCHDOG_CLKSEL_SUPPORT)
	Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_WATCHDOG_PCLK);
	g_watchdog_frequency = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_WDT) / 4;
#else
	g_watchdog_frequency = WDT_OSC;
#endif

	// Set watchdog feed time constant
	Chip_WWDT_SetTimeOut(LPC_WWDT, (g_watchdog_frequency * (WATCHDOG_RESOLUTION_MS / 1000)));

	// Configure WWDT to reset on timeout
	Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);

	// Clear watchdog warning and timeout interrupts
	Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);

	// Start watchdog
	Chip_WWDT_Start(LPC_WWDT);

	// Enable watchdog interrupt
	NVIC_ClearPendingIRQ(WDT_IRQn);
	NVIC_EnableIRQ(WDT_IRQn);
}

void polymcu_watchdog_alive(void) {
	Chip_WWDT_Feed(LPC_WWDT);
}

void polymcu_watchdog_set_timeout(uint32_t timeout_ms) {
	Chip_WWDT_SetTimeOut(LPC_WWDT, (g_watchdog_frequency * timeout_ms) / 1000);
}

void polymcu_watchdog_restore_default_timeout(void) {
	Chip_WWDT_SetTimeOut(LPC_WWDT, (WDT_OSC * WATCHDOG_RESOLUTION_MS) / 1000);
}
