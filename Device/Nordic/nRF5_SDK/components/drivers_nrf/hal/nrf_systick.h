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
#ifndef NRF_SYSTICK_H__
#define NRF_SYSTICK_H__

#include "nrf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @defgroup nrf_systick_hal SYSTICK HAL
 * @{
 * @ingroup nrf_systick
 *
 * @brief Hardware access layer for accessing the SYSTICK peripheral.
 *
 * SYSTICK is ARM peripheral, not Nordic design.
 * It means that it has no Nordic-typical interface with Tasks and Events.
 *
 * Its usage is limited here to implement simple delays.
 * Also keep in mind that this timer would be stopped when CPU is sleeping
 * (WFE/WFI instruction is successfully executed).
 */

/**
 * @brief Mask of usable bits in the SysTick value
 */
#define NRF_SYSTICK_VAL_MASK SysTick_VAL_CURRENT_Msk

/**
 * @brief Flags used by SysTick configuration.
 *
 * @sa nrf_systick_csr_set
 * @sa nrf_systick_csr_get
 */
typedef enum {
    NRF_SYSTICK_CSR_COUNTFLAG_MASK  = SysTick_CTRL_COUNTFLAG_Msk,       /**< Status flag: Returns 1 if timer counted to 0 since the last read of this register. */

    NRF_SYSTICK_CSR_CLKSOURCE_MASK  = SysTick_CTRL_CLKSOURCE_Msk,       /**< Configuration bit: Select the SysTick clock source. */
    NRF_SYSTICK_CSR_CLKSOURCE_REF   = 0U << SysTick_CTRL_CLKSOURCE_Pos, /**< Configuration value: Select reference clock. */
    NRF_SYSTICK_CSR_CLKSOURCE_CPU   = 1U << SysTick_CTRL_CLKSOURCE_Pos, /**< Configuration value: Select CPU clock. */

    NRF_SYSTICK_CSR_TICKINT_MASK    = SysTick_CTRL_TICKINT_Msk,         /**< Configuration bit: Enables SysTick exception request. */
    NRF_SYSTICK_CSR_TICKINT_ENABLE  = 1U << SysTick_CTRL_TICKINT_Pos,   /**< Configuration value: Counting down to zero does not assert the SysTick exception request. */
    NRF_SYSTICK_CSR_TICKINT_DISABLE = 0U << SysTick_CTRL_TICKINT_Pos,   /**< Configuration value: Counting down to zero to asserts the SysTick exception request. */

    NRF_SYSTICK_CSR_ENABLE_MASK     = SysTick_CTRL_ENABLE_Msk,          /**< Configuration bit: Enable the SysTick timer. */
    NRF_SYSTICK_CSR_ENABLE          = 1U << SysTick_CTRL_ENABLE_Pos,    /**< Configuration value: Counter enabled. */
    NRF_SYSTICK_CSR_DISABLE         = 0U << SysTick_CTRL_ENABLE_Pos     /**< Configuration value: Counter disabled. */
} nrf_systick_csr_flags_t;

/**
 * @brief Get Configuration and Status Register
 *
 * @return Values composed by @ref nrf_systick_csr_flags_t.
 * @note The @ref NRF_SYSTICK_CSR_COUNTFLAG_MASK value is cleared when CSR register is read.
 */
__STATIC_INLINE uint32_t nrf_systick_csr_get(void);

/**
 * @brief Set Configuration and Status Register
 *
 * @param[in] val The value composed from @ref nrf_systick_csr_flags_t.
 */
__STATIC_INLINE void nrf_systick_csr_set(uint32_t val);

/**
 * @brief Get the current reload value.
 *
 * @return The reload register value.
 */
__STATIC_INLINE uint32_t nrf_systick_load_get(void);

/**
 * @brief Configure the reload value.
 *
 * @param[in] val The value to set in the reload register.
 */
__STATIC_INLINE void nrf_systick_load_set(uint32_t val);

/**
 * @brief Read the SysTick current value
 *
 * @return The current SysTick value
 * @sa NRF_SYSTICK_VAL_MASK
 */
__STATIC_INLINE uint32_t nrf_systick_val_get(void);

/**
 * @brief Clear the SysTick current value
 *
 * @note The SysTick does not allow setting current value.
 *       Any write to VAL register would clear the timer.
 */
__STATIC_INLINE void nrf_systick_val_clear(void);

/**
 * @brief Read the calibration register
 *
 * @return The calibration register value
 */
__STATIC_INLINE uint32_t nrf_systick_calib_get(void);



#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE uint32_t nrf_systick_csr_get(void)
{
    return SysTick->CTRL;
}

__STATIC_INLINE void nrf_systick_csr_set(uint32_t val)
{
    SysTick->CTRL = val;
}

__STATIC_INLINE uint32_t nrf_systick_load_get(void)
{
    return SysTick->LOAD;
}

__STATIC_INLINE void nrf_systick_load_set(uint32_t val)
{
    SysTick->LOAD = val;
}

__STATIC_INLINE uint32_t nrf_systick_val_get(void)
{
    return SysTick->VAL;
}

__STATIC_INLINE void nrf_systick_val_clear(void)
{
    SysTick->VAL = 0;
}

__STATIC_INLINE uint32_t nrf_systick_calib_get(void)
{
    return SysTick->CALIB;
}

#endif /* SUPPRESS_INLINE_IMPLEMENTATION */

/** @} */
#endif /* NRF_SYSTICK_H__ */
