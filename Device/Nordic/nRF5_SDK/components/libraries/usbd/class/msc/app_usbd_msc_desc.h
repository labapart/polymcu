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

#ifndef APP_USBD_MSC_DESC_H__
#define APP_USBD_MSC_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_usbd_descriptor.h"

/**
 * @defgroup app_usbd_msc_desc USB MSC descriptors
 * @ingroup app_usbd_msc
 *
 * @brief @tagAPI52840 Descriptors for the USB MSC class.
 * @{
 */

/**
 * @brief Initializer of interface descriptor for MSC class
 *
 * @param interface_number Interface number
 * @param subclass         Subclass, @ref app_usbd_msc_subclass_t
 * @param protocol         Protocol, @ref app_usbd_msc_protocol_t
 * */
#define APP_USBD_MSC_INTERFACE_DSC(interface_number, subclass, protocol)                \
    /*.bLength =            */ sizeof(app_usbd_descriptor_iface_t),                     \
    /*.bDescriptorType =    */ APP_USBD_DESCRIPTOR_INTERFACE,                           \
    /*.bInterfaceNumber =   */ interface_number,                                        \
    /*.bAlternateSetting =  */ 0x00,                                                    \
    /*.bNumEndpoints =      */ 2,                                                       \
    /*.bInterfaceClass =    */ APP_USBD_MSC_CLASS,                                      \
    /*.bInterfaceSubClass = */ subclass,                                                \
    /*.bInterfaceProtocol = */ protocol,                                                \
    /*.iInterface = 0,      */ 0x00,                                                    \


/**
 * @brief Initializer of endpoint descriptors for MSC class
 *
 * @param endpoint_in   IN endpoint
 * @param endpoint_out  OUT endpoint
 * @param ep_size       Endpoint size
 * */
#define APP_USBD_MSC_EP_DSC(endpoint_in, endpoint_out, ep_size)               \
    /*.bLength =          */ sizeof(app_usbd_descriptor_ep_t),                \
    /*.bDescriptorType =  */ APP_USBD_DESCRIPTOR_ENDPOINT,                    \
    /*.bEndpointAddress = */ endpoint_in,                                     \
    /*.bmAttributes =     */ APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_BULK,           \
    /*.wMaxPacketSize =   */ APP_USBD_U16_TO_RAW_DSC(ep_size),                \
    /*.bInterval =        */ 0,                                               \
    /*.bLength =          */ sizeof(app_usbd_descriptor_ep_t),                \
    /*.bDescriptorType =  */ APP_USBD_DESCRIPTOR_ENDPOINT,                    \
    /*.bEndpointAddress = */ endpoint_out,                                    \
    /*.bmAttributes =     */ APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_BULK,           \
    /*.wMaxPacketSize =   */ APP_USBD_U16_TO_RAW_DSC(ep_size),                \
    /*.bInterval =        */ 0,                                               \


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_MSC_DESC_H__ */
