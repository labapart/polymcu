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
#ifndef APP_USBD_REQUEST_H__
#define APP_USBD_REQUEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk_common.h"
#include "nrf.h"
#include "nrf_drv_usbd.h"
#include "app_usbd_descriptor.h"
#include "app_util_platform.h"

/* Compiler support for anonymous unions */
ANON_UNIONS_ENABLE

#pragma pack(push, 1)

/**
 * @defgroup app_usbd_request USB standard requests
 * @ingroup app_usbd
 *
 * @brief @tagAPI52840 Module with types definitions used for standard requests processing.
 * @{
 */

/**
 * @brief Recipient bit-field in request type
 *
 * Bits 4...0
 */
#define APP_USBD_SETUP_REQ_BF_REC BF_CX(5, 0)

/**
 * @brief Type bit-field in request type
 *
 * Bits 6...5
 */
#define APP_USBD_SETUP_REQ_BF_TYP BF_CX(2, 5)

/**
 * @brief Direction bit-field in request type
 *
 * Bit 7
 */
#define APP_USBD_SETUP_REQ_BF_DIR BF_CX(1, 7)

/**
 * @brief Recipient enumerator.
 *
 * @note It is part of @ref app_usbd_setup_reqtype_t variable type.
 */
typedef enum {
    APP_USBD_SETUP_REQREC_DEVICE    = 0x0, /**< The whole device is a request target   */
    APP_USBD_SETUP_REQREC_INTERFACE = 0x1, /**< Selected interface is a request target */
    APP_USBD_SETUP_REQREC_ENDPOINT  = 0x2, /**< Selected endpoint is a request target  */
    APP_USBD_SETUP_REQREC_OTHER     = 0x3  /**< Other element is a request target      */
} app_usbd_setup_reqrec_t;

/**
 * @brief Request type enumerator.
 *
 * @note It is part of @ref app_usbd_setup_reqtype_t variable type.
 */
typedef enum {
    APP_USBD_SETUP_REQTYPE_STD    = 0x0, /**< Standard request */
    APP_USBD_SETUP_REQTYPE_CLASS  = 0x1, /**< Class specific request */
    APP_USBD_SETUP_REQTYPE_VENDOR = 0x2  /**< Vendor specific request */
} app_usbd_setup_reqtype_t;

/**
 * @brief Direction of setup command
 *
 * @note It is part of @ref app_usbd_setup_reqtype_t variable type.
 */
typedef enum {
    APP_USBD_SETUP_REQDIR_OUT = 0x0, /**< Host to device */
    APP_USBD_SETUP_REQDIR_IN  = 0x1, /**< Device to host */
} app_usbd_setup_reqdir_t;


/**
 * @brief Standard requests
 *
 * Enumerator for standard requests values
 */
typedef enum {
    APP_USBD_SETUP_STDREQ_GET_STATUS        = 0x00, /**<
                                                     * Targets: Device, Interface, Endpoint
                                                     * Expected SETUP frame format:
                                                     * - wValue: Zero
                                                     * - wIndex: Zero, (lb): Interface or Endpoint
                                                     * - wLength: 2
                                                     * - Data:2 bytes of data, depending on targets
                                                     *    - Device:
                                                     *        - D15..D2: Reserved (Reset to zero)
                                                     *        - D1: Remove Wakeup
                                                     *        - D0: Self Powered
                                                     *    - Interface:
                                                     *        - D15..D0: Reserved (Reset to zero)
                                                     *    - Endpoint:
                                                     *        - D15..D1: Reserved (Reset to zero)
                                                     *        - D0: Halt
                                                     */
    APP_USBD_SETUP_STDREQ_CLEAR_FEATURE     = 0x01, /**<
                                                     * Targets: Device, Interface, Endpoint
                                                     * Expected SETUP frame format:
                                                     * - wValue: Feature selector (@ref app_usbd_setup_stdfeature_t)
                                                     * - wIndex: Zero, Interface or Endpoint
                                                     * - wLength: 0
                                                     * - Data: None
                                                     */
    APP_USBD_SETUP_STDREQ_SET_FEATURE       = 0x03, /**<
                                                     * Targets: Device, Interface, Endpoint
                                                     * Expected SETUP frame format:
                                                     * - wValue: Feature selector (@ref app_usbd_setup_stdfeature_t)
                                                     * - wIndex: Zero, Interface or Endpoint
                                                     * - wLength: 0
                                                     * - Data: None
                                                     */
    APP_USBD_SETUP_STDREQ_SET_ADDRESS       = 0x05, /**<
                                                     * @note This SETUP request is processed in hardware.
                                                     *       Use it only to mark current USB state.
                                                     *
                                                     * Targets: Device
                                                     * Expected SETUP frame format:
                                                     * - wValue: New device address
                                                     * - wIndex: 0
                                                     * - wLength: 0
                                                     * - Data: None
                                                     */
    APP_USBD_SETUP_STDREQ_GET_DESCRIPTOR    = 0x06, /**<
                                                     * Targets: Device
                                                     * - wValue: (hb): Descriptor Type and (lb): Descriptor Index
                                                     * - wIndex: Zero of Language ID
                                                     * - wLength: Descriptor Length
                                                     * - Data: Descriptor
                                                     */
    APP_USBD_SETUP_STDREQ_SET_DESCRIPTOR    = 0x07, /**<
                                                     * Not supported - Stall when called.
                                                     */
    APP_USBD_SETUP_STDREQ_GET_CONFIGURATION = 0x08, /**<
                                                     * Target: Device
                                                     * Expected SETUP frame format:
                                                     * - wValue: 0
                                                     * - wIndex: 0
                                                     * - wLength: 1
                                                     * - Data: Configuration value
                                                     */
    APP_USBD_SETUP_STDREQ_SET_CONFIGURATION = 0x09, /**<
                                                     * Target: Device
                                                     * Expected SETUP frame format:
                                                     * - wValue: (lb): Configuration value
                                                     * - wIndex: 0
                                                     * - wLength: 0
                                                     * - Data: None
                                                     */
    APP_USBD_SETUP_STDREQ_GET_INTERFACE     = 0x0A, /**<
                                                     * Target: Interface
                                                     * Expected SETUP frame format:
                                                     * - wValue: 0
                                                     * - wIndex: Interface
                                                     * - wLength: 1
                                                     * - Data: Alternate setting
                                                     */
    APP_USBD_SETUP_STDREQ_SET_INTERFACE     = 0x0B, /**<
                                                     * Target: Interface
                                                     * Expected SETUP frame format:
                                                     * - wValue: Alternate setting
                                                     * - wIndex: Interface
                                                     * - wLength: 0
                                                     * - Data: None
                                                     */
    APP_USBD_SETUP_STDREQ_SYNCH_FRAME       = 0x0C  /**<
                                                     * Target: Endpoint
                                                     * Expected SETUP frame format:
                                                     * - wValue: 0
                                                     * - wIndex: Endpoint
                                                     * - wLength: 2
                                                     * - Data: Frame Number
                                                     *
                                                     * @note
                                                     * This request is used only in connection with isochronous endpoints.
                                                     * This is rarely used and probably we would not need to support it.
                                                     */
} app_usbd_setup_stdrequest_t;

/**
 * @brief Standard feature selectors
 *
 * Standard features that may be disabled or enabled by
 * @ref APP_USBD_SETUP_STDREQ_CLEAR_FEATURE or @ref APP_USBD_SETUP_STDREQ_SET_FEATURE
 */
typedef enum {
    APP_USBD_SETUP_STDFEATURE_DEVICE_REMOTE_WAKEUP = 1, /**< Remote wakeup feature.
                                                         *
                                                         *   Target: Device only
                                                         */
    APP_USBD_SETUP_STDFEATURE_ENDPOINT_HALT        = 0, /**< Stall or clear the endpoint.
                                                         *
                                                         *   Target: Endpoint different than default (0)
                                                         */
    APP_USBD_SETUP_STDFEATURE_TEST_MODE            = 2  /**< Upstream port test mode.
                                                         *   Power has to be cycled to exit test mode.
                                                         *   This feature cannot be cleared.
                                                         *
                                                         *   Target: Device only
                                                         *
                                                         * @note
                                                         * It should only be supported by HighSpeed capable devices.
                                                         * Not supported in this library.
                                                         */
} app_usbd_setup_stdfeature_t;


/**
 * @brief Universal way to access 16 bit values and its parts
 */
typedef union {
    uint16_t w; //!< 16 bit access
    struct
    {
        uint8_t lb; //!< Low byte access
        uint8_t hb; //!< High byte access
    };
} app_usbd_setup_w_t;

/**
 * @brief Internal redefinition of setup structure
 *
 * Redefinition of the structure to simplify changes in the future
 * if required - app_usbd API would present setup data using app_usbd_setup_t.
 *
 * The structure layout is always the same like @ref nrf_drv_usbd_setup_t
 */
typedef struct {
    uint8_t            bmRequestType; //!< Setup type bitfield
    uint8_t            bmRequest;     //!< One of @ref app_usbd_setup_stdrequest_t values or class dependent one.
    app_usbd_setup_w_t wValue;        //!< byte 2, 3
    app_usbd_setup_w_t wIndex;        //!< byte 4, 5
    app_usbd_setup_w_t wLength;       //!< byte 6, 7
} app_usbd_setup_t;

#pragma pack(pop)


/**
 * @brief Extract recipient from request type
 *
 * @param[in] bmRequestType
 *
 * @return Extracted recipient field from request type value
 */
static inline app_usbd_setup_reqrec_t app_usbd_setup_req_rec(uint8_t bmRequestType)
{
    return (app_usbd_setup_reqrec_t)BF_CX_GET(bmRequestType, APP_USBD_SETUP_REQ_BF_REC);
}

/**
 * @brief Extract type from request type
 *
 * @param[in] bmRequestType
 *
 * @return Extracted type field from request type value
 */
static inline app_usbd_setup_reqtype_t app_usbd_setup_req_typ(uint8_t bmRequestType)
{
    return (app_usbd_setup_reqtype_t)BF_CX_GET(bmRequestType, APP_USBD_SETUP_REQ_BF_TYP);
}


/**
 * @brief Extract direction from request type
 *
 * @param[in] bmRequestType
 *
 * @return Extracted direction field from request type value
 */
static inline app_usbd_setup_reqdir_t app_usbd_setup_req_dir(uint8_t bmRequestType)
{
    return (app_usbd_setup_reqdir_t)BF_CX_GET(bmRequestType, APP_USBD_SETUP_REQ_BF_DIR);
}

/**
 * @brief Make request type value
 *
 * @param[in] rec Recipient
 * @param[in] typ Request type
 * @param[in] dir Direction
 *
 * @return Assembled request type value
 */
static inline uint8_t app_usbd_setup_req_val(app_usbd_setup_reqrec_t  rec,
                                             app_usbd_setup_reqtype_t typ,
                                             app_usbd_setup_reqdir_t  dir)
{
    uint32_t bmRequestType = (
        BF_CX_VAL(rec, APP_USBD_SETUP_REQ_BF_REC) |
        BF_CX_VAL(typ, APP_USBD_SETUP_REQ_BF_TYP) |
        BF_CX_VAL(dir, APP_USBD_SETUP_REQ_BF_DIR)
        );

    ASSERT(bmRequestType < 256U);
    return (uint8_t)bmRequestType;
}


ANON_UNIONS_DISABLE
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_REQUEST_H__ */
