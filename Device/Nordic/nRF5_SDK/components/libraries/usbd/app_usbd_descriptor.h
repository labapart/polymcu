/* Copyright (c) Nordic Semiconductor. All Rights Reserved.
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
#ifndef APP_USBD_DESCRIPTOR_H__
#define APP_USBD_DESCRIPTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf.h"
#include "nrf_drv_usbd.h"
#include "app_usbd_langid.h"
#include "app_util_platform.h"

/* Compiler support for anonymous unions */
ANON_UNIONS_ENABLE

/**
 * @defgroup app_usbd_descriptor USB standard descriptors
 * @ingroup app_usbd
 *
 * @brief @tagAPI52840 Module with types definitions used for standard descriptors.
 * @{
 */

/**
 * @brief Helper macro for translating unsigned 24 bit value to 2 byte raw descriptor
 * */
#define APP_USBD_U16_TO_RAW_DSC(val) (uint8_t)(val),                    \
                                     (uint8_t)(((val) / (256)))

/**
 * @brief Helper macro for translating unsigned 24 bit value to 3 byte raw descriptor
 * */
#define APP_USBD_U24_TO_RAW_DSC(val) (uint8_t)(val),                    \
                                     (uint8_t)(((val) / (256))),        \
                                     (uint8_t)(((val) / (256 * 256)))

/**
 * @brief Helper macro for translating unsigned 32 bit value to 4 byte raw descriptor
 * */
#define APP_USBD_U32_TO_RAW_DSC(val) (uint8_t)(val),                    \
                                     (uint8_t)(((val) / (256))),        \
                                     (uint8_t)(((val) / (256 * 256)))   \
                                     (uint8_t)(((val) / (256 * 256 * 256)))
/**
 * @brief Descriptor types
 *
 * Descriptor types used in two situations:
 * - when processing @ref APP_USBD_SETUP_STDREQ_GET_DESCRIPTOR SETUP request,
 *   the required descriptor type may be placed in wValue in HighByte.
 * - As a descriptor identifier itself inside descriptor stream.
 *
 * According to chapter 9.6 of USB 2.0 specification, following descriptors may
 * be requested directly by GetDescriptor method:
 * - @ref APP_USBD_DESCRIPTOR_DEVICE
 * - @ref APP_USBD_DESCRIPTOR_DEVICE_QUALIFIER (not used for FullSpeed only device)
 * - @ref APP_USBD_DESCRIPTOR_CONFIGURATION
 * - @ref APP_USBD_DESCRIPTOR_STRING
 */
typedef enum
{
    APP_USBD_DESCRIPTOR_DEVICE                    =  1, /**< Device descriptor. */
    APP_USBD_DESCRIPTOR_CONFIGURATION             =  2, /**<
                                                         * Specific configuration descriptor.
                                                         * Configuration descriptor is always followed by all the related interface
                                                         * and endpoints descriptors.
                                                         */
    APP_USBD_DESCRIPTOR_STRING                    =  3, /**< String descriptor. */
    APP_USBD_DESCRIPTOR_INTERFACE                 =  4, /**<
                                                         * Interface descriptor followed by all the related endpoints descriptors.
                                                         *
                                                         * @note It is returned together with @ref APP_USBD_DESCRIPTOR_CONFIGURATION.
                                                         *       Cannot be accessed by GetDescriptor or SetDescriptor
                                                         */
    APP_USBD_DESCRIPTOR_ENDPOINT                  =  5, /**<
                                                         * Endpoint descriptor.
                                                         *
                                                         * @note It is returned together with @ref APP_USBD_DESCRIPTOR_CONFIGURATION.
                                                         *       Cannot be accessed by GetDescriptor or SetDescriptor
                                                         */
    APP_USBD_DESCRIPTOR_DEVICE_QUALIFIER          =  6, /**< @note Not supported - used only in HighSpeed capable devices. */
    APP_USBD_DESCRIPTOR_OTHER_SPEED_CONFIGURATION =  7, /**< @note Not supported - our USB implementation supports only one speed. */
    APP_USBD_DESCRIPTOR_INTERFACE_POWER           =  8, /**< @note Not supported */
    APP_USBD_DESCRIPTOR_OTG                       =  9, /**< @note Not supported - Our USB have not OTG functionality */
    APP_USBD_DESCRIPTOR_DEBUG                     = 10, /**< Debug channel descriptor if available, can be only reached by GetDescriptor */
    APP_USBD_DESCRIPTOR_INTERFACE_ASSOCIATION     = 11  /**<
                                                         * Descriptor used to describe that two or more interfaces are associated to the same function.
                                                         *
                                                         * @note It is returned together with @ref APP_USBD_DESCRIPTOR_CONFIGURATION.
                                                         *       Cannot be accessed by GetDescriptor or SetDescriptor
                                                         */

} app_usbd_descriptor_t;

/* Make all descriptors packed */
#pragma pack(push, 1)

/**
 * @brief Common descriptor header
 *
 * The header that we can find on the beginning of all descriptors that contains
 * the descriptor length and type.
 */
typedef struct
{
    uint8_t  bLength;         //!< Size of the descriptor in bytes.
    uint8_t  bDescriptorType; //!< Should equal one of @ref app_usbd_descriptor_t.
                              /**  Class specific descriptors values are defined inside classes. */
} app_usbd_descriptor_header_t;

/**
 * @brief Device descriptor
 *
 * Descriptor used for the whole device
 */
typedef struct
{
    uint8_t  bLength;            //!< Size of the descriptor in bytes.
    uint8_t  bDescriptorType;    //!< Should equal to @ref APP_USBD_DESCRIPTOR_DEVICE.
    uint16_t bcdUSB;             //!< USB Specification Release Number in Binary-Coded Decimal
    uint8_t  bDeviceClass;       //!< Device class code.
                                 /**< If 0, each interface specifies its own class information.
                                  *   0xFF for vendor-specific.
                                  */
    uint8_t  bDeviceSubClass;    //!< Subclass code.
                                 /**< If bDevice Class is set to value other than 0xFF,
                                  *   all values here are reserved for assignment by USB-IF.
                                  */
    uint8_t  bDeviceProtocol;    //!< Subclass code.
                                 /**< If 0, no specific protocol is defined on device basis.
                                  *   Each interface may define its own protocol then.
                                  *   If set to 0xFF, vendor-specific protocol is used.
                                  */
    uint8_t  bMaxPacketSize0;    //!< Maximum packet size for endpoint zero.
    uint16_t idVendor;           //!< Vendor ID (Assigned by the USB-IF).
    uint16_t idProduct;          //!< Product ID (assigned by manufacturer).
    uint16_t bcdDevice;          //!< Device release number in binary-coded decimal.
    uint8_t  iManufacturer;      //!< Index of string descriptor in describing manufacturer.
    uint8_t  iProduct;           //!< Index of string descriptor in describing product.
    uint8_t  iSerialNumber;      //!< Index of string descriptor in describing the device's serial number.
    uint8_t  bNumConfigurations; //!< Number of possible configurations.
} app_usbd_descriptor_device_t;

/**
 * @brief Attributes masks
 *
 * Masks used for attributes in configuration.
 */
typedef enum
{
    /** This is reserved descriptor that has always to be set */
    APP_USBD_DESCRIPTOR_CONFIGURATION_ATTRIBUTE_ALWAYS_SET_MASK    = 1U << 7,
    /** Attribute that informs that device is self powered */
    APP_USBD_DESCRIPTOR_CONFIGURATION_ATTRIBUTE_SELF_POWERED_MASK  = 1U << 6,
    /** Attribute that informs that device has Remove Wakeup functionality */
    APP_USBD_DESCRIPTOR_CONFIGURATION_ATTRIBUTE_REMOTE_WAKEUP_MASK = 1U << 5
} app_usbd_descriptor_configuration_attributes_t;

/**
 * @brief Configuration descriptor
 *
 * Descriptor used at the beginning of configuration response.
 */
typedef struct
{
    uint8_t  bLength;             //!< Size of the descriptor in bytes.
    uint8_t  bDescriptorType;     //!< Should equal to @ref APP_USBD_DESCRIPTOR_DEVICE.
    uint16_t wTotalLength;        //!< Total length of configuration data, including all descriptors returned after configuration itself.
    uint8_t  bNumInterfaces;      //!< Number of interfaces supportedf by this configuration
    uint8_t  bConfigurationValue; //!< Value to use as an argument to the SetConfiguration request.
    uint8_t  iConfiguration;      //!< Index of string descriptor describing this configuration.
    uint8_t  bmAttributes;        //!< Configuration characteristics.
    uint8_t  bMaxPower;           //!< Maximum power consumption. Expressed in 2&nbsp;mA units.
} app_usbd_descriptor_configuration_t;

/**
 * @brief Raw descriptor - String descriptor zero
 *
 * String descriptor sent only as a response for GetDescriptor.
 */
typedef struct
{
    uint8_t  bLength;         //!< Size of the descriptor in bytes.
    uint8_t  bDescriptorType; //!< Should equal to @ref APP_USBD_DESCRIPTOR_STRING.
    uint16_t wLANGID[];       //!< The array of LANGID codes supported by the device.
} app_usbd_descriptor_string0_t;

/**
 * @brief Raw descriptor - Any normal string
 *
 * String descriptor sent only as a response for GetDescriptor.
 */
typedef struct
{
    uint8_t  bLength;         //!< Size of the descriptor in bytes.
    uint8_t  bDescriptorType; //!< Should equal to @ref APP_USBD_DESCRIPTOR_STRING.
    uint16_t bString[];       //!< UNICODE encoded string.
} app_usbd_descriptor_string_t;


/**
 * @brief Interface descriptor
 *
 * Interface descriptor, returned as a part of configuration descriptor.
 */
typedef struct
{
    uint8_t bLength;            //!< Size of the descriptor in bytes.
    uint8_t bDescriptorType;    //!< Should equal to @ref APP_USBD_DESCRIPTOR_INTERFACE.
    uint8_t bInterfaceNumber;   //!< Number of this interface.
    uint8_t bAlternateSetting;  //!< Value used to select this alternate setting.
    uint8_t bNumEndpoints;      //!< Number of endpoints used by this interface.
    uint8_t bInterfaceClass;    //!< Class code (assigned by the USB-IF). 0xff for vendor specific.
    uint8_t bInterfaceSubClass; //!< Subclass code (assigned by the USB-IF).
    uint8_t bInterfaceProtocol; //!< Protocol code (assigned by the USB-IF). 0xff for vendor specific.
    uint8_t iInterface;         //!< Index of string descriptor describing this interface.
} app_usbd_descriptor_iface_t;

/** Offset of endpoint type attribute bits */
#define APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET 0
/** Mask of endpoint type attribute bits */
#define APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_MASK   BF_MASK(2, APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET)

/** Offset of endpoint synchronization type attribute bits */
#define APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET 2
/** Mask of endpoint synchronization type attribute bits */
#define APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_MASK   BF_MASK(2, APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET)

/** Offset of endpoint usage type attribute bits */
#define APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_OFFSET 4
/** Mask of endpoint usage type attribute bits */
#define APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_MASK   BF_MASK(2, APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_OFFSET)

/**
 * @brief Endpoint attributes mnemonics
 *
 * @sa APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_MASK
 * @sa APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_MASK
 * @sa APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_OFFSET APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_MASK
 */
typedef enum
{
    APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_CONTROL      = 0 << APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_ISOCHRONOUS  = 1 << APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_BULK         = 2 << APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_INTERRUPT    = 3 << APP_USBD_DESCRIPTOR_EP_ATTR_TYPE_OFFSET,

    APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_NONE         = 0 << APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_ASYNCHRONOUS = 1 << APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_ADAPTIVE     = 2 << APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_SYNCHRONOUS  = 3 << APP_USBD_DESCRIPTOR_EP_ATTR_SYNC_OFFSET,

    APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_DATA        = 0 << APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_FEEDBACK    = 1 << APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_OFFSET,
    APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_IMPLICIT    = 2 << APP_USBD_DESCRIPTOR_EP_ATTR_USAGE_OFFSET
} app_usbd_descriptor_ep_attr_bitmap_t;

/**
 * @brief Endpoint descriptor
 *
 * Endpoint descriptor, returned as a part of configuration descriptor.
 */
typedef struct
{
    uint8_t  bLength;          //!< Size of the descriptor in bytes.
    uint8_t  bDescriptorType;  //!< Should equal to @ref APP_USBD_DESCRIPTOR_ENDPOINT.
    uint8_t  bEndpointAddress; //!< Endpoint address
    uint8_t  bmAttributes;     //!< Endpoint attributes
    uint16_t wMaxPacketSize;   //!< Maximum packet size this endpoint is capable of handling.
    uint8_t  bInterval;        //!< Interval for pooling endpoint for data transfers.
} app_usbd_descriptor_ep_t;

/**
 * @brief Interface association descriptor
 */
typedef struct
{
    uint8_t bLength;          //!< size of this descriptor in bytes
    uint8_t bDescriptorType;  //!< INTERFACE descriptor type
    uint8_t bFirstInterface;  //!< Number of interface
    uint8_t bInterfaceCount;  //!< value to select alternate setting
    uint8_t bFunctionClass;   //!< Class code assigned by the USB
    uint8_t bFunctionSubClass;//!< Sub-class code assigned by the USB
    uint8_t bFunctionProtocol;//!< Protocol code assigned by the USB
    uint8_t iFunction;        //!< Index of string descriptor
} app_usbd_descriptor_iad_t;

#pragma pack(pop)
ANON_UNIONS_DISABLE

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* APP_USBD_DESCRIPTOR_H__ */
