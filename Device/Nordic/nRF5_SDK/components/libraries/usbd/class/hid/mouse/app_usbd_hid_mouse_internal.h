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

#ifndef APP_USBD_HID_MOUSE_INTERNAL_H__
#define APP_USBD_HID_MOUSE_INTERNAL_H__

/**
 * @defgroup app_usbd_hid_mouse_internals USB HID mouse internals
 * @ingroup app_usbd_hid_mouse
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID mouse class.
 * @{
 */

/**
 * @brief Forward declaration of HID mouse class type
 *
 */
APP_USBD_CLASS_FORWARD(app_usbd_hid_mouse);


/**
 * @brief HID mouse part of class instance data
 *
 */
typedef struct {
    app_usbd_hid_inst_t hid_inst;      //!< HID instance data
    const uint8_t       button_count;  //!< Number of buttons mouse specific
} app_usbd_hid_mouse_inst_t;

/**
 * @brief HID mouse context
 *
 * */
typedef struct {
    app_usbd_hid_ctx_t hid_ctx; //!< HID class context

    int16_t acc_x_axis;         //!< Mouse specific. Accumulated x axis offset
    int16_t acc_y_axis;         //!< Mouse specific. Accumulated y axis offset
    int16_t acc_scroll;         //!< Mouse specific. Accumulated scroll offset
    uint8_t button_state;       //!< Mouse specific. Actual button state. Bitfield of maximum 8 buttons states.
    uint8_t report_buff[4];
} app_usbd_hid_mouse_ctx_t;


/**
 * @brief HID mouse configuration macro
 *
 * Used by @ref APP_USBD_HID_MOUSE_GLOBAL_DEF
 *
 * */
#define APP_USBD_HID_MOUSE_CONFIG(iface, ep) ((iface, ep))


/**
 * @brief Specific class constant data for HID mouse class
 * */
#define APP_USBD_HID_MOUSE_INSTANCE_SPECIFIC_DEC app_usbd_hid_mouse_inst_t inst;

/**
 * @brief Specific class data for HID mouse class
 * */
#define APP_USBD_HID_MOUSE_DATA_SPECIFIC_DEC app_usbd_hid_mouse_ctx_t ctx;


/**
 * @brief HID mouse descriptors config macro
 *
 * @ref app_usbd_hid_mouse_inst_t
 *
 * */
#define APP_USBD_HID_MOUSE_DSC_CONFIG(interface_number, endpoint, rep_desc) {  \
        APP_USBD_HID_MOUSE_INTERFACE_DSC(interface_number)                     \
        APP_USBD_HID_MOUSE_HID_DSC(rep_desc)                                   \
        APP_USBD_HID_MOUSE_EP_DSC(endpoint)                                    \
}


/**
 * @brief Configure internal part of HID mouse instance
 *
 * @param descriptors     Raw descriptors buffer
 * @param report_desc     Report descriptor
 * @param report_buff_in  Input report buffers array
 * @param report_buff_out Output report buffer
 * @param user_ev_handler User event handler
 * @param bcnt            Mouse button count
 */
#define APP_USBD_HID_MOUSE_INST_CONFIG(descriptors,                         \
                                       report_desc,                         \
                                       report_buff_in,                      \
                                       report_buff_out,                     \
                                       user_ev_handler,                     \
                                       bcnt)                                \
    .inst = {                                                               \
        .hid_inst = APP_USBD_HID_INST_CONFIG(descriptors,                   \
                                             report_desc,                   \
                                             report_buff_in,                \
                                             report_buff_out,               \
                                             user_ev_handler,               \
                                             &app_usbd_hid_mouse_methods),  \
        .button_count = bcnt,                                               \
    }

/**
 * @brief Public HID mouse interface
 * */
extern const app_usbd_hid_methods_t app_usbd_hid_mouse_methods;

/**
 * @brief Public HID mouse class interface
 * */
extern const app_usbd_class_methods_t app_usbd_hid_mouse_class_methods;

/**
 * @brief Global definition of app_usbd_hid_mouse_t class
 *
 * @ref  APP_USBD_HID_MOUSE_GLOBAL_DEF
 */
#define APP_USBD_HID_MOUSE_GLOBAL_DEF_INTERNAL(instance_name,               \
                                               interface_number,            \
                                               endpoint,                    \
                                               bcnt,                        \
                                               user_ev_handler)             \
    static const uint8_t CONCAT_2(instance_name, _rep_dsc)[] =              \
        APP_USBD_HID_MOUSE_REPORT_DSC_BUTTON(bcnt);                         \
    static const uint8_t CONCAT_2(instance_name, _dsc)[] =                  \
        APP_USBD_HID_MOUSE_DSC_CONFIG(interface_number,                     \
                                      endpoint,                             \
                                      CONCAT_2(instance_name,               \
                                               _rep_dsc));                  \
    static app_usbd_hid_report_buffer_t  CONCAT_2(instance_name, _in)[1];   \
    APP_USBD_CLASS_INST_GLOBAL_DEF(                                         \
        instance_name,                                                      \
        app_usbd_hid_mouse,                                                 \
        &app_usbd_hid_mouse_class_methods,                                  \
        APP_USBD_HID_MOUSE_CONFIG(interface_number, endpoint),              \
        (APP_USBD_HID_MOUSE_INST_CONFIG(CONCAT_2(instance_name, _dsc),      \
                                        CONCAT_2(instance_name, _rep_dsc),  \
                                        CONCAT_2(instance_name, _in),       \
                                        NULL,                               \
                                        user_ev_handler,                    \
                                        bcnt))                              \
    )


/** @} */

#endif /* APP_USBD_HID_MOUSE_INTERNAL_H__ */
