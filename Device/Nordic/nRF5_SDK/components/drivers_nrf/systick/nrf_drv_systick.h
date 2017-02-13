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
#ifndef NRF_DRV_SYSTICK_H__
#define NRF_DRV_SYSTICK_H__
#include <stdint.h>
#include <stdbool.h>


/**
 * @addtogroup nrf_systick SysTick HAL and driver
 * @ingroup nrf_drivers
 * @brief System Timer (SysTick) APIs
 *
 * The SysTick HAL provides basic APIs for accessing the registers of the system timer (SysTick).
 * The SysTick driver provides APIs on a higher level.
 */

/**
 * @defgroup nrf_drv_systick SysTick driver
 * @{
 * @ingroup nrf_systick
 *
 * This library configures SysTick as a free-running timer.
 * This timer is used to generate delays and pool for timeouts.
 * Only relatively short timeouts are supported.
 * The SysTick works on 64MHz and is 24-bits wide.
 * It means that it overflows around 4 times per second and around 250&nbsp;ms
 * would be the highest supported time in the library.
 * But it would be really hard to detect if overflow was generated without
 * using interrupts. For safely we would limit the maximum delay range by half.
 */

/**
 * @brief The value type that holds the SysTick state
 *
 * This variable is used to count the requested timeout.
 * @sa nrf_drv_systick_get
 */
typedef struct {
    uint32_t time; //!< Registered time value
}nrf_drv_systick_state_t;

/**
 * @brief Configure and start the timer
 *
 * Function configures SysTick as a free-running timer without interrupt.
 */
void nrf_drv_systick_init(void);

/**
 * @brief Get current SysTick state
 *
 * Function gets current state of the SysTick timer.
 * It can be used to check time-out by @ref nrf_drv_systick_test.
 *
 * @param[out] p_state The pointer to the state variable to be filled
 */
void nrf_drv_systick_get(nrf_drv_systick_state_t * p_state);

/**
 * @brief Test if specified time is up in relation to remembered state
 *
 * @param[in] p_state Remembered state set by @ref nrf_drv_systick_get
 * @param[in] us      Required time-out.
 *
 * @retval true  If current time is higher than specified state plus given time-out.
 * @retval false If current time is lower than specified state plus given time-out
 */
bool nrf_drv_systick_test(nrf_drv_systick_state_t const * p_state, uint32_t us);

/**
 * @brief Blocking delay in CPU ticks
 *
 * @param[in] ticks Number of CPU ticks to delay.
 */
void nrf_drv_systick_delay_ticks(uint32_t ticks);

/**
 * @brief Blocking delay in us
 *
 * @param[in] us Number of microseconds to delay.
 */
void nrf_drv_systick_delay_us(uint32_t us);

/**
 * @brief Blocking delay in ms
 *
 * This delay function removes the limits of the highest possible delay value.
 *
 * @param[in] ms Number of milliseconds to delay.
 */
void nrf_drv_systick_delay_ms(uint32_t ms);


/** @} */
#endif /* NRF_DRV_SYSTICK_H__ */
