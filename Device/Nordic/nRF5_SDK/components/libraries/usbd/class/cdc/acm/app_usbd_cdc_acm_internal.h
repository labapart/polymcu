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

#ifndef APP_USBD_CDC_ACM_INTERNAL_H__
#define APP_USBD_CDC_ACM_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "app_util.h"

/**
 * @defgroup app_usbd_cdc_acm_internal USB CDC ACM internals
 * @ingroup app_usbd_cdc_acm
 * @brief @tagAPI52840 Internals of the USB ACM class implementation.
 * @{
 */

/**
 * @brief Forward declaration of type defined by @ref APP_USBD_CLASS_TYPEDEF in cdc_acm class
 *
 */
APP_USBD_CLASS_FORWARD(app_usbd_cdc_acm);

/*lint -save -e165*/
/**
 * @brief Forward declaration of @ref app_usbd_cdc_acm_user_event_e
 *
 */
enum app_usbd_cdc_acm_user_event_e;

/*lint -restore*/

/**
 * @brief User event handler
 *
 * @param[in] p_inst    Class instance
 * @param[in] event     User event
 *
 * */
typedef void (*app_usbd_cdc_acm_user_ev_handler_t)(app_usbd_class_inst_t const *    p_inst,
                                                 enum app_usbd_cdc_acm_user_event_e event);

/**
 * @brief CDC ACM class part of class instance data
 */
typedef struct {
    uint8_t const * p_raw_desc;         //!< CDC ACM class descriptors
    size_t          raw_desc_size;      //!< CDC ACM class descriptors size

    app_usbd_cdc_acm_user_ev_handler_t user_ev_handler; //!< User event handler
} app_usbd_cdc_acm_inst_t;


/**
 * @brief CDC ACM serial state class notify
 * */
typedef struct {
    app_usbd_cdc_notify_t cdc_notify;
    uint16_t              serial_state;
} app_usbd_cdc_acm_notify_t;

/**
 * @brief CDC ACM class specific request handled via control endpoint
 * */
typedef struct {
    uint8_t type;    //!< Request type
    uint8_t len;     //!< Request length

    union {
        app_usbd_cdc_line_coding_t line_coding;  //!< CDC ACM current line coding
        app_usbd_cdc_acm_notify_t  notify;       //!< CDC ACM class notify
    } payload;
} app_usbd_cdc_acm_req_t;


/**
 * @brief CDC ACM class context
 * */
typedef struct {
    app_usbd_cdc_acm_req_t     request;     //!< CDC ACM class request
    app_usbd_cdc_line_coding_t line_coding; //!< CDC ACM current line coding

    uint16_t line_state;                    //!< CDC ACM line state bitmap, DTE side
    uint16_t serial_state;                  //!< CDC ACM serial state bitmap, DCE side
} app_usbd_cdc_acm_ctx_t;


/**
 * @brief CDC ACM class configuration macro
 *
 * Used by @ref APP_USBD_CDC_ACM_GLOBAL_DEF
 *
 * @param iface_comm  Interface number of cdc_acm control
 * @param epin_comm   COMM subclass IN endpoint
 * @param iface_data  Interface number of cdc_acm DATA
 * @param epin_data   COMM subclass IN endpoint
 * @param epout_data  COMM subclass OUT endpoint
 *
 * */
#define APP_USBD_CDC_ACM_CONFIG(iface_comm, epin_comm, iface_data, epin_data, epout_data)   \
        ((iface_comm, epin_comm),                                                           \
         (iface_data, epin_data, epout_data))


/**
 * @brief Specific class constant data for cdc_acm class
 *
 * @ref app_usbd_cdc_acm_inst_t
 */
#define APP_USBD_CDC_ACM_INSTANCE_SPECIFIC_DEC app_usbd_cdc_acm_inst_t inst;


/**
 * @brief Configures cdc_acm class instance
 *
 * @param descriptors           Mass storage class descriptors (raw table)
 * @param user_event_handler    User event handler
 */
#define APP_USBD_CDC_ACM_INST_CONFIG(descriptors, user_event_handler)   \
    .inst = {                                                           \
         .p_raw_desc = descriptors,                                     \
         .raw_desc_size = sizeof(descriptors),                          \
         .user_ev_handler = user_event_handler,                         \
    }

/**
 * @brief Specific class data for cdc_acm class
 *
 * @ref app_usbd_cdc_acm_ctx_t
 * */
#define APP_USBD_CDC_ACM_DATA_SPECIFIC_DEC app_usbd_cdc_acm_ctx_t ctx;


/**
 * @brief CDC ACM class descriptors config macro
 *
 * @param interface_number Interface number
 * @param ...              Extracted endpoint list
 * */
#define APP_USBD_CDC_ACM_DSC_CONFIG(interface_number, ...) {                     \
        APP_USBD_CDC_ACM_INTERFACE_DSC(interface_number,                         \
                                       0,                                        \
                                       0,                                        \
                                       APP_USBD_CDC_ACM_SUBCLASS_CDC_ACMCONTROL) \
}

/**
 * @brief Public cdc_acm class interface
 *
 * */
extern const app_usbd_class_methods_t app_usbd_cdc_acm_class_methods;

/**
 * @brief Global definition of @ref app_usbd_cdc_acm_t class
 *
 */
#define APP_USBD_CDC_ACM_GLOBAL_DEF_INTERNAL(instance_name,               \
                                             interfaces_configs,          \
                                             user_ev_handler,             \
                                             raw_descriptors)             \
    APP_USBD_CLASS_INST_GLOBAL_DEF(                                       \
        instance_name,                                                    \
        app_usbd_cdc_acm,                                                 \
        &app_usbd_cdc_acm_class_methods,                                  \
        interfaces_configs,                                               \
        (APP_USBD_CDC_ACM_INST_CONFIG(raw_descriptors, user_ev_handler))  \
    )


/** @} */



#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_CDC_ACM_INTERNAL_H__ */
