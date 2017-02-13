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

#ifndef APP_USBD_HID_MOUSE_H__
#define APP_USBD_HID_MOUSE_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_drv_usbd.h"
#include "app_usbd_class_base.h"
#include "app_usbd_hid_types.h"
#include "app_usbd_hid.h"
#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_descriptor.h"
#include "app_usbd_hid_mouse_desc.h"
#include "app_usbd_hid_mouse_internal.h"

/**
 * @defgroup app_usbd_hid_mouse USB HID mouse
 * @ingroup app_usbd_hid
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID mouse class.
 * @{
 */

#ifdef DOXYGEN
/**
 * @brief HID mouse class instance type
 *
 * @ref APP_USBD_CLASS_TYPEDEF
 */
typedef struct { } app_usbd_hid_mouse_t;
#else
/*lint -save -e10 -e26 -e123 -e505 */
APP_USBD_CLASS_TYPEDEF(app_usbd_hid_mouse,                      \
            APP_USBD_HID_MOUSE_CONFIG(0, NRF_DRV_USBD_EPIN1),   \
            APP_USBD_HID_MOUSE_INSTANCE_SPECIFIC_DEC,           \
            APP_USBD_HID_MOUSE_DATA_SPECIFIC_DEC                \
);
/*lint -restore*/
#endif
/**
 * @brief Global definition of app_usbd_hid_mouse_t class
 *
 * @param instance_name     Name of global instance
 * @param interface_number  Unique interface number
 * @param endpoint          Input endpoint (@ref nrf_drv_usbd_ep_t)
 * @param bcnt              Mouse button count (from 1 to 8)
 * @param user_ev_handler   User event handler (optional)
 *
 * @note This macro is just simplified version of @ref APP_USBD_HID_MOUSE_GLOBAL_DEF_INTERNAL.
 *
 * @code
   APP_USBD_HID_MOUSE_GLOBAL_DEF(my_awesome_mouse, 0, NRF_DRV_USBD_EPIN1, 3, NULL)
 * @endcode
 */
#define APP_USBD_HID_MOUSE_GLOBAL_DEF(instance_name,         \
                                      interface_number,      \
                                      endpoint,              \
                                      bcnt,                  \
                                      user_ev_handler)       \
    APP_USBD_HID_MOUSE_GLOBAL_DEF_INTERNAL(instance_name,    \
                                           interface_number, \
                                           endpoint,         \
                                           bcnt,             \
                                           user_ev_handler)


/**
 * @brief Helper function to get class instance from HID mouse internals
 *
 * @param[in] p_mouse Mouse instance (declared by @ref APP_USBD_HID_MOUSE_GLOBAL_DEF)
 *
 * @return Base class instance
 * */
static inline app_usbd_class_inst_t const *
app_usbd_hid_mouse_class_inst_get(app_usbd_hid_mouse_t const * p_mouse)
{
    return &p_mouse->base;
}

/**
 * @brief Helper function to get HID mouse from base class instance
 *
 * @param[in] p_inst Base class instance
 *
 * @return HID mouse class handle
 */
static inline app_usbd_hid_mouse_t const *
app_usbd_hid_mouse_class_get(app_usbd_class_inst_t const * p_inst)
{
    return (app_usbd_hid_mouse_t const *)p_inst;
}

/**
 * @brief Move mouse X axis
 *
 * @param[in] p_mouse Mouse instance (declared by @ref APP_USBD_HID_MOUSE_GLOBAL_DEF)
 * @param[in] offset  Relative mouse position: allowed full int8_t range
 *
 * @return Standard error code
 * */
ret_code_t app_usbd_hid_mouse_x_move(app_usbd_hid_mouse_t const * p_mouse, int8_t offset);

/**
 * @brief Move mouse Y axis
 *
 * @param[in] p_mouse Mouse instance (declared by @ref APP_USBD_HID_MOUSE_GLOBAL_DEF)
 * @param[in] offset  Relative mouse position: allowed full int8_t range
 *
 * @return Standard error code
 * */
ret_code_t app_usbd_hid_mouse_y_move(app_usbd_hid_mouse_t const * p_mouse, int8_t offset);

/**
 * @brief Move mouse scroll
 *
 * @param[in] p_mouse Mouse instance (declared by @ref APP_USBD_HID_MOUSE_GLOBAL_DEF)
 * @param[in] offset  Relative mouse position: allowed full int8_t range
 *
 * @return Standard error code
 * */
ret_code_t app_usbd_hid_mouse_scroll_move(app_usbd_hid_mouse_t const * p_mouse, int8_t offset);

/**
* @brief Set mouse button state
*
* @param[in] p_mouse    Mouse instance (declared by @ref APP_USBD_HID_MOUSE_GLOBAL_DEF)
* @param[in] button_id  Button number (0...7)
* @param[in] state      Button state: true -> PRESSED, false -> RELEASED
*
* @return Standard error code
* */
ret_code_t app_usbd_hid_mouse_button_state(app_usbd_hid_mouse_t const * p_mouse,
                                           uint8_t button_id,
                                           bool state);

/** @} */

#endif /* APP_USBD_HID_MOUSE_H__ */
