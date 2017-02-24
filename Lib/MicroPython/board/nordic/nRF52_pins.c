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

#include <stdio.h>

#include "py/obj.h"
#include "py/mphal.h"
#include "pin.h"

#define AF(af_idx, af_fn, af_unit, af_type, af_ptr) \
{ \
    { &pin_af_type }, \
    .name = MP_QSTR_AF ## af_idx ## _ ## af_fn ## af_unit, \
    .idx = (af_idx), \
    .fn = AF_FN_ ## af_fn, \
    .unit = (af_unit), \
    .type = AF_PIN_TYPE_ ## af_fn ## _ ## af_type, \
    .af_fn = (af_ptr) \
}

#define PIN(p_pin) \
{ \
    { &pyb_pin_type }, \
    .name = MP_QSTR_ ## p_pin, \
    .pin_number = (p_pin), \
}

const pin_obj_t pin_P0 = PIN(0);
const pin_obj_t pin_P1 = PIN(1);
const pin_obj_t pin_P2 = PIN(2);
const pin_obj_t pin_P3 = PIN(3);
const pin_obj_t pin_P4 = PIN(4);
const pin_obj_t pin_P5 = PIN(5);
const pin_obj_t pin_P6 = PIN(6);
const pin_obj_t pin_P7 = PIN(7);
const pin_obj_t pin_P8 = PIN(8);
const pin_obj_t pin_P9 = PIN(9);
const pin_obj_t pin_P10 = PIN(10);
const pin_obj_t pin_P11 = PIN(11);
const pin_obj_t pin_P12 = PIN(12);
const pin_obj_t pin_P13 = PIN(13);
const pin_obj_t pin_P14 = PIN(14);
const pin_obj_t pin_P15 = PIN(15);
const pin_obj_t pin_P16 = PIN(16);
const pin_obj_t pin_P17 = PIN(17);
const pin_obj_t pin_P18 = PIN(18);
const pin_obj_t pin_P19 = PIN(19);
const pin_obj_t pin_P20 = PIN(20);
const pin_obj_t pin_P21 = PIN(21);
const pin_obj_t pin_P22 = PIN(22);
const pin_obj_t pin_P23 = PIN(23);
const pin_obj_t pin_P24 = PIN(24);
const pin_obj_t pin_P25 = PIN(25);
const pin_obj_t pin_P26 = PIN(26);
const pin_obj_t pin_P27 = PIN(27);

STATIC const mp_map_elem_t pin_cpu_pins_locals_dict_table[] = {
  { MP_OBJ_NEW_QSTR(MP_QSTR_P0), (mp_obj_t)&pin_P0 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P1), (mp_obj_t)&pin_P1 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P2), (mp_obj_t)&pin_P2 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P3), (mp_obj_t)&pin_P3 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P4), (mp_obj_t)&pin_P4 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P5), (mp_obj_t)&pin_P5 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P6), (mp_obj_t)&pin_P6 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P7), (mp_obj_t)&pin_P7 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P8), (mp_obj_t)&pin_P8 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P9), (mp_obj_t)&pin_P9 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P10), (mp_obj_t)&pin_P10 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P11), (mp_obj_t)&pin_P11 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P12), (mp_obj_t)&pin_P12 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P13), (mp_obj_t)&pin_P13 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P14), (mp_obj_t)&pin_P14 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P15), (mp_obj_t)&pin_P15 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P16), (mp_obj_t)&pin_P16 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P17), (mp_obj_t)&pin_P17 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P18), (mp_obj_t)&pin_P18 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P19), (mp_obj_t)&pin_P19 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P20), (mp_obj_t)&pin_P20 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P21), (mp_obj_t)&pin_P21 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P22), (mp_obj_t)&pin_P22 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P23), (mp_obj_t)&pin_P23 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P24), (mp_obj_t)&pin_P24 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P25), (mp_obj_t)&pin_P25 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P26), (mp_obj_t)&pin_P26 },
  { MP_OBJ_NEW_QSTR(MP_QSTR_P27), (mp_obj_t)&pin_P27 },
};
MP_DEFINE_CONST_DICT(pin_cpu_pins_locals_dict, pin_cpu_pins_locals_dict_table);

STATIC const mp_map_elem_t pin_board_pins_locals_dict_table[] = {
};
MP_DEFINE_CONST_DICT(pin_board_pins_locals_dict, pin_board_pins_locals_dict_table);
