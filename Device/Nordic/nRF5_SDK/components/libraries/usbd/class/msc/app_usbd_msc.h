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

#ifndef APP_USBD_MSC_H__
#define APP_USBD_MSC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "nrf_drv_usbd.h"
#include "nrf_block_dev.h"
#include "app_usbd_class_base.h"
#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_descriptor.h"

#include "app_usbd_msc_types.h"
#include "app_usbd_msc_desc.h"
#include "app_usbd_msc_scsi.h"
#include "app_usbd_msc_internal.h"


/**
 * @defgroup app_usbd_msc USB MSC class
 * @ingroup app_usbd
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the USB MSC class.
 *   
 * @details References:
 * - "Universal Serial Bus Mass Storage Class, Specification Overview,"
 *   Revision 1.2,  USB Implementer's Forum, June 23, 2003.
 * - "Universal Serial Bus Mass Storage Class, Bulk-Only Transport,"
 *   Revision 1.0, USB Implementer's Forum, September 31, 1999.
 *
 * @{
 */

#ifdef DOXYGEN
/**
 * @brief Mass storage class instance type
 *
 * @ref APP_USBD_CLASS_TYPEDEF
 */
typedef struct { } app_usbd_msc_t;
#else
/*lint -save -e10 -e26 -e123 -e505 */
APP_USBD_CLASS_TYPEDEF(app_usbd_msc,                                                   \
            APP_USBD_MSC_CONFIG(0, (NRF_DRV_USBD_EPIN1, NRF_DRV_USBD_EPOUT1)),         \
            APP_USBD_MSC_INSTANCE_SPECIFIC_DEC,                                        \
            APP_USBD_MSC_DATA_SPECIFIC_DEC                                             \
);
#endif

/*lint -restore*/


/*lint -save -e407 */

/**
 * @brief Events passed to user event handler
 *
 * @note Example prototype of user event handler:
 *
 * void msc_user_ev_handler(app_usbd_class_inst_t const * p_inst,
 *                          app_usbd_msc_user_event_t     event);
 */
typedef enum app_usbd_msc_user_event_e {
    APP_USBD_MSC_USER_EVT_SUSPEND = 0, /**< User event SUSPEND (bus state notification) */
    APP_USBD_MSC_USER_EVT_RESUME,      /**< User event RESUME (bus state notification)  */
    APP_USBD_MSC_USER_EVT_START,       /**< User event START (cable connect)            */
    APP_USBD_MSC_USER_EVT_STOP,        /**< User event STOP (cable disconnect)          */
} app_usbd_msc_user_event_t;

/*lint -restore*/

/**
 * @brief Helper macro for defining MSC endpoints
 *
 * @param in_number     Input endpoint number
 * @param out_number    Output endpoint number
 * */
#define APP_USBD_MSC_ENDPOINT_LIST(in_number, out_number) (    \
        CONCAT_2(NRF_DRV_USBD_EPIN, in_number),                \
        CONCAT_2(NRF_DRV_USBD_EPOUT, out_number)               \
)

/**
 * @brief Global definition of app_usbd_msc_t class
 *
 * @param instance_name             Name of global instance
 * @param interface_number          Unique interface number
 * @param user_ev_handler           User event handler (optional)
 * @param endpoint_list             Input endpoint list (@ref nrf_drv_usbd_ep_t)
 * @param blockdev_list             Block device list
 * @param workbuffer_size           Work buffer size (need to fit into all block devices from
 *                                  block device list)
 *
 * @note This macro is just simplified version of @ref APP_USBD_MSC_GLOBAL_DEF_INTERNAL
 *
 */
#define APP_USBD_MSC_GLOBAL_DEF(instance_name,              \
                                interface_number,           \
                                user_ev_handler,            \
                                endpoint_list,              \
                                blockdev_list,              \
                                workbuffer_size)            \
        APP_USBD_MSC_GLOBAL_DEF_INTERNAL(instance_name,     \
                                         interface_number,  \
                                         user_ev_handler,   \
                                         endpoint_list,     \
                                         blockdev_list,     \
                                         workbuffer_size)


/**
 * @@brief Helper function to get class instance from MSC
 *
 * @param[in] p_msc     MSC instance (declared by @ref APP_USBD_MSC_GLOBAL_DEF)
 *
 * @return Base class instance
 */
static inline app_usbd_class_inst_t const *
app_usbd_msc_class_inst_get(app_usbd_msc_t const * p_msc)
{
    return &p_msc->base;
}

/**
 * @brief Helper function to get MSC from base class instance
 *
 * @param[in] p_inst Base class instance
 *
 * @return MSC class handle
 */
static inline app_usbd_msc_t const * app_usbd_msc_class_get(app_usbd_class_inst_t const * p_inst)
{
    return (app_usbd_msc_t const *)p_inst;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_MSC_H__ */
