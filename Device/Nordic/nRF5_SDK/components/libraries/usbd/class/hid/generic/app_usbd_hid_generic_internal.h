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

#ifndef APP_USBD_HID_GENERIC_INTERNAL_H__
#define APP_USBD_HID_GENERIC_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup app_usbd_hid_generic_internal USB HID generic internals
 * @ingroup app_usbd_hid_generic
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID generic protocol.
 * @{
 */


/**
 * @brief Forward declaration of HID generic class type
 *
 */
APP_USBD_CLASS_FORWARD(app_usbd_hid_generic);

/**
 * @brief HID generic part of class instance data
 *
 */
typedef struct {
    app_usbd_hid_inst_t hid_inst;  //!< HID instance data
} app_usbd_hid_generic_inst_t;

/**
 * @brief HID generic context
 *
 * */
typedef struct {
    app_usbd_hid_ctx_t hid_ctx;          //!< HID class context
    nrf_atomic_u32_t   rep_request_mask; //!< Input report requests
    uint8_t            rep_in_index;     //!< Current IN report ID
    uint8_t            rep_out_index;    //!< Current OUT report ID
} app_usbd_hid_generic_ctx_t;


/**
 * @brief HID generic configuration macro
 *
 * Used by @ref APP_USBD_HID_GENERIC_GLOBAL_DEF
 *
 * @param iface         Interface number
 * @param endpoints     Endpoint list
 * */
#define APP_USBD_HID_GENERIC_CONFIG(iface, endpoints) ((iface, BRACKET_EXTRACT(endpoints)))


/**
 * @brief Specific class constant data for HID generic class
 * */
#define APP_USBD_HID_GENERIC_INSTANCE_SPECIFIC_DEC app_usbd_hid_generic_inst_t inst;

/**
 * @brief Specific class data for HID generic class
 * */
#define APP_USBD_HID_GENERIC_DATA_SPECIFIC_DEC app_usbd_hid_generic_ctx_t ctx;


/**
 * @brief   HID generic configuration for one endpoint
 *
 * @ref APP_USBD_HID_GENERIC_DSC_CONFIG
 * */
#define APP_USBD_HID_GENERIC_DSC_CONFIG_1(interface_number, report_list, ...) {         \
        APP_USBD_HID_GENERIC_INTERFACE_DSC(interface_number, NUM_VA_ARGS(__VA_ARGS__))  \
        APP_USBD_HID_GENERIC_HID_DSC(BRACKET_EXTRACT(report_list))                      \
        APP_USBD_HID_GENERIC_EP_DSC(GET_VA_ARG_1(__VA_ARGS__))                          \
}

/**
 * @brief   HID generic configuration for two endpoints
 *
 * @ref APP_USBD_HID_GENERIC_DSC_CONFIG
 * */
#define APP_USBD_HID_GENERIC_DSC_CONFIG_2(interface_number, report_list, ...) {         \
        APP_USBD_HID_GENERIC_INTERFACE_DSC(interface_number, NUM_VA_ARGS(__VA_ARGS__))  \
        APP_USBD_HID_GENERIC_HID_DSC(BRACKET_EXTRACT(report_list))                      \
        APP_USBD_HID_GENERIC_EP_DSC(GET_VA_ARG_1(__VA_ARGS__))                          \
        APP_USBD_HID_GENERIC_EP_DSC(GET_VA_ARG_1(GET_ARGS_AFTER_1_(__VA_ARGS__)))       \
}

/**
 * @brief HID generic descriptors config macro
 *
 * @ref app_usbd_hid_generic_inst_t
 *
 * @param interface_number  Interface number
 * @param report_list       Report list
 * @param ...               Endpoint list
 *
 * */
#define APP_USBD_HID_GENERIC_DSC_CONFIG(interface_number, report_list, ...)           \
        CONCAT_2(APP_USBD_HID_GENERIC_DSC_CONFIG_, NUM_VA_ARGS(__VA_ARGS__))          \
                                        (interface_number, report_list, __VA_ARGS__)


/**
 * @brief Configure internal part of HID generic instance
 *
 * @param descriptors       Raw descriptors buffer
 * @param report_buff_in    Input report buffers array
 * @param report_buff_out   Output report buffer
 * @param user_ev_handler   User event handler
 * @param ...               Hid descriptors list
 */
#define APP_USBD_HID_GENERIC_INST_CONFIG(descriptors,                         \
                                         report_buff_in,                      \
                                         report_buff_out,                     \
                                         user_ev_handler,                     \
                                         ...)                                 \
    .inst = {                                                                 \
         .hid_inst = APP_USBD_HID_INST_CONFIG(descriptors,                    \
                                              GET_VA_ARG_1(__VA_ARGS__),      \
                                              report_buff_in,                 \
                                              report_buff_out,                \
                                              user_ev_handler,                \
                                              &app_usbd_hid_generic_methods), \
    }

/**
 * @brief Public HID generic interface
 * */
extern const app_usbd_hid_methods_t app_usbd_hid_generic_methods;

/**
 * @brief Public HID generic class interface
 * */
extern const app_usbd_class_methods_t app_usbd_generic_class_methods;

/**
 * @brief Global definition of @ref app_usbd_hid_generic_t class
 *
 * @ref APP_USBD_HID_GENERIC_GLOBAL_DEF
 */
#define APP_USBD_HID_GENERIC_GLOBAL_DEF_INTERNAL(instance_name,                     \
                                                 interface_number,                  \
                                                 user_ev_handler,                   \
                                                 endpoint_list,                     \
                                                 hid_dsc_list,                      \
                                                 report_cnt,                        \
                                                 report_out_maxsize)                \
    static const uint8_t CONCAT_2(instance_name, _dsc)[] =                          \
                 APP_USBD_HID_GENERIC_DSC_CONFIG(interface_number,                  \
                                                 hid_dsc_list,                      \
                                                 BRACKET_EXTRACT(endpoint_list));   \
    static app_usbd_hid_report_buffer_t CONCAT_2(instance_name, _in)[report_cnt];   \
    APP_USBD_HID_GENERIC_GLOBAL_OUT_REP_DEF(CONCAT_2(instance_name, _out),          \
                                            report_out_maxsize + 1);                \
    APP_USBD_CLASS_INST_GLOBAL_DEF(                                                 \
        instance_name,                                                              \
        app_usbd_hid_generic,                                                       \
        &app_usbd_generic_class_methods,                                            \
        APP_USBD_HID_GENERIC_CONFIG(interface_number, endpoint_list),               \
        (APP_USBD_HID_GENERIC_INST_CONFIG(CONCAT_2(instance_name, _dsc),            \
                                          CONCAT_2(instance_name, _in),             \
                                          &CONCAT_2(instance_name, _out),           \
                                          user_ev_handler,                          \
                                          BRACKET_EXTRACT(hid_dsc_list)))           \
    )


/** @} */


#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_HID_GENERIC_INTERNAL_H__ */
