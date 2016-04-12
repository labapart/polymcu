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

typedef struct {
    PinName  pin;
    uint32_t mask;

    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_set;
    __IO uint32_t *reg_clr;
    __I  uint32_t *reg_in;
} gpio_t;

extern const ARM_DRIVER_USART Driver_UART_DEBUG;

static gpio_t leds[4];

uint32_t gpio_set(PinName pin) {
    return (1 << ((int)pin & 0x1F));
}

void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);
    LPC_GPIO_T *port_reg = (LPC_GPIO_T *)((int)pin & ~0x1F);
    obj->reg_set = &port_reg->SET;
    obj->reg_clr = &port_reg->CLR;
    obj->reg_in  = &port_reg->PIN;
    obj->reg_dir = &port_reg->DIR;
}

void gpio_mode(gpio_t *obj, PinMode mode) {
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    switch (direction) {
        case PIN_INPUT :
            *obj->reg_dir &= ~obj->mask;
            break;
        case PIN_OUTPUT:
            *obj->reg_dir |= obj->mask;
            break;
    }
}

void gpio_write(gpio_t *obj, int value) {
    if (value)
        *obj->reg_set = obj->mask;
    else
        *obj->reg_clr = obj->mask;
}

static inline void _gpio_init_out(gpio_t* gpio, PinName pin, PinMode mode, int value)
{
    gpio_init(gpio, pin);
    if (pin != NC) {
        gpio_write(gpio, value);
        gpio_dir(gpio, PIN_OUTPUT);
        gpio_mode(gpio, mode);
    }
}

void gpio_init_out_ex(gpio_t* gpio, PinName pin, int value) {
    _gpio_init_out(gpio, pin, PullNone, value);
}

void gpio_init_out(gpio_t* gpio, PinName pin) {
    gpio_init_out_ex(gpio, pin, 0);
}

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
	// Setup pin muxings
	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

	//
	// Initialize Board LED
	//

	//Configure GPIOs as output
	gpio_init_out(&leds[0], LED1);
	gpio_init_out(&leds[1], LED2);
	gpio_init_out(&leds[2], LED3);
	gpio_init_out(&leds[3], LED4);

#if 0
	/* Pin PIO0_22 is configured as GPIO pin during SystemInit */
	/* Set the PIO_22 as output */
	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	Chip_GPIO_WriteDirBit(LPC_GPIO, 1, 1 << (LED1 & 0x1F), 1);
	Chip_GPIO_WritePortBit(LPC_GPIO, 1, 1 << (LED1 & 0x1F), 1);
#endif

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
	if(led < sizeof(leds)/sizeof(gpio_t)) {
		gpio_write(&leds[led], value);
	}
}

void __libc_init_array(void) {
	// Do nothing. NXP LPCOpen already initialized the .data and .bss segments
}
