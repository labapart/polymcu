/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef NRF_ATOMIC_INTERNAL_H__
#define NRF_ATOMIC_INTERNAL_H__

#include "sdk_common.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @defgroup nrf_atomic_internal Atomic operations internals
 * @ingroup nrf_atomic
 * @{
 *
 */

/* Only Cortex M cores > 3 support LDREX/STREX instructions*/
#if ((__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U)) == 0
#error "Unsupported core version"
#endif

/**
 * @brief Atomic 32 bit unsigned type
 * */
typedef volatile uint32_t nrf_atomic_u32_t;

/**
 * @brief Atomic 1 bit flag type (technically 32 bit)
 * */
typedef volatile uint32_t nrf_atomic_flag_t;

#if defined ( __CC_ARM )
static __asm uint32_t nrf_atomic_internal_mov(nrf_atomic_u32_t * p_ptr,
                                              uint32_t value,
                                              uint32_t * p_new)
{
    /* The base standard provides for passing arguments in core registers (r0-r3) and on the stack.
     * Registers r4 and r5 have to be saved on stack. Note that only even number of register push are
     * allowed. This is a requirement of the Procedure Call Standard for the ARM Architecture [AAPCS].
     * */
    push  {r4, r5}
    mov   r4, r0

loop_mov
    ldrex r0, [r4]
    mov   r5, r1
    strex r3, r5, [r4]
    cmp   r3, #0
    bne   loop_mov

    str   r5, [r2]
    pop   {r4, r5}
    bx    lr
}


static __asm uint32_t nrf_atomic_internal_orr(nrf_atomic_u32_t * p_ptr,
                                              uint32_t value,
                                              uint32_t * p_new)
{
    push  {r4, r5}
    mov   r4, r0

loop_orr
    ldrex r0, [r4]
    orr   r5, r0, r1
    strex r3, r5, [r4]
    cmp   r3, #0
    bne   loop_orr

    str   r5, [r2]
    pop   {r4, r5}
    bx    lr
}

static __asm uint32_t nrf_atomic_internal_and(nrf_atomic_u32_t * p_ptr,
                                              uint32_t value,
                                              uint32_t * p_new)
{
    push  {r4, r5}
    mov   r4, r0

loop_and
    ldrex r0, [r4]
    and   r5, r0, r1
    strex r3, r5, [r4]
    cmp   r3, #0
    bne   loop_and

    str   r5, [r2]
    pop   {r4, r5}
    bx    lr
}

static __asm uint32_t nrf_atomic_internal_eor(nrf_atomic_u32_t * p_ptr,
                                              uint32_t value,
                                              uint32_t * p_new)
{
    push  {r4, r5}
    mov   r4, r0

loop_eor
    ldrex r0, [r4]
    eor   r5, r0, r1
    strex r3, r5, [r4]
    cmp   r3, #0
    bne   loop_eor

    str   r5, [r2]
    pop   {r4, r5}
    bx    lr
}

static __asm uint32_t nrf_atomic_internal_add(nrf_atomic_u32_t * p_ptr,
                                              uint32_t value,
                                              uint32_t * p_new)
{
    push  {r4, r5}
    mov   r4, r0

loop_add
    ldrex r0, [r4]
    add   r5, r0, r1
    strex r3, r5, [r4]
    cmp   r3, #0
    bne   loop_add

    str   r5, [r2]
    pop   {r4, r5}
    bx    lr
}

static __asm uint32_t nrf_atomic_internal_sub(nrf_atomic_u32_t * p_ptr,
                                              uint32_t value,
                                              uint32_t * p_new)
{
    push  {r4, r5}
    mov   r4, r0

loop_sub
    ldrex r0, [r4]
    sub   r5, r0, r1
    strex r3, r5, [r4]
    cmp   r3, #0
    bne   loop_sub

    str   r5, [r2]
    pop   {r4, r5}
    bx    lr
}


#define NRF_ATOMIC_OP(asm_op, old_val, new_val, ptr, value)          \
        old_val = nrf_atomic_internal_##asm_op(ptr, value, &new_val)

#elif defined ( __ICCARM__ ) || defined ( __GNUC__ )

/**
 * @brief Atomic operation generic macro
 * @param[in] asm_op operation: mov, orr, and, eor, add, sub
 * @param[out] old_val atomic object output (uint32_t), value before operation
 * @param[out] new_val atomic object output (uint32_t), value after operation
 * @param[in] value atomic operation operand
 * */
#define NRF_ATOMIC_OP(asm_op, old_val, new_val, ptr, value)                 \
{                                                                           \
    uint32_t str_res;                                                       \
            __ASM volatile(                                                 \
    "1:     ldrex   %["#old_val"], [%["#ptr"]]\n"                           \
    NRF_ATOMIC_OP_##asm_op(new_val, old_val, value)                         \
    "       strex   %[str_res], %["#new_val"], [%["#ptr"]]\n"               \
    "       teq     %[str_res], #0\n"                                       \
    "       bne.n     1b"                                                   \
            :                                                               \
        [old_val]"=&r" (old_val),                                           \
        [new_val]"=&r" (new_val),                                           \
        [str_res]"=&r" (str_res)                                            \
            :                                                               \
        [ptr]"r" (ptr),                                                     \
        [value]"r" (value)                                                  \
            : "cc");                                                        \
    UNUSED_PARAMETER(str_res);                                              \
}

#define NRF_ATOMIC_OP_mov(new_val, old_val, value) "mov %["#new_val"], %["#value"]\n"
#define NRF_ATOMIC_OP_orr(new_val, old_val, value) "orr %["#new_val"], %["#old_val"], %["#value"]\n"
#define NRF_ATOMIC_OP_and(new_val, old_val, value) "and %["#new_val"], %["#old_val"], %["#value"]\n"
#define NRF_ATOMIC_OP_eor(new_val, old_val, value) "eor %["#new_val"], %["#old_val"], %["#value"]\n"
#define NRF_ATOMIC_OP_add(new_val, old_val, value) "add %["#new_val"], %["#old_val"], %["#value"]\n"
#define NRF_ATOMIC_OP_sub(new_val, old_val, value) "sub %["#new_val"], %["#old_val"], %["#value"]\n"

#else
#error "Unsupported compiler"
#endif

#ifdef __cplusplus
}
#endif

#endif /* NRF_ATOMIC_INTERNAL_H__ */

/** @} */
