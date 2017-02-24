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

#ifndef __PIN_H__
#define __PIN_H__

#include "py/obj.h"

#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_PIN_CNF_PULL_Disabled) || ((PULL) == GPIO_PIN_CNF_PULL_Pullup) || \
                            ((PULL) == GPIO_PIN_CNF_PULL_Pulldown))

extern const mp_obj_type_t pyb_pin_type;

#if 0
typedef struct {
  mp_obj_base_t base;
  qstr          name;
  uint8_t       fn;
  union {
    void*           reg;
    ARM_DRIVER_I2C* i2c;
  };
} pin_af_obj_t;
#endif

typedef struct {
	mp_obj_base_t base;
	qstr          name;
	uint32_t      pin_number;
} pin_obj_t;

typedef struct {
  const char      *name;
  const pin_obj_t *pin;
} pin_named_pin_t;

//extern const pin_named_pin_t g_pin_board_pins[];
//extern const pin_named_pin_t g_pin_cpu_pins[];

typedef struct {
    mp_obj_base_t base;
    qstr name;
    const pin_named_pin_t *named_pins;
} pin_named_pins_obj_t;

extern const mp_obj_type_t pin_board_pins_obj_type;
extern const mp_obj_type_t pin_cpu_pins_obj_type;

extern const mp_obj_dict_t pin_cpu_pins_locals_dict;
extern const mp_obj_dict_t pin_board_pins_locals_dict;

const pin_obj_t *pin_find_named_pin(const mp_obj_dict_t *named_pins, mp_obj_t name);
//const pin_af_obj_t *pin_find_af_by_index(const pin_obj_t *pin, mp_uint_t af_idx);

#endif
