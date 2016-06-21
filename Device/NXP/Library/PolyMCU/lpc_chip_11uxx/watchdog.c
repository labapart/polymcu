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
	// Initialize WWDT (also enables WWDT clock)
	Chip_WWDT_Init(LPC_WWDT);

	// Prior to initializing the watchdog driver, the clocking for the
	// watchdog must be enabled. Use a 50KHz (= 1Mhz / 20) watchdog oscillator
	Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_WDTOSC_PD);
	Chip_Clock_SetWDTOSC(WDTLFO_OSC_1_05, 20);

	// The WDT divides the input frequency into it by 4
	g_watchdog_frequency = Chip_Clock_GetWDTOSCRate() / 4;

	// LPC1102/4, LPC11XXLV, and LPC11CXX devices select the watchdog
	// clock source from the SYSCLK block, while LPC11AXX, LPC11EXX, and
	// LPC11UXX devices select the clock as part of the watchdog block.
	// Select watchdog oscillator for WDT clock source
#if defined(CHIP_LPC110X) || defined(CHIP_LPC11XXLV) || defined(CHIP_LPC11CXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC1125)
	Chip_Clock_SetWDTClockSource(SYSCTL_WDTCLKSRC_WDTOSC, 1);
#else
	Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_WATCHDOG_WDOSC);
#endif

	Chip_WWDT_SetTimeOut(LPC_WWDT, (g_watchdog_frequency * (WATCHDOG_RESOLUTION_MS / 1000)));

#if !defined(CHIP_LPC11CXX)
	// Configure WWDT to reset on timeout
	Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);
#endif

	// Clear watchdog warning and timeout interrupts
#if !defined(CHIP_LPC11CXX)
	Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);
#else
	Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF);
#endif

	// Clear and enable watchdog interrupt
	NVIC_ClearPendingIRQ(WDT_IRQn);
	NVIC_EnableIRQ(WDT_IRQn);

	// Start watchdog
	Chip_WWDT_Start(LPC_WWDT);
}

void polymcu_watchdog_alive(void) {
	Chip_WWDT_Feed(LPC_WWDT);
}

void polymcu_watchdog_set_timeout(uint32_t timeout_ms) {
	Chip_WWDT_SetTimeOut(LPC_WWDT, (g_watchdog_frequency * (timeout_ms / 1000)));
}

void polymcu_watchdog_restore_default_timeout(void) {
	Chip_WWDT_SetTimeOut(LPC_WWDT, (g_watchdog_frequency * (WATCHDOG_RESOLUTION_MS / 1000)));
}
