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

#ifndef APP_USBD_HID_GENERIC_H__
#define APP_USBD_HID_GENERIC_H__



#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "nrf_drv_usbd.h"
#include "app_usbd_class_base.h"
#include "app_usbd_hid_types.h"
#include "app_usbd_hid.h"
#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_descriptor.h"
#include "app_usbd_hid_generic_desc.h"
#include "app_usbd_hid_generic_internal.h"

/**
 * @defgroup app_usbd_hid_generic USB HID generic
 * @ingroup app_usbd_hid
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID generic class.
 * @{
 */


#ifdef DOXYGEN
/**
 * @brief HID generic class instance type
 *
 * @ref APP_USBD_CLASS_TYPEDEF
 */
typedef struct { } app_usbd_hid_generic_t;
#else
/*lint -save -e10 -e26 -e123 -e505 */
APP_USBD_CLASS_TYPEDEF(app_usbd_hid_generic,                                           \
            APP_USBD_HID_GENERIC_CONFIG(0, (NRF_DRV_USBD_EPIN1, NRF_DRV_USBD_EPOUT1)), \
            APP_USBD_HID_GENERIC_INSTANCE_SPECIFIC_DEC,                                \
            APP_USBD_HID_GENERIC_DATA_SPECIFIC_DEC                                     \
);
/*lint -restore*/
#endif

/**
 * @brief Global definition of app_usbd_hid_generic_t class
 *
 * @param instance_name             Name of global instance
 * @param interface_number          Unique interface index
 * @param user_ev_handler           User event handler (optional)
 * @param endpoint_list             Input endpoint list (@ref nrf_drv_usbd_ep_t)
 * @param hid_dsc_list              HID descriptor item list @ref APP_USBD_HID_REPORT_ITEM, @ref APP_USBD_HID_PHYSICAL_ITEM
 * @param report_cnt                Report count
 * @param report_out_maxsize        Maximum output report size
 *
 * @note This macro is just simplified version of @ref APP_USBD_HID_GENERIC_GLOBAL_DEF_INTERNAL
 *
 * Example class definition:
 * @code

       static uint8_t m_generic_rep_dsc[] = APP_USBD_HID_MOUSE_REPORT_DSC_BUTTON(2);

       #define HID_DESCRIPTOR_ITEM_LIST                             \
       (                                                            \
               m_generic_rep_dsc,                                   \
       )

       #define ENDPOINT_LIST                                        \
       (                                                            \
               NRF_DRV_USBD_EPIN1                                   \
       )

       #define REPORT_COUNT        1
       #define REPORT_OUT_MAXSIZE  0

       APP_USBD_HID_GENERIC_GLOBAL_DEF(m_app_hid_generic,
                                       0,
                                       hid_user_ev_handler,
                                       ENDPOINT_LIST,
                                       HID_DESCRIPTOR_ITEM_LIST,
                                       REPORT_COUNT,
                                       REPORT_OUT_MAXSIZE);
  @endcode
 *
 */
#define APP_USBD_HID_GENERIC_GLOBAL_DEF(instance_name,              \
                                        interface_number,           \
                                        user_ev_handler,            \
                                        endpoint_list,              \
                                        hid_dsc_list,               \
                                        report_cnt,                 \
                                        report_out_maxsize)         \
    APP_USBD_HID_GENERIC_GLOBAL_DEF_INTERNAL(instance_name,         \
                                             interface_number,      \
                                             user_ev_handler,       \
                                             endpoint_list,         \
                                             hid_dsc_list,          \
                                             report_cnt,            \
                                             report_out_maxsize)


/**
 * @brief Helper function to get class instance from HID generic class
 *
 * @param[in] p_generic HID generic class instance (defined by @ref APP_USBD_HID_GENERIC_GLOBAL_DEF)
 *
 * @return Base class instance
 */
static inline app_usbd_class_inst_t const *
app_usbd_hid_generic_class_inst_get(app_usbd_hid_generic_t const * p_generic)
{
    return &p_generic->base;
}

/**
 * @brief Helper function to get HID generic from base class instance
 *
 * @param[in] p_inst Base class instance
 *
 * @return HID generic class handle
 */
static inline app_usbd_hid_generic_t const *
app_usbd_hid_generic_class_get(app_usbd_class_inst_t const * p_inst)
{
    return (app_usbd_hid_generic_t const *)p_inst;
}

/**
 * @brief New IN report trigger
 *
 *
 * @param[in] p_generic HID generic class instance (defined by @ref APP_USBD_HID_GENERIC_GLOBAL_DEF)
 * @param[in] rep_id    Input report ID
 * @param[in] p_buff    Report buffer
 * @param[in] size      Report size
 *
 * @return Standard error code
 */
ret_code_t app_usbd_hid_generic_report_in_set(app_usbd_hid_generic_t const * p_generic,
                                              uint8_t rep_id,
                                              const void * p_buff,
                                              size_t size);


/**
 * @brief Checks whether last IN report transfer has been done
 *
 * Helper function to check, whether that last IN report transfer is done.
 *
 * @param[in] p_generic HID generic class instance (defined by @ref APP_USBD_HID_GENERIC_GLOBAL_DEF)
 * @param rep_id input  Report ID
 *
 * @return true if transfer done, false otherwise
 */
bool app_usbd_hid_generic_report_in_done(app_usbd_hid_generic_t const * p_generic, uint8_t rep_id);


/**
 * @brief Returns last successful transfered IN report.
 *
 * @note Use this call only on @ref APP_USBD_HID_USER_EVT_IN_REPORT_DONE event
 *
 * @param[in] p_generic HID generic class instance (defined by @ref APP_USBD_HID_GENERIC_GLOBAL_DEF)
 * @return Last transfered report ID.
 */
uint8_t app_usbd_hid_generic_in_report_last_id(app_usbd_hid_generic_t const * p_generic);

/**
 * @brief Returns last successful transfered OUT report.
 *
 * @warning Use this call only on @ref APP_USBD_HID_USER_EVT_OUT_REPORT_READY event.
 *
 * @param[in]  p_generic HID generic class instance (defined by @ref APP_USBD_HID_GENERIC_GLOBAL_DEF)
 * @param[out] p_size    Last transfered OUT report size.
 * @return Last transfered OUT report.
 */
const void * app_usbd_hid_generic_out_report_get(app_usbd_hid_generic_t const * p_generic,
                                                 size_t * p_size);


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_HID_GENERIC_H__ */
