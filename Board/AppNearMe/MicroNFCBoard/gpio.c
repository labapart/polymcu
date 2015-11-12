/*
 * Copyright (c) 2015, Lab A Part
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
#include "PinNames.h"

#define LPC_IOCON0_BASE (LPC_IOCON_BASE)
#define LPC_IOCON1_BASE (LPC_IOCON_BASE + 0x60)

typedef struct {
    PinName  pin;
    uint32_t mask;

    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_set;
    __IO uint32_t *reg_clr;
    __I  uint32_t *reg_in;
} gpio_t;

static gpio_t led1;
static gpio_t led2;

static gpio_t* leds[] = {&led1, &led2, };

static inline void gpio_write(gpio_t *obj, int value) {
    if (value)
        *obj->reg_set = obj->mask;
    else
        *obj->reg_clr = obj->mask;
}

void pin_function(PinName pin, int function) {
    if (pin == (PinName)NC) return;

    uint32_t pin_number = (uint32_t)pin;

    __IO uint32_t *reg = (pin_number < 32) ?
                         (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * pin_number) :
                         (__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (pin_number - 32));

    // pin function bits: [2:0] -> 111 = (0x7)
    *reg = (*reg & ~0x7) | (function & 0x7);
}

void pin_mode(PinName pin, PinMode mode) {
    uint32_t pin_number = (uint32_t)pin;
    uint32_t drain = ((uint32_t) mode & (uint32_t) OpenDrain) >> 2;

    __IO uint32_t *reg = (pin_number < 32) ?
                         (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * pin_number) :
                         (__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (pin_number - 32));
    uint32_t tmp = *reg;

    // pin mode bits: [4:3] -> 11000 = (0x3 << 3)
    tmp &= ~(0x3 << 3);
    tmp |= (mode & 0x3) << 3;

    // drain
    tmp &= ~(0x1 << 10);
    tmp |= drain << 10;

    *reg = tmp;
}

uint32_t gpio_set(PinName pin) {
    int f = ((pin == P0_0)  ||
             (pin == P0_10) ||
             (pin == P0_11) ||
             (pin == P0_12) ||
             (pin == P0_13) ||
             (pin == P0_14) ||
             (pin == P0_15)) ? (1) : (0);

    pin_function(pin, f);

    return (1 << ((int)pin & 0x1F));
}

void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);

    unsigned int port = (unsigned int)pin >> PORT_SHIFT;

    obj->reg_set = &LPC_GPIO->SET[port];
    obj->reg_clr = &LPC_GPIO->CLR[port];
    obj->reg_in  = &LPC_GPIO->PIN[port];
    obj->reg_dir = &LPC_GPIO->DIR[port];
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    switch (direction) {
        case PIN_INPUT :
            *obj->reg_dir &= ~obj->mask;
            break;
        case PIN_OUTPUT:
            *obj->reg_dir |=  obj->mask;
            break;
    }
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

void leds_init(void)
{
  //Configure GPIOs as output
  gpio_init_out(&led1, /*P0_13*/P0_11);
  gpio_init_out(&led2, /*P0_14*/P0_12);
}

void set_led(int led, int value) {
  if(led < sizeof(leds)/sizeof(const gpio_t*))
  {
    gpio_write(leds[led], value);
  }
}
