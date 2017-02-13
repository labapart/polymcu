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


/**@file
 *
 * @defgroup nrf_atomic Atomic operations API
 * @ingroup app_atfifo
 * @{
 *
 * @brief @tagAPI52 This module implements C11 stdatomic.h simplified API.
          At this point only Cortex-M3/M4 cores are supported (LDREX/STREX instructions).
 *        Atomic types are limited to @ref nrf_atomic_u32_t and @ref nrf_atomic_flag_t.
 */

#ifndef NRF_ATOMIC_H__
#define NRF_ATOMIC_H__

#include "sdk_common.h"
#include "nrf_atomic_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Stores value to an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value to store
 *
 * @return Old value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_store_fetch(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(mov, old_val, new_val, p_data, value);

    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return old_val;
}

/**
 * @brief Stores value to an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value to store
 *
 * @return New value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_store(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(mov, old_val, new_val, p_data, value);

    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return new_val;
}

/**
 * @brief Logical OR operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand OR operation
 *
 * @return Old value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_or_fetch(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(orr, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return old_val;
}

/**
 * @brief Logical OR operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand OR operation
 *
 * @return New value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_or(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(orr, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return new_val;
}

/**
 * @brief Logical AND operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand AND operation
 *
 * @return Old value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_and_fetch(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(and, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return old_val;
}

/**
 * @brief Logical AND operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand AND operation
 *
 * @return New value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_and(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(and, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return new_val;
}

/**
 * @brief Logical XOR operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand XOR operation
 *
 * @return Old value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_xor_fetch(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(eor, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return old_val;
}

/**
 * @brief Logical XOR operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand XOR operation
 *
 * @return New value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_xor(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(eor, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return new_val;
}

/**
 * @brief Arithmetic ADD operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand ADD operation
 *
 * @return Old value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_add_fetch(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(add, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return old_val;
}

/**
 * @brief Arithmetic ADD operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand ADD operation
 *
 * @return New value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_add(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(add, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return new_val;
}

/**
 * @brief Arithmetic SUB operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand SUB operation
 *
 * @return Old value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_sub_fetch(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(sub, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return old_val;
}

/**
 * @brief Arithmetic SUB operation on an atomic object
 *
 * @param[in] p_data    Atomic memory pointer
 * @param[in] value     Value of second operand SUB operation
 *
 * @return New value stored into atomic object
 * */
static inline uint32_t nrf_atomic_u32_sub(nrf_atomic_u32_t * p_data, uint32_t value)
{
    uint32_t old_val;
    uint32_t new_val;

    NRF_ATOMIC_OP(sub, old_val, new_val, p_data, value);
    UNUSED_PARAMETER(old_val);
    UNUSED_PARAMETER(new_val);
    return new_val;
}

/**************************************************************************************************/

/**
 * @brief Logic one bit flag set operation on an atomic object
 *
 * @param[in] p_data    Atomic flag memory pointer
 *
 * @return Old flag value
 * */
static inline uint32_t nrf_atomic_flag_set_fetch(nrf_atomic_flag_t * p_data)
{
    return nrf_atomic_u32_or_fetch(p_data, 1);
}

/**
 * @brief Logic one bit flag set operation on an atomic object
 *
 * @param[in] p_data    Atomic flag memory pointer
 *
 * @return New flag value
 * */
static inline uint32_t nrf_atomic_flag_set(nrf_atomic_flag_t * p_data)
{
    return nrf_atomic_u32_or(p_data, 1);
}

/**
 * @brief Logic one bit flag clear operation on an atomic object
 *
 * @param[in] p_data    Atomic flag memory pointer
 *
 * @return Old flag value
 * */
static inline uint32_t nrf_atomic_flag_clear_fetch(nrf_atomic_flag_t * p_data)
{
    return nrf_atomic_u32_and_fetch(p_data, 0);
}

/**
 * @brief Logic one bit flag clear operation on an atomic object
 *
 * @param[in] p_data    Atomic flag memory pointer
 *
 * @return New flag value
 * */
static inline uint32_t nrf_atomic_flag_clear(nrf_atomic_flag_t * p_data)
{
    return nrf_atomic_u32_and(p_data, 0);
}

#ifdef __cplusplus
}
#endif

#endif /* NRF_ATOMIC_H__ */

/** @} */
