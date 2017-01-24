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
 
 /**@file
 *
 * @defgroup bsp_btn_ant BSP: ANT Button Module
 * @{
 * @ingroup bsp
 *
 * @brief Module for controlling ANT behavior through button actions.
 *
 * @details The application must propagate ANT events to the ANT Button Module.
 * Based on these events, the ANT Button Module configures the Board Support Package
 * to generate BSP events for certain button actions. These BSP events should then be
 * handled by the application's BSP event handler.
 *
 */
 
#ifndef BSP_BTN_ANT_H__
#define BSP_BTN_ANT_H__

#include <stdint.h>
#include "ant_stack_handler_types.h"

 /**@brief Function for initializing the ANT Button Module.
 *
 * Before calling this function, the BSP module must be initialized with buttons.
 *
 * @retval NRF_SUCCESS  If initialization was successful. Otherwise, a propagated error code is
 *                      returned.
 */
uint32_t bsp_btn_ant_init(void);

/**@brief Function for setting up wakeup buttons before going into sleep mode.
 *
 * @retval NRF_SUCCESS  If the buttons were prepared successfully. Otherwise, a propagated error
 *                      code is returned.
 */
uint32_t bsp_btn_ant_sleep_mode_prepare(void);

/**@brief Function for handling the application's ANT stack events.
 *
 * @details This function handles all events from the ANT stack that are of interest to this module.
 *
 * @param[in] p_ant_evt ANT stack event.
 */
void bsp_btn_ant_on_ant_evt(ant_evt_t * p_ant_evt);

#endif /* BSP_BTN_ANT_H__ */

/** @} */
