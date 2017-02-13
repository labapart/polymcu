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

#ifndef APP_USBD_HID_KBD_INTERNAL_H__
#define APP_USBD_HID_KBD_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup app_usbd_hid_kbd_internal USB HID keyboard internals
 * @ingroup app_usbd_hid_kbd
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID keyboard class.
 * @{
 */

/**
 * @brief Forward declaration of HID keyboard class type
 *
 */
APP_USBD_CLASS_FORWARD(app_usbd_hid_kbd);


/**
 * @brief HID keyboard part of class instance data
 *
 */
typedef struct {
    app_usbd_hid_inst_t hid_inst;  //!< HID instance data
} app_usbd_hid_kbd_inst_t;

/**
 * @brief HID keyboard context
 *
 * */
typedef struct {
    app_usbd_hid_ctx_t hid_ctx; //!< HID class context

    struct app_usbd_hid_kbd_ctx_internal_s {
        uint8_t modifier;       //!< Keyboard modifier state @ref app_usbd_hid_kbd_modifier_t
        uint8_t reserved;       //!< Reserved value
        uint8_t key_table[6];   //!< Keyboard keys table @ref app_usbd_hid_kbd_codes_t
    } rep;

    uint8_t report_buff[8];     //!< Raw report buffer
    uint8_t leds_state;         //!< Output report LEDs state
    uint8_t set_report;         //!< Set report flag
} app_usbd_hid_kbd_ctx_t;

/**
 * @brief HID keyboard configuration macro
 *
 * Used by @ref APP_USBD_HID_KBD_GLOBAL_DEF
 *
 *
 * */
#define APP_USBD_HID_KBD_CONFIG(iface, ep) ((iface, ep))


/**
 * @brief Specific class constant data for HID keyboard class
 * */
#define APP_USBD_HID_KBD_INSTANCE_SPECIFIC_DEC app_usbd_hid_kbd_inst_t inst;

/**
 * @brief Specific class data for HID keyboard class
 * */
#define APP_USBD_HID_KBD_DATA_SPECIFIC_DEC app_usbd_hid_kbd_ctx_t ctx;


/**
 * @brief HID keyboard descriptors config macro
 *
 * @ref app_usbd_hid_kbd_inst_t
 *
 * @param interface_number  Interface number
 * @param endpoint          Endpoint number
 * @param rep_descriptor    Keyboard report descriptor
 * */
#define APP_USBD_HID_KBD_DSC_CONFIG(interface_number, endpoint, rep_descriptor) {   \
        APP_USBD_HID_KBD_INTERFACE_DSC(interface_number)                            \
        APP_USBD_HID_KBD_HID_DSC(rep_descriptor)                                    \
        APP_USBD_HID_KBD_EP_DSC(endpoint)                                           \
}

/**
 * @brief Configure internal part of HID keyboard instance
 *
 * @param descriptors       Raw descriptors buffer
 * @param report_desc       Report descriptor
 * @param report_buff_in    Input report buffers array
 * @param report_buff_out   Output report buffer
 * @param user_ev_handler   User event handler
 */
#define APP_USBD_HID_KBD_INST_CONFIG(descriptors,                         \
                                     report_desc,                         \
                                     report_buff_in,                      \
                                     report_buff_out,                     \
                                     user_ev_handler)                     \
    .inst = {                                                             \
         .hid_inst = APP_USBD_HID_INST_CONFIG(descriptors,                \
                                              report_desc,                \
                                              report_buff_in,             \
                                              report_buff_out,            \
                                              user_ev_handler,            \
                                              &app_usbd_hid_kbd_methods), \
}


/**
 * @brief Public HID keyboard interface
 * */
extern const app_usbd_hid_methods_t app_usbd_hid_kbd_methods;

/**
 * @brief Public HID keyboard class interface
 * */
extern const app_usbd_class_methods_t app_usbd_hid_kbd_class_methods;

/**
 * @brief Global definition of @ref app_usbd_hid_kbd_t class
 *
 * @ref  APP_USBD_HID_KBD_GLOBAL_DEF
 */
#define APP_USBD_HID_KBD_GLOBAL_DEF_INTERNAL(instance_name,                        \
                                             interface_number,                     \
                                             endpoint,                             \
                                             user_ev_handler)                      \
    static const uint8_t CONCAT_2(instance_name, _rep_dsc)[] =                     \
        APP_USBD_HID_KBD_REPORT_DSC();                                             \
    static const uint8_t CONCAT_2(instance_name, _dsc)[] =                         \
                 APP_USBD_HID_KBD_DSC_CONFIG(interface_number,                     \
                                             endpoint,                             \
                                             CONCAT_2(instance_name, _rep_dsc));   \
    APP_USBD_HID_GENERIC_GLOBAL_OUT_REP_DEF(CONCAT_2(instance_name, _out), 1 + 1); \
    static app_usbd_hid_report_buffer_t CONCAT_2(instance_name, _in)[1];           \
    APP_USBD_CLASS_INST_GLOBAL_DEF(                                                \
        instance_name,                                                             \
        app_usbd_hid_kbd,                                                          \
        &app_usbd_hid_kbd_class_methods,                                           \
        APP_USBD_HID_KBD_CONFIG(interface_number, endpoint),                       \
        (APP_USBD_HID_KBD_INST_CONFIG(CONCAT_2(instance_name, _dsc),               \
                                      CONCAT_2(instance_name, _rep_dsc),           \
                                      CONCAT_2(instance_name, _in),                \
                                      &CONCAT_2(instance_name, _out),              \
                                      user_ev_handler))                            \
    )


/** @} */


#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_HID_KBD_INTERNAL_H__ */
