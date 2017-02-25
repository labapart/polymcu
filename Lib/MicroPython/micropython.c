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

#include "Driver_Common.h"
#include "py/gc.h"
#include "py/runtime.h"
#include "py/mperrno.h"

void gc_collect(void) {
#if 0
	// WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
#endif
}

// This table converts from ARM CMSIS to POSIX errno
static const byte mp_hal_status_to_errno_table[7] = {
    [ARM_DRIVER_OK] = 0,
    [-ARM_DRIVER_ERROR] = MP_EIO,
    [-ARM_DRIVER_ERROR_BUSY] = MP_EBUSY,
    [-ARM_DRIVER_ERROR_TIMEOUT] = MP_ETIMEDOUT,
    [-ARM_DRIVER_ERROR_UNSUPPORTED] = MP_EIO, //TODO: Find better error code
    [-ARM_DRIVER_ERROR_PARAMETER] = MP_EIO, //TODO: Find better error code
    [-ARM_DRIVER_ERROR_SPECIFIC] = MP_EIO //TODO: Find better error code
};

NORETURN void mp_hal_cmsis_raise(int32_t status) {
    mp_raise_OSError(mp_hal_status_to_errno_table[-status]);
}
