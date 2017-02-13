/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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
 * @defgroup bsp Board Support Package
 * @{
 * @ingroup app_common
 *
 * @brief BSP module.
 * @details This module provides a layer of abstraction from the board.
 *          It allows the user to indicate certain states on LEDs in a simple way.
 *          Module functionality can be modified by additional defines:
 *          - BSP_SIMPLE - reduces functionality of this module to enable
 *            and read state of the buttons.
 *          - BSP_UART_SUPPORT - enables support for UART.
 */

#ifndef BSP_CONFIG_H__
#define BSP_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>
#include "boards.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(BSP_DEFINES_ONLY) && !defined(BSP_SIMPLE)
#include "app_button.h"

#define BSP_BUTTON_ACTION_PUSH      (APP_BUTTON_PUSH)    /**< Represents pushing a button. See @ref bsp_button_action_t. */
#define BSP_BUTTON_ACTION_RELEASE   (APP_BUTTON_RELEASE) /**< Represents releasing a button. See @ref bsp_button_action_t. */
#define BSP_BUTTON_ACTION_LONG_PUSH (2)                  /**< Represents pushing and holding a button for @ref BSP_LONG_PUSH_TIMEOUT_MS milliseconds. See also @ref bsp_button_action_t. */
#endif

#define BSP_MS_TO_TICK(MS) (m_app_ticks_per_100ms * (MS / 100))


#define BUTTON_ERASE_BONDING BSP_BUTTON_0_MASK
#define BUTTON_ERASE_ALL     BSP_BUTTON_1_MASK
#define BUTTON_ADVERTISE     BSP_BUTTON_0_MASK
#define BUTTON_CLEAR_EVT     BSP_BUTTON_1_MASK
#define BUTTON_CAPSLOCK      BSP_BUTTON_2_MASK
#define BSP_BUTTONS_ALL      0xFFFFFFFF
#define BSP_BUTTONS_NONE     0

#define BSP_LONG_PUSH_TIMEOUT_MS (1000) /**< The time to hold for a long push (in milliseconds). */
/**@brief Types of BSP initialization.
 */

#define ADVERTISING_LED_ON_INTERVAL            200
#define ADVERTISING_LED_OFF_INTERVAL           1800

#define ADVERTISING_DIRECTED_LED_ON_INTERVAL   200
#define ADVERTISING_DIRECTED_LED_OFF_INTERVAL  200

#define ADVERTISING_WHITELIST_LED_ON_INTERVAL  200
#define ADVERTISING_WHITELIST_LED_OFF_INTERVAL 800

#define ADVERTISING_SLOW_LED_ON_INTERVAL       400
#define ADVERTISING_SLOW_LED_OFF_INTERVAL      4000

#define BONDING_INTERVAL                       100

#define SENT_OK_INTERVAL                       100
#define SEND_ERROR_INTERVAL                    500

#define RCV_OK_INTERVAL                        100
#define RCV_ERROR_INTERVAL                     500

#define ALERT_INTERVAL                         200

#define BSP_LED_INDICATE_SENT_OK               BSP_BOARD_LED_1
#define BSP_LED_INDICATE_SEND_ERROR            BSP_BOARD_LED_1
#define BSP_LED_INDICATE_RCV_OK                BSP_BOARD_LED_1
#define BSP_LED_INDICATE_RCV_ERROR             BSP_BOARD_LED_1
#define BSP_LED_INDICATE_CONNECTED             BSP_BOARD_LED_0
#define BSP_LED_INDICATE_BONDING               BSP_BOARD_LED_0
#define BSP_LED_INDICATE_ADVERTISING_DIRECTED  BSP_BOARD_LED_0
#define BSP_LED_INDICATE_ADVERTISING_SLOW      BSP_BOARD_LED_0
#define BSP_LED_INDICATE_ADVERTISING_WHITELIST BSP_BOARD_LED_0
#define BSP_LED_INDICATE_INDICATE_ADVERTISING  BSP_BOARD_LED_0

#define BSP_LED_INDICATE_USER_LED1            BSP_BOARD_LED_0
#define BSP_LED_INDICATE_USER_LED2            BSP_BOARD_LED_1
#define BSP_LED_INDICATE_USER_LED3            BSP_BOARD_LED_2
#define BSP_LED_INDICATE_USER_LED4            BSP_BOARD_LED_3

#define BSP_LED_ALERT                         BSP_BOARD_LED_2

#ifdef __cplusplus
}
#endif

#endif // BSP_CONFIG_H__

/** @} */
