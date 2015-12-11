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
 
#include "bsp_btn_ant.h"
#include <stdint.h>
#include "bsp.h"
#include "ant_stack_handler_types.h"
#include "ant_parameters.h"
#include "app_trace.h"

#define BTN_ID_WAKEUP             3  /**< ID of button used to wake up the application. */
#define BTN_ID_SLEEP              3  /**< ID of button used to put the application into sleep mode. */

#define BTN_ACTION_SLEEP          BSP_BUTTON_ACTION_RELEASE    /**< Button action used to put the application into sleep mode. */
 
 
 /**@brief This macro will return from the current function if err_code
 *        is not NRF_SUCCESS or NRF_ERROR_INVALID_PARAM.
 */
#define RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code)                             \
do                                                                              \
{                                                                               \
    if (((err_code) != NRF_SUCCESS) && ((err_code) != NRF_ERROR_INVALID_PARAM)) \
    {                                                                           \
        return err_code;                                                        \
    }                                                                           \
}                                                                               \
while(0)
 

/**@brief This macro will return from the current function if err_code
 *        is not NRF_SUCCESS or NRF_ERROR_NOT_SUPPORTED.
 */
#define RETURN_ON_ERROR_NOT_NOT_SUPPORTED(err_code)                             \
do                                                                              \
{                                                                               \
    if (((err_code) != NRF_SUCCESS) && ((err_code) != NRF_ERROR_NOT_SUPPORTED)) \
    {                                                                           \
        return err_code;                                                        \
    }                                                                           \
}                                                                               \
while(0)
    

static bool m_connected = false;  /**< Notify if channel is connected. */
 

 /**@brief Function for configuring the buttons for connection.
 *
 * @retval NRF_SUCCESS  Configured successfully.
 * @return A propagated error code.
 */
static uint32_t connection_buttons_configure(void)
{
    uint32_t err_code;

    err_code = bsp_event_to_button_action_assign(BTN_ID_SLEEP,
                                                 BTN_ACTION_SLEEP,
                                                 BSP_EVENT_DEFAULT);
    RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);

    return NRF_SUCCESS;
}


/**@brief Function for configuring the buttons for searching.
 *
 * @retval NRF_SUCCESS  Configured successfully.
 * @return A propagated error code.
 */
static uint32_t searching_buttons_configure(void)
{
    uint32_t err_code;
	
    err_code = bsp_event_to_button_action_assign(BTN_ID_SLEEP,
                                                 BTN_ACTION_SLEEP,
                                                 BSP_EVENT_SLEEP);
    RETURN_ON_ERROR_NOT_INVALID_PARAM(err_code);

    return NRF_SUCCESS;
}
 

uint32_t bsp_btn_ant_sleep_mode_prepare(void)
{
    uint32_t err_code = bsp_wakeup_buttons_set(1 << BTN_ID_WAKEUP);
    RETURN_ON_ERROR_NOT_NOT_SUPPORTED(err_code);

    return NRF_SUCCESS;
}


void bsp_btn_ant_on_ant_evt(ant_evt_t * p_ant_evt)
{
    uint32_t err_code;
    
    switch (p_ant_evt->event)
    {
        case EVENT_RX:
            if (!m_connected)
            {
                err_code = connection_buttons_configure();
                APP_ERROR_CHECK(err_code);
            }
						
            m_connected = true;
            break;

        case EVENT_RX_FAIL_GO_TO_SEARCH:
            m_connected = false;
				
            err_code = searching_buttons_configure();
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}


uint32_t bsp_btn_ant_init(void)
{
    uint32_t err_code = NRF_SUCCESS;

    if (!m_connected)
    {
        err_code = searching_buttons_configure();
    }
    else
    {
        err_code = connection_buttons_configure();
    }

    return err_code;
}
