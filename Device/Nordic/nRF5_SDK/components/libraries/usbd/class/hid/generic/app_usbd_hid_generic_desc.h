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

#ifndef APP_USBD_HID_GENERIC_DESC_H__
#define APP_USBD_HID_GENERIC_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup app_usbd_hid_generic_desc USB HID generic descriptors
 * @ingroup app_usbd_hid_generic
 *
 * @brief @tagAPI52840 Module with descriptors used by the HID generic class.
 * @{
 */

/**
 * @brief Initializer of interface descriptor for HID generic class
 *
 * @param interface_number  Interface number
 * @param endpoints_num     Endpoint number
 * */
#define APP_USBD_HID_GENERIC_INTERFACE_DSC(interface_number, endpoints_num)     \
        APP_USBD_HID_INTERFACE_DSC(interface_number, endpoints_num,             \
                                   APP_USBD_HID_SUBCLASS_NONE, APP_USBD_HID_PROTO_GENERIC)

/**
 * @brief Initializer of HID descriptor for HID generic class
 *
 * @param ...   Report descriptor item
 * */
#define APP_USBD_HID_GENERIC_HID_DSC(...)   \
        APP_USBD_HID_HID_DSC(__VA_ARGS__)
/**
 * @brief Initializer of endpoint descriptor for HID generic class
 *
 * @param endpoint  Endpoint number
 * */
#define APP_USBD_HID_GENERIC_EP_DSC(endpoint) \
        APP_USBD_HID_EP_DSC(endpoint, NRF_DRV_USBD_EPSIZE, 1)



/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_HID_GENERIC_DESC_H__ */
