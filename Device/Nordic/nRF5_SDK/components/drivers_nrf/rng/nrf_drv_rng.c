/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#include "sdk_common.h"
#if NRF_MODULE_ENABLED(RNG)

#include <stdint.h>
#include <stddef.h>
#include "nrf_drv_rng.h"
#include "nrf_drv_common.h"
#include "nordic_common.h"
#include "nrf_assert.h"
#include "nrf_queue.h"

#ifdef SOFTDEVICE_PRESENT
    #include "softdevice_handler.h"
    #include "nrf_soc.h"
    #include "app_util_platform.h"
#endif // SOFTDEVICE_PRESENT

#define NRF_LOG_MODULE_NAME "RNG"

#if RNG_CONFIG_LOG_ENABLED
    #define NRF_LOG_LEVEL       RNG_CONFIG_LOG_LEVEL
    #define NRF_LOG_INFO_COLOR  RNG_CONFIG_INFO_COLOR
    #define NRF_LOG_DEBUG_COLOR RNG_CONFIG_DEBUG_COLOR
#else //RNG_CONFIG_LOG_ENABLED
    #define NRF_LOG_LEVEL       0
#endif //RNG_CONFIG_LOG_ENABLED
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

/* Validate configuration */
INTERRUPT_PRIORITY_VALIDATION(RNG_CONFIG_IRQ_PRIORITY);

typedef struct
{
    nrf_drv_state_t      state;
    nrf_drv_rng_config_t config;
} nrf_drv_rng_cb_t;

static nrf_drv_rng_cb_t m_rng_cb;
NRF_QUEUE_DEF(uint8_t, m_rand_pool, RNG_CONFIG_POOL_SIZE, NRF_QUEUE_MODE_OVERFLOW);
static const nrf_drv_rng_config_t m_default_config = NRF_DRV_RNG_DEFAULT_CONFIG;

#ifdef SOFTDEVICE_PRESENT
    #define SD_RAND_POOL_SIZE           (32)
    STATIC_ASSERT(RNG_CONFIG_POOL_SIZE == SD_RAND_POOL_SIZE);

    #define NRF_DRV_RNG_LOCK()          CRITICAL_REGION_ENTER()
    #define NRF_DRV_RNG_RELEASE()       CRITICAL_REGION_EXIT()
    #define NRF_DRV_RNG_SD_IS_ENABLED() softdevice_handler_is_enabled()
#else
    #define NRF_DRV_RNG_LOCK()          do { } while (0)
    #define NRF_DRV_RNG_RELEASE()       do { } while (0)
    #define NRF_DRV_RNG_SD_IS_ENABLED() false
#endif // SOFTDEVICE_PRESENT

/**
 * @brief Function for starting generation.
 */
static void nrf_drv_rng_start(void)
{
    ASSERT(!NRF_DRV_RNG_SD_IS_ENABLED());

    nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);
    nrf_rng_int_enable(NRF_RNG_INT_VALRDY_MASK);
    nrf_rng_task_trigger(NRF_RNG_TASK_START);
}

/**
 * @brief Function for stoping generation.
 */
static void nrf_drv_rng_stop(void)
{
    ASSERT(!NRF_DRV_RNG_SD_IS_ENABLED());

    nrf_rng_int_disable(NRF_RNG_INT_VALRDY_MASK);
    nrf_rng_task_trigger(NRF_RNG_TASK_STOP);
}

/**
 * @brief Function for setting up RNG hardware.
 */
static void nrf_drv_rng_setup(void)
{
    ASSERT(!NRF_DRV_RNG_SD_IS_ENABLED());

    if (m_rng_cb.config.error_correction)
    {
        nrf_rng_error_correction_enable();
    }
    nrf_rng_shorts_disable(NRF_RNG_SHORT_VALRDY_STOP_MASK);
    nrf_drv_common_irq_enable(RNG_IRQn, m_rng_cb.config.interrupt_priority);
}

ret_code_t nrf_drv_rng_init(nrf_drv_rng_config_t const * p_config)
{
    if (m_rng_cb.state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_MODULE_ALREADY_INITIALIZED;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config;
    }
    m_rng_cb.config = *p_config;

    NRF_DRV_RNG_LOCK();

    if (!NRF_DRV_RNG_SD_IS_ENABLED())
    {
        nrf_drv_rng_setup();
        nrf_drv_rng_start();
    }

    NRF_DRV_RNG_RELEASE();

    m_rng_cb.state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

void nrf_drv_rng_uninit(void)
{
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

    NRF_DRV_RNG_LOCK();

    if (!NRF_DRV_RNG_SD_IS_ENABLED())
    {
        nrf_drv_rng_stop();
        nrf_drv_common_irq_disable(RNG_IRQn);
    }

    NRF_DRV_RNG_RELEASE();

    nrf_queue_reset(&m_rand_pool);
    m_rng_cb.state = NRF_DRV_STATE_UNINITIALIZED;
    NRF_LOG_INFO("Uninitialized.\r\n");
}

void nrf_drv_rng_bytes_available(uint8_t * p_bytes_available)
{
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

#ifdef SOFTDEVICE_PRESENT
    if (NRF_DRV_RNG_SD_IS_ENABLED())
    {
        if (NRF_SUCCESS == sd_rand_application_bytes_available_get(p_bytes_available))
        {
            return;
        }
    }
#endif // SOFTDEVICE_PRESENT

    *p_bytes_available  = nrf_queue_utilization_get(&m_rand_pool);

    NRF_LOG_INFO("Function: %s, available bytes: %d.\r\n", (uint32_t)__func__, *p_bytes_available);
}

ret_code_t nrf_drv_rng_rand(uint8_t * p_buff, uint8_t length)
{
    ret_code_t err_code = NRF_SUCCESS;
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

#ifdef SOFTDEVICE_PRESENT
    do {
        bool sd_is_enabled;
        NRF_DRV_RNG_LOCK();
        sd_is_enabled = NRF_DRV_RNG_SD_IS_ENABLED();
        if (!sd_is_enabled)
#endif // SOFTDEVICE_PRESENT
        {
            err_code = nrf_queue_read(&m_rand_pool, p_buff, (uint32_t)length);
            nrf_drv_rng_start();
        }
#ifdef SOFTDEVICE_PRESENT
        NRF_DRV_RNG_RELEASE();

        if (sd_is_enabled)
        {
            err_code = sd_rand_application_vector_get(p_buff, length);
            if (err_code == NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES)
            {
                err_code = NRF_ERROR_NOT_FOUND;
            }
        }
    } while (err_code == NRF_ERROR_SOFTDEVICE_NOT_ENABLED);
#endif // SOFTDEVICE_PRESENT
    ASSERT((err_code == NRF_SUCCESS) || (err_code == NRF_ERROR_NOT_FOUND));

    NRF_LOG_DEBUG("Rand buffer data:\r\n");
    NRF_LOG_HEXDUMP_DEBUG((uint8_t *)p_buff, length);
    NRF_LOG_WARNING("Function: %s, error code: %s.\r\n",
                    (uint32_t)__func__,
                    (uint32_t)ERR_TO_STR(err_code));

    return err_code;
}

void nrf_drv_rng_block_rand(uint8_t * p_buff, uint32_t length)
{
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

    while (length)
    {
        uint32_t    len = MIN(length, RNG_CONFIG_POOL_SIZE);
        ret_code_t  err_code;

        do {
            err_code = nrf_drv_rng_rand(p_buff, len);
        } while(err_code != NRF_SUCCESS);

        length -= len;
        p_buff += len;
    }

    NRF_LOG_DEBUG("Rand buffer data:\r\n");
    NRF_LOG_HEXDUMP_DEBUG((uint8_t *)p_buff, length);
}

#ifdef SOFTDEVICE_PRESENT
void nrf_drv_rng_on_sd_disable(void)
{
    NRF_DRV_RNG_LOCK();
    if (m_rng_cb.state == NRF_DRV_STATE_INITIALIZED)
    {
        nrf_drv_rng_setup();
        nrf_drv_rng_start();
    }
    NRF_DRV_RNG_RELEASE();
}
#endif // SOFTDEVICE_PRESENT

void RNG_IRQHandler(void)
{
    NRF_DRV_RNG_LOCK();
    if (
        !NRF_DRV_RNG_SD_IS_ENABLED()            &&
        nrf_rng_event_get(NRF_RNG_EVENT_VALRDY) &&
        nrf_rng_int_get(NRF_RNG_INT_VALRDY_MASK))
    {
        nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);

        uint8_t new_value = nrf_rng_random_value_get();
        UNUSED_RETURN_VALUE(nrf_queue_push(&m_rand_pool, &new_value));

        if (nrf_queue_is_full(&m_rand_pool))
        {
            nrf_drv_rng_stop();
        }

        NRF_LOG_DEBUG("Event: NRF_RNG_EVENT_VALRDY.\r\n");
    }
    NRF_DRV_RNG_RELEASE();
}

#endif // NRF_MODULE_ENABLED(RNG)
