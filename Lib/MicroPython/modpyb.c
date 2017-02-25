/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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

#include "board.h"

#include "py/mpconfig.h"
#include "py/obj.h"
#include "extmod/utime_mphal.h"

#include "i2c.h"
#include "led.h"
#include "pin.h"

/// \function wfi()
/// Wait for an interrupt.
/// This executies a `wfi` instruction which reduces power consumption
/// of the MCU until an interrupt occurs, at which point execution continues.
STATIC mp_obj_t pyb_wfi(void) {
    __WFI();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(pyb_wfi_obj, pyb_wfi);

/// \function disable_irq()
/// Disable interrupt requests.
/// Returns the previous IRQ state: `False`/`True` for disabled/enabled IRQs
/// respectively.  This return value can be passed to enable_irq to restore
/// the IRQ to its original state.
STATIC mp_obj_t pyb_disable_irq(void) {
    return mp_obj_new_bool(disable_irq() == IRQ_STATE_ENABLED);
}
MP_DEFINE_CONST_FUN_OBJ_0(pyb_disable_irq_obj, pyb_disable_irq);

/// \function enable_irq(state=True)
/// Enable interrupt requests.
/// If `state` is `True` (the default value) then IRQs are enabled.
/// If `state` is `False` then IRQs are disabled.  The most common use of
/// this function is to pass it the value returned by `disable_irq` to
/// exit a critical section.
STATIC mp_obj_t pyb_enable_irq(uint n_args, const mp_obj_t *arg) {
    enable_irq((n_args == 0 || mp_obj_is_true(arg[0])) ? IRQ_STATE_ENABLED : IRQ_STATE_DISABLED);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pyb_enable_irq_obj, 0, 1, pyb_enable_irq);

// Resets the pyboard in a manner similar to pushing the external RESET button.
STATIC mp_obj_t pyb_hard_reset_obj(void) {
    NVIC_SystemReset();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(pyb_hard_reset_obj_obj, pyb_hard_reset_obj);

STATIC const mp_map_elem_t pyb_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_pyb) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_wfi), (mp_obj_t)&pyb_wfi_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_disable_irq), (mp_obj_t)&pyb_disable_irq_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_enable_irq), (mp_obj_t)&pyb_enable_irq_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hard_reset), (mp_obj_t)&pyb_hard_reset_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_delay), (mp_obj_t)&mp_utime_sleep_ms_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_udelay), (mp_obj_t)&mp_utime_sleep_us_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_LED), (mp_obj_t)&pyb_led_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Pin), (mp_obj_t)&pyb_pin_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_I2C), (mp_obj_t)&pyb_i2c_type },
};

STATIC MP_DEFINE_CONST_DICT(pyb_module_globals, pyb_module_globals_table);

const mp_obj_module_t pyb_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&pyb_module_globals,
};
