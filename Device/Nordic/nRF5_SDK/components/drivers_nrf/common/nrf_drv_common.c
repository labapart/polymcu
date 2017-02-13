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

#include <stddef.h>
#include "nrf_drv_common.h"
#include "nrf_assert.h"
#include "app_util_platform.h"
#include "nrf_peripherals.h"

#if NRF_DRV_COMMON_POWER_CLOCK_ISR
#include "nrf_drv_power.h"
#include "nrf_drv_clock.h"
#endif
#ifdef SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#endif

#if NRF_MODULE_ENABLED(PERIPHERAL_RESOURCE_SHARING)

#define NRF_LOG_MODULE_NAME "COMMON"

#if COMMON_CONFIG_LOG_ENABLED
#define NRF_LOG_LEVEL       COMMON_CONFIG_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  COMMON_CONFIG_INFO_COLOR
#define NRF_LOG_DEBUG_COLOR COMMON_CONFIG_DEBUG_COLOR
#else //COMMON_CONFIG_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif //COMMON_CONFIG_LOG_ENABLED
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

typedef struct {
    nrf_drv_irq_handler_t handler;
    bool                  acquired;
} shared_resource_t;

// SPIM0, SPIS0, SPI0, TWIM0, TWIS0, TWI0
#if (NRF_MODULE_ENABLED(SPI0) || NRF_MODULE_ENABLED(SPIS0) || NRF_MODULE_ENABLED(TWI0) || NRF_MODULE_ENABLED(TWIS0))
    #define SERIAL_BOX_0_IN_USE
    // [this checking may need a different form in unit tests, hence macro]
    #ifndef IS_SERIAL_BOX_0
        #define IS_SERIAL_BOX_0(p_per_base)  (p_per_base == NRF_SPI0)
    #endif

    static shared_resource_t m_serial_box_0 = { .acquired = false };
    void SPI0_TWI0_IRQHandler(void)
    {
        ASSERT(m_serial_box_0.handler);
        m_serial_box_0.handler();
    }
#endif // (NRF_MODULE_ENABLED(SPI0) || NRF_MODULE_ENABLED(SPIS0) || NRF_MODULE_ENABLED(TWI0) || NRF_MODULE_ENABLED(TWIS0))

// SPIM1, SPIS1, SPI1, TWIM1, TWIS1, TWI1
#if (NRF_MODULE_ENABLED(SPI1) || NRF_MODULE_ENABLED(SPIS1) || NRF_MODULE_ENABLED(TWI1) || NRF_MODULE_ENABLED(TWIS1))
    #define SERIAL_BOX_1_IN_USE
    // [this checking may need a different form in unit tests, hence macro]
    #ifndef IS_SERIAL_BOX_1
        #define IS_SERIAL_BOX_1(p_per_base)  (p_per_base == NRF_SPI1)
    #endif

    static shared_resource_t m_serial_box_1 = { .acquired = false };
#ifdef TWIM_PRESENT
    void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler(void)
#else
    void SPI1_TWI1_IRQHandler(void)
#endif
    {
        ASSERT(m_serial_box_1.handler);
        m_serial_box_1.handler();
    }
#endif // (NRF_MODULE_ENABLED(SPI1) || NRF_MODULE_ENABLED(SPIS1) || NRF_MODULE_ENABLED(TWI1) || NRF_MODULE_ENABLED(TWIS1))

// SPIM2, SPIS2, SPI2
#if (NRF_MODULE_ENABLED(SPI2) || NRF_MODULE_ENABLED(SPIS2))
    #define SERIAL_BOX_2_IN_USE
    // [this checking may need a different form in unit tests, hence macro]
    #ifndef IS_SERIAL_BOX_2
        #define IS_SERIAL_BOX_2(p_per_base)  (p_per_base == NRF_SPI2)
    #endif

    static shared_resource_t m_serial_box_2 = { .acquired = false };
    void SPIM2_SPIS2_SPI2_IRQHandler(void)
    {
        ASSERT(m_serial_box_2.handler);
        m_serial_box_2.handler();
    }
#endif // (NRF_MODULE_ENABLED(SPI2) || NRF_MODULE_ENABLED(SPIS2))

// COMP, LPCOMP
#if (NRF_MODULE_ENABLED(COMP) || NRF_MODULE_ENABLED(LPCOMP))
    #define COMP_LPCOMP_IN_USE

    #ifndef IS_COMP_LPCOMP
        #define IS_COMP_LPCOMP(p_per_base)  ((p_per_base) == NRF_LPCOMP)
    #endif

    static shared_resource_t m_comp_lpcomp = { .acquired = false };
    void LPCOMP_IRQHandler(void)
    {
        ASSERT(m_comp_lpcomp.handler);
        m_comp_lpcomp.handler();
    }
#endif    // (NRF_MODULE_ENABLED(COMP) || NRF_MODULE_ENABLED(LPCOMP))

#if defined(SERIAL_BOX_0_IN_USE) || \
    defined(SERIAL_BOX_1_IN_USE) || \
    defined(SERIAL_BOX_2_IN_USE) || \
    defined(COMP_LPCOMP_IN_USE)
static ret_code_t acquire_shared_resource(shared_resource_t * p_resource,
                                          nrf_drv_irq_handler_t handler)
{
    ret_code_t err_code;
    
    bool busy = false;

    CRITICAL_REGION_ENTER();
    if (p_resource->acquired)
    {
        busy = true;
    }
    else
    {
        p_resource->acquired = true;
    }
    CRITICAL_REGION_EXIT();

    if (busy)
    {
        err_code = NRF_ERROR_BUSY;
        NRF_LOG_WARNING("Function: %s, error code: %s.\r\n", (uint32_t)__func__, (uint32_t)ERR_TO_STR(err_code));
        return err_code;
    }

    p_resource->handler = handler;
    err_code = NRF_SUCCESS;
    NRF_LOG_INFO("Function: %s, error code: %s.\r\n", (uint32_t)__func__, (uint32_t)ERR_TO_STR(err_code));
    return err_code;
}
#endif

ret_code_t nrf_drv_common_per_res_acquire(void const * p_per_base,
                                          nrf_drv_irq_handler_t handler)
{
#ifdef SERIAL_BOX_0_IN_USE
    if (IS_SERIAL_BOX_0(p_per_base))
    {
        return acquire_shared_resource(&m_serial_box_0, handler);
    }
#endif

#ifdef SERIAL_BOX_1_IN_USE
    if (IS_SERIAL_BOX_1(p_per_base))
    {
        return acquire_shared_resource(&m_serial_box_1, handler);
    }
#endif

#ifdef SERIAL_BOX_2_IN_USE
    if (IS_SERIAL_BOX_2(p_per_base))
    {
        return acquire_shared_resource(&m_serial_box_2, handler);
    }
#endif

#ifdef COMP_LPCOMP_IN_USE
    if (IS_COMP_LPCOMP(p_per_base))
    {
        return acquire_shared_resource(&m_comp_lpcomp, handler);
    }
#endif
    ret_code_t err_code;
    
    err_code = NRF_ERROR_INVALID_PARAM;
    NRF_LOG_WARNING("Function: %s, error code: %s.\r\n", (uint32_t)__func__, (uint32_t)ERR_TO_STR(err_code));
    return err_code;
}

void nrf_drv_common_per_res_release(void const * p_per_base)
{
#ifdef SERIAL_BOX_0_IN_USE
    if (IS_SERIAL_BOX_0(p_per_base))
    {
        m_serial_box_0.acquired = false;
    }
    else
#endif

#ifdef SERIAL_BOX_1_IN_USE
    if (IS_SERIAL_BOX_1(p_per_base))
    {
        m_serial_box_1.acquired = false;
    }
    else
#endif

#ifdef SERIAL_BOX_2_IN_USE
    if (IS_SERIAL_BOX_2(p_per_base))
    {
        m_serial_box_2.acquired = false;
    }
    else
#endif

#ifdef COMP_LPCOMP_IN_USE
    if (IS_COMP_LPCOMP(p_per_base))
    {
        m_comp_lpcomp.acquired = false;
    }
    else
#endif

    {}
}

#endif // NRF_MODULE_ENABLED(PERIPHERAL_RESOURCE_SHARING)

#if NRF_MODULE_ENABLED(POWER)
void nrf_drv_common_power_irq_disable(void)
{
#if NRF_DRV_COMMON_POWER_CLOCK_ISR
    if(!nrf_drv_clock_init_check())
#endif
    {
        nrf_drv_common_irq_disable(POWER_CLOCK_IRQn);
    }
}
#endif

#if NRF_MODULE_ENABLED(CLOCK)
void nrf_drv_common_clock_irq_disable(void)
{
#if NRF_DRV_COMMON_POWER_CLOCK_ISR
    if(!nrf_drv_power_init_check())
#endif
    {
        nrf_drv_common_irq_disable(POWER_CLOCK_IRQn);
    }
}
#endif

#if NRF_DRV_COMMON_POWER_CLOCK_ISR
void POWER_CLOCK_IRQHandler(void)
{
    extern void nrf_drv_clock_onIRQ(void);
    extern void nrf_drv_power_onIRQ(void);

    nrf_drv_clock_onIRQ();
    nrf_drv_power_onIRQ();
}
#endif // NRF_DRV_COMMON_POWER_CLOCK_ISR


void nrf_drv_common_irq_enable(IRQn_Type IRQn, uint8_t priority)
{
    INTERRUPT_PRIORITY_ASSERT(priority);

    NVIC_SetPriority(IRQn, priority);
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_EnableIRQ(IRQn);
}
