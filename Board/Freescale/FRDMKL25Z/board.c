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

#include <unistd.h>

#include "board.h"
#include "Driver_USART.h"

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_smc.h"

extern const ARM_DRIVER_USART Driver_UART_DEBUG;

#define LED_COUNT  3

#define LOGIC_LED_ON  0U
#define LOGIC_LED_OFF 1U

static const struct {
	GPIO_Type* gpio;
	PORT_Type* port;
	int        pin;
	port_mux_t mux;
} g_leds[LED_COUNT] = {
	{ GPIOB, PORTB, 18, kPORT_MuxAsGpio }, // RED LED
	{ GPIOB, PORTB, 19, kPORT_MuxAsGpio }, // GREEN LED
	{ GPIOD, PORTD, 1,  kPORT_MuxAsGpio  }, // BLUE LED
};

#if (CLOCK_INIT_CONFIG == CLOCK_VLPR)
static void BOARD_BootClockVLPR(void) {
	/*
	* Core clock: 4MHz
	* Bus clock: 4MHz
	*/
	const sim_clock_config_t simConfig = {
		.pllFllSel = 0U,        /* PLLFLLSEL select FLL. */
		.er32kSrc = 3U,         /* ERCLK32K selection, use LPO. */
		.clkdiv1 = 0x00040000U, /* SIM_CLKDIV1. */
	};

	CLOCK_SetSimSafeDivs();

	CLOCK_BootToBlpiMode(0U, kMCG_IrcFast, kMCG_IrclkEnable);

	CLOCK_SetSimConfig(&simConfig);

	// Core clock = 4MHz
	SystemCoreClock = 4000000U;

	SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
	SMC_SetPowerModeVlpr(SMC);
	while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr) {
	}
}

#else

#define BOARD_XTAL0_CLK_HZ   8000000U
#define BOARD_XTAL32K_CLK_HZ 0U

static void BOARD_InitOsc0(void) {
	const osc_config_t oscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
									.capLoad = 0,
									.workMode = kOSC_ModeOscLowPower,
									.oscerConfig = {
										.enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                                        .erclkDiv = 0U,
#endif
									}};

	CLOCK_InitOsc0(&oscConfig);

	/* Passing the XTAL0 frequency to clock driver. */
	CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}

static void BOARD_BootClockRUN(void) {
	/*
	* Core clock: 48MHz
	* Bus clock: 24MHz
	*/
	const mcg_pll_config_t pll0Config = {
		.enableMode = 0U, .prdiv = 0x1U, .vdiv = 0x0U,
	};
	const sim_clock_config_t simConfig = {
		.pllFllSel = 1U,        /* PLLFLLSEL select PLL. */
		.er32kSrc = 3U,         /* ERCLK32K selection, use LPO. */
		.clkdiv1 = 0x10010000U, /* SIM_CLKDIV1. */
	};

	CLOCK_SetSimSafeDivs();
	BOARD_InitOsc0();

	CLOCK_BootToPeeMode(kMCG_OscselOsc, kMCG_PllClkSelPll0, &pll0Config);

	CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0);
	CLOCK_SetSimConfig(&simConfig);

	// Core clock = 48MHz
	SystemCoreClock = 48000000U;
}
#endif

static void configure_lpsci_pins(void) {
    //
	// Initialize LPUART0 pins
	//

    // Affects PORTA_PCR1 register
    PORT_SetPinMux(PORTA, 1u, kPORT_MuxAlt2);
    // Affects PORTA_PCR2 register
    PORT_SetPinMux(PORTA, 2u, kPORT_MuxAlt2);
}

// Initialize newlib
void hardware_init_hook(void) {
	int32_t i, ret;

#if (CLOCK_INIT_CONFIG == CLOCK_VLPR)
	BOARD_BootClockVLPR();
#else
	BOARD_BootClockRUN();
#endif

	//
	// Enable clock for PORTs
	//
	static const clock_ip_name_t ports[] = PORT_CLOCKS;
	for (i = 0; i < (sizeof(ports) / sizeof(clock_ip_name_t)); i++) {
		CLOCK_EnableClock(ports[i]);
	}

	//
	// Initialize Board LEDs
	//
	static const gpio_pin_config_t gpio_led_config = { kGPIO_DigitalOutput, LOGIC_LED_OFF };
	for (i = 0; i < LED_COUNT; i++) {
		// Set the pinmuxp for the LED
		PORT_SetPinMux(g_leds[i].port, g_leds[i].pin, g_leds[i].mux);

		// Configure GPIO
		GPIO_PinInit(g_leds[i].gpio, g_leds[i].pin, &gpio_led_config);
	}

	//
	// Initialize Debug UART Port
	//
#ifndef SUPPORT_DEBUG_UART_NONE
	configure_lpsci_pins();

	ret = Driver_UART_DEBUG.Initialize(NULL);
	assert(ret == ARM_DRIVER_OK);
#endif
}

void led_on(int led) {
	if (led < LED_COUNT) {
		GPIO_SetPinsOutput(g_leds[led].gpio, 1U << g_leds[led].pin);
	}
}

void led_off(int led) {
	if (led < LED_COUNT) {
		GPIO_ClearPinsOutput(g_leds[led].gpio, 1U << g_leds[led].pin);
	}
}

void led_toggle(int led) {
	if (led < LED_COUNT) {
		GPIO_TogglePinsOutput(g_leds[led].gpio, 1U << g_leds[led].pin);
	}
}

void led_set(int led, int value) {
	if (led < LED_COUNT) {
		GPIO_WritePinOutput(g_leds[led].gpio, 1U << g_leds[led].pin, value);
	}
}

int led_get(int led) {
	if (led < LED_COUNT) {
		return GPIO_ReadPinInput(g_leds[led].gpio, 1U << g_leds[led].pin);
	} else {
		return 0;
	}
}
