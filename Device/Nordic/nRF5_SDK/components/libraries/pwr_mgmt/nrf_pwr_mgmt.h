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

/**
 * @defgroup nrf_pwr_mgmt Power management
 * @ingroup app_common
 * @{
 * @brief This module handles power management features.
 *
 */
#ifndef NRF_PWR_MGMT_H__
#define NRF_PWR_MGMT_H__

#include <stdbool.h>
#include <stdint.h>
#include <sdk_errors.h>
#include "section_vars.h"

/**@brief Power management shutdown types. */
typedef enum
{
    NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF,
    //!< Go to System OFF.

    NRF_PWR_MGMT_SHUTDOWN_STAY_IN_SYSOFF,
    //!< Go to System OFF and stay there.
    /**<
     * Useful when battery level is dangerously low, for example.
     */

    NRF_PWR_MGMT_SHUTDOWN_GOTO_DFU,
    //!< Go to DFU mode.

    NRF_PWR_MGMT_SHUTDOWN_CONTINUE
    //!< Continue shutdown.
    /**<
     * This should be used by modules that block the shutdown process, when they become ready for
     * shutdown.
     */
} nrf_pwr_mgmt_shutdown_t;

/**@brief Shutdown event types. */
typedef enum
{
    NRF_PWR_MGMT_EVT_PREPARE_WAKEUP,
    //!< Application will prepare the wakeup mechanism.

    NRF_PWR_MGMT_EVT_PREPARE_SYSOFF,
    //!< Application will prepare to stay in System OFF state.

    NRF_PWR_MGMT_EVT_PREPARE_DFU
    //!< Application will prepare to enter DFU mode.
} nrf_pwr_mgmt_evt_t;


/**@brief Shutdown callback.
 * @param[in] event   Type of shutdown process.
 *
 * @retval    true    System OFF / Enter DFU preparation successful. Process will be continued.
 * @retval    false   System OFF / Enter DFU preparation failed. @ref NRF_PWR_MGMT_SHUTDOWN_CONTINUE
 *                    should be used to continue the shutdown process.
 */
typedef bool (*nrf_pwr_mgmt_shutdown_handler_t)(nrf_pwr_mgmt_evt_t event);


/**@brief   Macro for registering a shutdown handler. Modules that want to get events
 *          from this module must register the handler using this macro.
 *
 * @details This macro places the handler in a section named "pwr_mgmt_data".
 *
 * @param[in]   handler     Event handler (@ref nrf_pwr_mgmt_shutdown_handler_t).
 */
#define NRF_PWR_MGMT_REGISTER_HANDLER(handler)                                      \
            NRF_SECTION_VARS_REGISTER_VAR(pwr_mgmt_data,                            \
                                          nrf_pwr_mgmt_shutdown_handler_t const handler)

/**@brief   Function for initializing power management.
 *
 * @warning Depending on configuration, this function sets SEVONPEND in System Control Block (SCB).
 *          This operation is unsafe with the SoftDevice from interrupt priority higher than SVC.
 *
 * @param[in] ticks_per_1s  Number of RTC ticks for 1 s.
 *
 * @retval NRF_SUCCESS
 */
ret_code_t nrf_pwr_mgmt_init(uint32_t ticks_per_1s);

/**@brief Function for running power management. Should run in the main loop.
 */
void nrf_pwr_mgmt_run(void);

/**@brief Function for indicating activity.
 *
 * @details Call this function whenever doing something that constitutes "activity".
 *          For example, whenever sending data, call this function to indicate that the application
 *          is active and should not disconnect any ongoing communication links.
 */
void nrf_pwr_mgmt_feed(void);

/**@brief Function for shutting down the system.	
 *
 * @param[in] shutdown_type     Type of operation.
 *
 * @details All callbacks will be executed prior to shutdown.
 */
void nrf_pwr_mgmt_shutdown(nrf_pwr_mgmt_shutdown_t shutdown_type);

#endif // NRF_PWR_MGMT_H__
/** @} */
