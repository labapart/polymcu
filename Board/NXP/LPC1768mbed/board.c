/*
 * Copyright (c) 2015-2016, Lab A Part
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

#include "internal.h"

#define LED_COUNT  4

static const int g_led_gpio_port_num[LED_COUNT] = { 1,   1,  1,  1 };
static const int g_led_gpio_bit_num[LED_COUNT]  = { 18, 20, 21, 23 };

extern const ARM_DRIVER_USART Driver_UART_DEBUG;

/* Pin muxing configuration */
static const PINMUX_GRP_T pinmuxing[] = {
	{0,  2,   IOCON_MODE_INACT | IOCON_FUNC1},	/* P0.2: USBTX */
	{0,  3,   IOCON_MODE_INACT | IOCON_FUNC1},	/* P0.3: USBRX */
	{1, 18,   IOCON_MODE_PULLUP | IOCON_FUNC0},	/* P1.18: GPIO/LED1 */
	{1, 20,   IOCON_MODE_PULLUP | IOCON_FUNC0},	/* P1.20: GPIO/LED2 */
	{1, 21,   IOCON_MODE_PULLUP | IOCON_FUNC0},	/* P1.21: GPIO/LED3 */
	{1, 23,   IOCON_MODE_PULLUP | IOCON_FUNC0},	/* P1.23: GPIO/LED4 */
};

// Initialize newlib
void hardware_init_hook(void) {
	int i;

	// Setup pin muxings
	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

	// Initialize Board LEDs
	for (i = 0; i < LED_COUNT; i++) {
		Chip_GPIO_WriteDirBit(LPC_GPIO, g_led_gpio_port_num[i], g_led_gpio_bit_num[i], true);
	}

#ifndef SUPPORT_DEBUG_UART_NONE
	// Initialize UART
	Driver_UART_DEBUG.Initialize(NULL);
#endif

#ifdef SUPPORT_DEVICE_USB
	// Initialize USBD ROM
	usbd_rom_init(usb_interface_inits, ep_count);
#endif

	// Ensure SystemCoreClock is set
	SystemCoreClockUpdate();
}

void set_led(int led, int value) {
	if(led < LED_COUNT) {
		Chip_GPIO_WritePortBit(LPC_GPIO, g_led_gpio_port_num[led], g_led_gpio_bit_num[led], value);
	}
}

void __libc_init_array(void) {
	// Do nothing. NXP LPCOpen already initialized the .data and .bss segments
}
