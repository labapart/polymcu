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

#include "board.h"
#include "arch/lpm_arch.h"

/*
 * Newlib software hook
 */
__attribute__((naked)) void software_init_hook(void) {
  __asm (
    ".syntax unified\n"
    ".thumb\n"
    "movs r0,#0\n"
    "movs r1,#0\n"
    "mov  r4,r0\n"
    "mov  r5,r1\n"
    "ldr  r0,= __libc_fini_array\n"
    "bl   atexit\n"
    "bl   __libc_init_array\n"
    "bl   kernel_init\n"
    "bl   exit\n"
  );
}

/**
 * @brief Try to set the controller to a given power mode
 *
 * @param[in] target    the desired power mode
 *
 * @return              the power mode that was actually set
 */
__attribute__((weak)) enum lpm_mode lpm_arch_set(enum lpm_mode target)
{
    switch (target) {
        /* wait for next interrupt */
        case LPM_IDLE:
        case LPM_SLEEP:
        case LPM_POWERDOWN:
        case LPM_OFF:
            __DSB();
            __WFI();
            break;

        /* do nothing here */
        case LPM_UNKNOWN:
        case LPM_ON:
        default:
            break;
    }
    return 0;
}

__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile (
    "b  isr_svc               \n"
    );
}

__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile (
    "b  isr_pendsv            \n"
    );
}
