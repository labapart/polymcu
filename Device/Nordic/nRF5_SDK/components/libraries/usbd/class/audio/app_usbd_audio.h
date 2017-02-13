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

#ifndef APP_USBD_AUDIO_H__
#define APP_USBD_AUDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "nrf_drv_usbd.h"
#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_class_base.h"
#include "app_usbd_descriptor.h"

#include "app_usbd_audio_types.h"
#include "app_usbd_audio_desc.h"
#include "app_usbd_audio_internal.h"


/**
 * @defgroup app_usbd_audio USB AUDIO class
 * @ingroup app_usbd
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by USB Audio class.
 * 
 * @details Reference specifications:
 * - "Universal Serial Bus Device Class Definition for Audio Devices"
 *   	Release 1.0,  March 18, 1998.
 * - "Universal Serial Bus Device Class Definition for Audio Data Formats"
 *   	Release 1.0, March 18, 1998.
 * - "Universal Serial Bus Device Class Definition for Terminal Types"
 *   	Release 1.0, March 18, 1998.
 *
 * @{
 */


#ifdef DOXYGEN
/**
 * @brief Audio class instance type
 *
 * @ref APP_USBD_CLASS_TYPEDEF
 */
typedef struct { } app_usbd_audio_t;
#else
/*lint -save -e10 -e26 -e123 -e505 */
APP_USBD_CLASS_TYPEDEF(app_usbd_audio,            \
            APP_USBD_AUDIO_CONFIG(0, 1),          \
            APP_USBD_AUDIO_INSTANCE_SPECIFIC_DEC, \
            APP_USBD_AUDIO_DATA_SPECIFIC_DEC      \
);
/*lint -restore*/
#endif


/*lint -save -e407 */

/**
 * @brief Events passed to user event handler
 *
 * @note Example prototype of user event handler:
 *
 * @code
   void audio_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                              app_usbd_audio_user_event_t   event);
 * @endcode
 */
typedef enum app_usbd_audio_user_event_e {
    APP_USBD_AUDIO_USER_EVT_SUSPEND = 0, /**< User event SUSPEND */
    APP_USBD_AUDIO_USER_EVT_RESUME,      /**< User event RESUME  */
    APP_USBD_AUDIO_USER_EVT_START,       /**< User event START   */
    APP_USBD_AUDIO_USER_EVT_STOP,        /**< User event STOP    */

    APP_USBD_AUDIO_USER_EVT_CLASS_REQ,   /**< User event CLASS_REQ */
    APP_USBD_AUDIO_USER_EVT_RX_DONE,     /**< User event RX_DONE   */
    APP_USBD_AUDIO_USER_EVT_TX_DONE,     /**< User event TX_DONE   */
} app_usbd_audio_user_event_t;

/*lint -restore*/


/**
 * @brief Global definition of app_usbd_audio_t class instance
 *
 * @param instance_name             Name of global instance
 * @param interfaces_configs        Interfaces configurations
 * @param user_ev_handler           User event handler (optional)
 * @param raw_descriptors           Raw descriptor table
 *
 * @note This macro is just simplified version of @ref APP_USBD_AUDIO_GLOBAL_DEF_INTERNAL
 *
 */
#define APP_USBD_AUDIO_GLOBAL_DEF(instance_name,            \
                                  interfaces_configs,       \
                                  user_ev_handler,          \
                                  raw_descriptors)          \
    APP_USBD_AUDIO_GLOBAL_DEF_INTERNAL(instance_name,       \
                                       interfaces_configs,  \
                                       user_ev_handler,     \
                                       raw_descriptors)

/**
 * @@brief Helper function to get class instance from Audio class
 *
 * @param[in] p_audio Audio class instance (declared by @ref APP_USBD_AUDIO_GLOBAL_DEF)
 * @return Base class instance
 */
static inline app_usbd_class_inst_t const *
app_usbd_audio_class_inst_get(app_usbd_audio_t const * p_audio)
{
    return &p_audio->base;
}

/**
 * @brief Helper function to get audio specific request from audio class
 *
 * @param[in] p_audio Audio class instance (declared by @ref APP_USBD_AUDIO_GLOBAL_DEF)
 * @return Audio class specific request
 */
static inline app_usbd_audio_req_t *
app_usbd_audio_class_request_get(app_usbd_audio_t const * p_audio)
{
    return &p_audio->specific.p_data->ctx.request;
}

/**
 * @brief Helper function to get audio from base class instance
 *
 * @param[in] p_inst Base class instance
 * @return Audio class handle
 */
static inline app_usbd_audio_t const *
app_usbd_audio_class_get(app_usbd_class_inst_t const * p_inst)
{
    return (app_usbd_audio_t const *)p_inst;
}

/**
 * @brief Set transmit buffer
 *
 * @param[in] p_inst    Base class instance
 * @param[in] p_buff    Transmit buffer
 * @param[in] size      Transmit buffer size
 * */
void app_usbd_audio_class_tx_buf_set(app_usbd_class_inst_t const * p_inst,
                                     const void * p_buff,
                                     size_t size);

/**
 * @brief Set receive buffer
 *
 * @param[in] p_inst    Base class instance
 * @param[in] p_buff    Receive buffer
 * @param[in] size      Receive buffer size
 * */
void app_usbd_audio_class_rx_buf_set(app_usbd_class_inst_t const * p_inst,
                                     void * p_buff,
                                     size_t size);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_AUDIO_H__ */
