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
#ifndef APP_USBD_CDC_DESC_H__
#define APP_USBD_CDC_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "app_usbd_descriptor.h"
#include "app_usbd_cdc_types.h"

/**
 * @defgroup app_usbd_cdc_desc CDC class descriptors
 * @brief @tagAPI52840 Descriptors used in the USB CDC class implementation.
 * @ingroup app_usbd_cdc_acm
 *
 * A group of macros used to initialize CDC descriptors
 * @{
 */

/**
 * @brief Initializer of IAD descriptor for CDC class
 *
 * @param interface_number Interface number
 * @param subclass         Subclass, @ref app_usbd_cdc_subclass_t
 * @param protocol         Protocol, @ref app_usbd_cdc_comm_protocol_t
 * */
#define APP_USBD_CDC_IAD_DSC(interface_number, subclass, protocol)        \
    /*.bLength =           */ sizeof(app_usbd_descriptor_iad_t),          \
    /*.bDescriptorType =   */ APP_USBD_DESCRIPTOR_INTERFACE_ASSOCIATION,  \
    /*.bFirstInterface =   */ interface_number,                           \
    /*.bInterfaceCount =   */ 2,                                          \
    /*.bFunctionClass =    */ APP_USBD_CDC_COMM_CLASS,                    \
    /*.bFunctionSubClass = */ subclass,                                   \
    /*.bFunctionProtocol = */ protocol,                                   \
    /*.iFunction =         */ 0,                                          \

/**
 * @brief Initializer of interface descriptor for CDC COMM class
 *
 * @param interface_number Interface number
 * @param subclass         Subclass, @ref app_usbd_cdc_subclass_t
 * @param protocol         Protocol, @ref app_usbd_cdc_comm_protocol_t
 * */
#define APP_USBD_CDC_COMM_INTERFACE_DSC(interface_number, subclass, protocol)  \
    /*.bLength =            */ sizeof(app_usbd_descriptor_iface_t),            \
    /*.bDescriptorType =    */ APP_USBD_DESCRIPTOR_INTERFACE,                  \
    /*.bInterfaceNumber =   */ interface_number,                               \
    /*.bAlternateSetting =  */ 0x00,                                           \
    /*.bNumEndpoints =      */ 1,                                              \
    /*.bInterfaceClass =    */ APP_USBD_CDC_COMM_CLASS,                        \
    /*.bInterfaceSubClass = */ subclass,                                       \
    /*.bInterfaceProtocol = */ protocol,                                       \
    /*.iInterface = 0,      */ 0x00,                                           \


/**
 * @brief Initializer of interface descriptor for CDC DATA class
 *
 * @param interface_number Interface number
 * @param subclass         Subclass, @ref app_usbd_cdc_subclass_t
 * @param protocol         Protocol, @ref app_usbd_cdc_data_protocol_t
 * */
#define APP_USBD_CDC_DATA_INTERFACE_DSC(interface_number, subclass, protocol) \
    /*.bLength =            */ sizeof(app_usbd_descriptor_iface_t),           \
    /*.bDescriptorType =    */ APP_USBD_DESCRIPTOR_INTERFACE,                 \
    /*.bInterfaceNumber =   */ interface_number,                              \
    /*.bAlternateSetting =  */ 0x00,                                          \
    /*.bNumEndpoints =      */ 2,                                             \
    /*.bInterfaceClass =    */ APP_USBD_CDC_DATA_CLASS,                       \
    /*.bInterfaceSubClass = */ subclass,                                      \
    /*.bInterfaceProtocol = */ protocol,                                      \
    /*.iInterface = 0,      */ 0x00,                                          \



/**
 * @brief Initializer of endpoint descriptor for CDC COM class
 *
 * @param endpoint_in   IN endpoint
 * @param ep_size       Endpoint size
 * */
#define APP_USBD_CDC_COM_EP_DSC(endpoint_in, ep_size)                     \
    /*.bLength =          */ sizeof(app_usbd_descriptor_ep_t),            \
    /*.bDescriptorType =  */ APP_USBD_DESCRIPTOR_ENDPOINT,                \
    /*.bEndpointAddress = */ endpoint_in,                                 \
    /*.bmAttributes =     */ APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_INTERRUPT,  \
    /*.wMaxPacketSize =   */ APP_USBD_U16_TO_RAW_DSC(ep_size),            \
    /*.bInterval =        */ 16,                                          \

/**
 * @brief Initializer of endpoint descriptors for CDC DATA class
 *
 * @param endpoint_in   IN endpoint
 * @param endpoint_out  OUT endpoint
 * @param ep_size       Endpoint size
 * */
#define APP_USBD_CDC_DATA_EP_DSC(endpoint_in, endpoint_out, ep_size) \
    /*.bLength =          */ sizeof(app_usbd_descriptor_ep_t),       \
    /*.bDescriptorType =  */ APP_USBD_DESCRIPTOR_ENDPOINT,           \
    /*.bEndpointAddress = */ endpoint_in,                            \
    /*.bmAttributes =     */ APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_BULK,  \
    /*.wMaxPacketSize =   */ APP_USBD_U16_TO_RAW_DSC(ep_size),       \
    /*.bInterval =        */ 0,                                      \
    /*.bLength =          */ sizeof(app_usbd_descriptor_ep_t),       \
    /*.bDescriptorType =  */ APP_USBD_DESCRIPTOR_ENDPOINT,           \
    /*.bEndpointAddress = */ endpoint_out,                           \
    /*.bmAttributes =     */ APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_BULK,  \
    /*.wMaxPacketSize =   */ APP_USBD_U16_TO_RAW_DSC(ep_size),       \
    /*.bInterval =        */ 0,                                      \

/**
 * @brief Initializer of endpoint descriptors for CDC header descriptor
 *
 * @param bcd_cdc   BCD CDC version
 */
#define APP_USBD_CDC_HEADER_DSC(bcd_cdc)                           \
    /*.bLength =            */ sizeof(app_usbd_cdc_desc_header_t), \
    /*.bDescriptorType =    */ APP_USBD_CDC_CS_INTERFACE,          \
    /*.bDescriptorSubtype = */ APP_USBD_CDC_SCS_HEADER,            \
    /*.bcdCDC =             */ APP_USBD_U16_TO_RAW_DSC(bcd_cdc),   \

/**
 * @brief Initializer of endpoint descriptors for CDC call management descriptor
 *
 * @param capabilities   Capabilities
 * @param data_interface Data interface
 */
#define APP_USBD_CDC_CALL_MGMT_DSC(capabilities, data_interface)      \
    /*.bLength =            */ sizeof(app_usbd_cdc_desc_call_mgmt_t), \
    /*.bDescriptorType =    */ APP_USBD_CDC_CS_INTERFACE,             \
    /*.bDescriptorSubtype = */ APP_USBD_CDC_SCS_CALL_MGMT,            \
    /*.bmCapabilities =     */ capabilities,                          \
    /*.bDataInterface =     */ data_interface,                        \


/**
 * @brief Initializer of endpoint descriptors for CDC DATA class
 *
 * @param capabilities   Capabilities
 */
#define APP_USBD_CDC_ACM_DSC(capabilities)                      \
    /*.bLength =            */ sizeof(app_usbd_cdc_desc_acm_t), \
    /*.bDescriptorType =    */ APP_USBD_CDC_CS_INTERFACE,       \
    /*.bDescriptorSubtype = */ APP_USBD_CDC_SCS_ACM,            \
    /*.bmCapabilities =     */ capabilities,                    \

/**
 * @brief Initializer of endpoint descriptors for CDC DATA class
 *
 * @param control_interface   Control interface
 * @param ...                 Subordinate interfaces list
 */
#define APP_USBD_CDC_UNION_DSC(control_interface, ...)                                            \
    /*.bLength =               */ sizeof(app_usbd_cdc_desc_union_t) + (NUM_VA_ARGS(__VA_ARGS__)), \
    /*.bDescriptorType =       */ APP_USBD_CDC_CS_INTERFACE,                                      \
    /*.bDescriptorSubtype =    */ APP_USBD_CDC_SCS_UNION,                                         \
    /*.bControlInterface =     */ control_interface,                                              \
    /*.bSubordinateInterface = */ __VA_ARGS__,                                                    \

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_CDC_H__ */
