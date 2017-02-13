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

#ifndef NRF_ATOMIC_SANITY_CHECK_H__
#define NRF_ATOMIC_SANITY_CHECK_H__

#include "nrf_atomic.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Quick sanity check of nrf_atomic API
 * */
static inline void nrf_atomic_sanity_check(void)
{
#if defined(DEBUG_NRF) || defined(DEBUG_NRF_USER)
    nrf_atomic_u32_t val;
    nrf_atomic_u32_t flag;

    /*Fetch version tests*/
    val = 0;
    ASSERT(nrf_atomic_u32_store_fetch(&val, 10) == 0);
    ASSERT(nrf_atomic_u32_store_fetch(&val, 0) == 10);

    val = 0;
    ASSERT(nrf_atomic_u32_or_fetch(&val, 1 << 16) == 0);
    ASSERT(nrf_atomic_u32_or_fetch(&val, 1 << 5) == ((1 << 16)));
    ASSERT(nrf_atomic_u32_or_fetch(&val, 1 << 5) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_or_fetch(&val, 0) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_or_fetch(&val, 0xFFFFFFFF) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_or_fetch(&val, 0xFFFFFFFF) == (0xFFFFFFFF));

    val = 0xFFFFFFFF;
    ASSERT(nrf_atomic_u32_and_fetch(&val, ~(1 << 16)) == 0xFFFFFFFF);
    ASSERT(nrf_atomic_u32_and_fetch(&val, ~(1 << 5)) == (0xFFFFFFFF & ~((1 << 16))));
    ASSERT(nrf_atomic_u32_and_fetch(&val, 0) == (0xFFFFFFFF & ~(((1 << 16) | (1 << 5)))));
    ASSERT(nrf_atomic_u32_and_fetch(&val, 0xFFFFFFFF) == (0));

    val = 0;
    ASSERT(nrf_atomic_u32_xor_fetch(&val, (1 << 16)) == 0);
    ASSERT(nrf_atomic_u32_xor_fetch(&val, (1 << 5)) == ((1 << 16)));
    ASSERT(nrf_atomic_u32_xor_fetch(&val, 0) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_xor_fetch(&val, (1 << 16) | (1 << 5)) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_xor_fetch(&val, 0) == (0));

    val = 0;
    ASSERT(nrf_atomic_u32_add_fetch(&val, 100) == 0);
    ASSERT(nrf_atomic_u32_add_fetch(&val, 100) == 100);
    ASSERT(nrf_atomic_u32_add_fetch(&val, 1 << 24) == 200);
    ASSERT(nrf_atomic_u32_add_fetch(&val, 0) == (200 + (1 << 24)));
    ASSERT(nrf_atomic_u32_add_fetch(&val, 0xFFFFFFFF) == (200 + (1 << 24)));
    ASSERT(nrf_atomic_u32_add_fetch(&val, 0) == (200 - 1 + (1 << 24)));

    val = 1000;
    ASSERT(nrf_atomic_u32_sub_fetch(&val, 100) == 1000);
    ASSERT(nrf_atomic_u32_sub_fetch(&val, 100) == 900);
    ASSERT(nrf_atomic_u32_sub_fetch(&val, 0) == 800);
    ASSERT(nrf_atomic_u32_sub_fetch(&val, 0xFFFFFFFF) == 800);
    ASSERT(nrf_atomic_u32_sub_fetch(&val, 0) == 801);

    flag = 0;
    ASSERT(nrf_atomic_flag_set_fetch(&flag) == 0);
    ASSERT(nrf_atomic_flag_set_fetch(&flag) == 1);
    ASSERT(nrf_atomic_flag_clear_fetch(&flag) == 1);
    ASSERT(nrf_atomic_flag_clear_fetch(&flag) == 0);

    /*No fetch version tests*/
    val = 0;
    ASSERT(nrf_atomic_u32_store(&val, 10) == 10);
    ASSERT(nrf_atomic_u32_store(&val, 0) == 0);

    val = 0;
    ASSERT(nrf_atomic_u32_or(&val, 1 << 16) == 1 << 16);
    ASSERT(nrf_atomic_u32_or(&val, 1 << 5) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_or(&val, 1 << 5) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_or(&val, 0) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_or(&val, 0xFFFFFFFF) == 0xFFFFFFFF);

    val = 0xFFFFFFFF;
    ASSERT(nrf_atomic_u32_and(&val, ~(1 << 16)) == (0xFFFFFFFF & ~((1 << 16))));
    ASSERT(nrf_atomic_u32_and(&val, ~(1 << 5)) == (0xFFFFFFFF & ~(((1 << 16) | (1 << 5)))));
    ASSERT(nrf_atomic_u32_and(&val, 0) == 0);

    val = 0;
    ASSERT(nrf_atomic_u32_xor(&val, (1 << 16)) == ((1 << 16)));
    ASSERT(nrf_atomic_u32_xor(&val, (1 << 5)) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_xor(&val, 0) == ((1 << 16) | (1 << 5)));
    ASSERT(nrf_atomic_u32_xor(&val, (1 << 16) | (1 << 5)) == 0);

    val = 0;
    ASSERT(nrf_atomic_u32_add(&val, 100) == 100);
    ASSERT(nrf_atomic_u32_add(&val, 100) == 200);
    ASSERT(nrf_atomic_u32_add(&val, 1 << 24) == (200 + (1 << 24)));
    ASSERT(nrf_atomic_u32_add(&val, 0) == (200 + (1 << 24)));
    ASSERT(nrf_atomic_u32_add(&val, 0xFFFFFFFF) == (200 - 1 + (1 << 24)));

    val = 1000;
    ASSERT(nrf_atomic_u32_sub(&val, 100) == 900);
    ASSERT(nrf_atomic_u32_sub(&val, 100) == 800);
    ASSERT(nrf_atomic_u32_sub(&val, 0) == 800);
    ASSERT(nrf_atomic_u32_sub(&val, 0xFFFFFFFF) == 801);

    flag = 0;
    ASSERT(nrf_atomic_flag_set(&flag) == 1);
    ASSERT(nrf_atomic_flag_set(&flag) == 1);
    ASSERT(nrf_atomic_flag_clear(&flag) == 0);
    ASSERT(nrf_atomic_flag_clear(&flag) == 0);
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* NRF_ATOMIC_SANITY_CHECK_H__ */
