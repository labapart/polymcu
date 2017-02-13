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

#ifndef APP_USBD_HID_MOUSE_DESC_H__
#define APP_USBD_HID_MOUSE_DESC_H__

/**
 * @defgroup app_usbd_hid_mouse_desc USB HID mouse descriptors
 * @ingroup app_usbd_hid_mouse
 *
 * @brief @tagAPI52840 Module with types, definitions, and API used by the HID mouse class.
 * @{
 */

/**
 * @brief Initializer of interface descriptor for HID mouse class
 *
 * @param interface_number Interface number
 * */
#define APP_USBD_HID_MOUSE_INTERFACE_DSC(interface_number)      \
        APP_USBD_HID_INTERFACE_DSC(interface_number,            \
                                   1,                           \
                                   APP_USBD_HID_SUBCLASS_BOOT,  \
                                   APP_USBD_HID_PROTO_MOUSE)

/**
 * @brief Initializer of HID descriptor for HID mouse class
 *
 * @param ... Descriptor list
 * */
#define APP_USBD_HID_MOUSE_HID_DSC(...)     \
        APP_USBD_HID_HID_DSC(__VA_ARGS__)

/**
 * @brief Initializer of endpoint descriptor for HID mouse class
 *
 * @param endpoint_number Endpoint number
 * */
#define APP_USBD_HID_MOUSE_EP_DSC(endpoint_number)  \
        APP_USBD_HID_EP_DSC(endpoint_number, 8, 1)



/**
 * @brief Example of USB HID mouse report descriptor for n button mouse
 *
 * @param bcnt  Button count, allowed values from 1 to 8
 *
 * */
#define APP_USBD_HID_MOUSE_REPORT_DSC_BUTTON(bcnt) {                \
    0x05, 0x01,       /* Usage Page (Generic Desktop),       */     \
    0x09, 0x02,       /* Usage (Mouse),                      */     \
    0xA1, 0x01,       /*  Collection (Application),          */     \
    0x09, 0x01,       /*   Usage (Pointer),                  */     \
    0xA1, 0x00,       /*  Collection (Physical),             */     \
    0x05, 0x09,       /*     Usage Page (Buttons),           */     \
    0x19, 0x01,       /*     Usage Minimum (01),             */     \
    0x29, bcnt,       /*     Usage Maximum (bcnt),           */     \
    0x15, 0x00,       /*     Logical Minimum (0),            */     \
    0x25, 0x01,       /*     Logical Maximum (1),            */     \
    0x75, 0x01,       /*     Report Size (1),                */     \
    0x95, bcnt,       /*     Report Count (bcnt),            */     \
    0x81, 0x02,       /*     Input (Data, Variable, Absolute)*/     \
    0x75, (8-(bcnt)), /*     Report Size (8-(bcnt)),         */     \
    0x95, 0x01,       /*     Report Count (1),               */     \
    0x81, 0x01,       /*     Input (Constant),               */     \
    0x05, 0x01,       /*     Usage Page (Generic Desktop),   */     \
    0x09, 0x30,       /*     Usage (X),                      */     \
    0x09, 0x31,       /*     Usage (Y),                      */     \
    0x09, 0x38,       /*     Usage (Scroll),                 */     \
    0x15, 0x81,       /*     Logical Minimum (-127),         */     \
    0x25, 0x7F,       /*     Logical Maximum (127),          */     \
    0x75, 0x08,       /*     Report Size (8),                */     \
    0x95, 0x03,       /*     Report Count (3),               */     \
    0x81, 0x06,       /*     Input (Data, Variable, Relative)*/     \
    0xC0,         /*  End Collection,                        */     \
    0xC0,         /* End Collection                          */     \
}

 /** @} */

#endif /* APP_USBD_HID_MOUSE_DESC_H__ */
