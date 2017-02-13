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

#ifndef APP_USBD_HID_KBD_DESC_H__
#define APP_USBD_HID_KBD_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup app_usbd_hid_kbd_desc USB HID keyboard descriptors
 * @ingroup app_usbd_hid_kbd
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID keyboard class.
 * @{
 */

/**
 * @brief Initializer of interface descriptor for HID keyboard class
 *
 * @param interface_number  Interface number
 * */
#define APP_USBD_HID_KBD_INTERFACE_DSC(interface_number)            \
        APP_USBD_HID_INTERFACE_DSC(interface_number,                \
                                   1,                               \
                                   APP_USBD_HID_SUBCLASS_BOOT,      \
                                   APP_USBD_HID_PROTO_KEYBOARD)

/**
 * @brief Initializer of HID descriptor for HID keyboard class
 *
 * @param ...   Report descriptor item
 * */
#define APP_USBD_HID_KBD_HID_DSC(...)       \
        APP_USBD_HID_HID_DSC(__VA_ARGS__)
/**
 * @brief Initializer of endpoint descriptor for HID keyboard class
 *
 * @param endpoint  Endpoint number
 * */
#define APP_USBD_HID_KBD_EP_DSC(endpoint)   \
        APP_USBD_HID_EP_DSC(endpoint, 8, 1)


/**
 * @brief Example of USB HID keyboard report descriptor
 *
 * */
#define APP_USBD_HID_KBD_REPORT_DSC() {                                                    \
        0x05, 0x01,                    /* USAGE_PAGE (Generic Desktop)                   */\
        0x09, 0x06,                    /* USAGE (Keyboard)                               */\
        0xa1, 0x01,                    /* COLLECTION (Application)                       */\
        0x05, 0x07,                    /*   USAGE_PAGE (Keyboard)                        */\
        0x19, 0xe0,                    /*   USAGE_MINIMUM (Keyboard LeftControl)         */\
        0x29, 0xe7,                    /*   USAGE_MAXIMUM (Keyboard Right GUI)           */\
        0x15, 0x00,                    /*   LOGICAL_MINIMUM (0)                          */\
        0x25, 0x01,                    /*   LOGICAL_MAXIMUM (1)                          */\
        0x75, 0x01,                    /*   REPORT_SIZE (1)                              */\
        0x95, 0x08,                    /*   REPORT_COUNT (8)                             */\
        0x81, 0x02,                    /*   INPUT (Data,Var,Abs)                         */\
        0x95, 0x01,                    /*   REPORT_COUNT (1)                             */\
        0x75, 0x08,                    /*   REPORT_SIZE (8)                              */\
        0x81, 0x03,                    /*   INPUT (Cnst,Var,Abs)                         */\
        0x95, 0x05,                    /*   REPORT_COUNT (5)                             */\
        0x75, 0x01,                    /*   REPORT_SIZE (1)                              */\
        0x05, 0x08,                    /*   USAGE_PAGE (LEDs)                            */\
        0x19, 0x01,                    /*   USAGE_MINIMUM (Num Lock)                     */\
        0x29, 0x05,                    /*   USAGE_MAXIMUM (Kana)                         */\
        0x91, 0x02,                    /*   OUTPUT (Data,Var,Abs)                        */\
        0x95, 0x01,                    /*   REPORT_COUNT (1)                             */\
        0x75, 0x03,                    /*   REPORT_SIZE (3)                              */\
        0x91, 0x03,                    /*   OUTPUT (Cnst,Var,Abs)                        */\
        0x95, 0x06,                    /*   REPORT_COUNT (6)                             */\
        0x75, 0x08,                    /*   REPORT_SIZE (8)                              */\
        0x15, 0x00,                    /*   LOGICAL_MINIMUM (0)                          */\
        0x25, 0x65,                    /*   LOGICAL_MAXIMUM (101)                        */\
        0x05, 0x07,                    /*   USAGE_PAGE (Keyboard)                        */\
        0x19, 0x00,                    /*   USAGE_MINIMUM (Reserved (no event indicated))*/\
        0x29, 0x65,                    /*   USAGE_MAXIMUM (Keyboard Application)         */\
        0x81, 0x00,                    /*   INPUT (Data,Ary,Abs)                         */\
        0xc0                           /* END_COLLECTION                                 */\
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_HID_KBD_DESC_H__ */
