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

#ifndef APP_USBD_MSC_TYPES_H__
#define APP_USBD_MSC_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_util.h"

/**
 * @defgroup app_usbd_msc_types USB MSC types
 * @ingroup app_usbd_msc
 *
 * @brief @tagAPI52840 Types used in the USB MSC class.
 * @{
 */

/** @brief MSC class definition in interface descriptor
 *
 *  @ref app_usbd_descriptor_iface_t::bInterfaceClass
 * */
#define  APP_USBD_MSC_CLASS                  0x08

/**
 * @brief MSC subclass possible value
 */
typedef enum {
    APP_USBD_MSC_SUBCLASS_RBC          = 0x01,    /**< Reduced Block Commands */
    APP_USBD_MSC_SUBCLASS_ATAPI        = 0x02,    /**< CD/DVD devices         */
    APP_USBD_MSC_SUBCLASS_QIC_157      = 0x03,    /**< Tape devices           */
    APP_USBD_MSC_SUBCLASS_UFI          = 0x04,    /**< Floppy disk drives     */
    APP_USBD_MSC_SUBCLASS_SFF_8070I    = 0x05,    /**< Floppy disk drives     */
    APP_USBD_MSC_SUBCLASS_TRANSPARENT  = 0x06,    /**< Determined by INQUIRY  */
} app_usbd_msc_subclass_t;

/**
 * @brief MSC protocol possible value
 *
 * @note The USB Mass Storage Class Control/Bulk/Interrupt (CBI) Transport specification is approved
 *       for use only with full-speed floppy disk drives. CBI shall not be used in high-speed
 *       capable devices.
 */
typedef enum {
    APP_USBD_MSC_PROTOCOL_CBI      = 0x00,    /**< Command/Bulk/Interrupt */
    APP_USBD_MSC_PROTOCOL_CBI_ALT  = 0x01,    /**< W/o command completion */
    APP_USBD_MSC_PROTOCOL_BULK     = 0x50,    /**< Bulk-only              */
} app_usbd_msc_protocol_t;

/**
 * @brief MSC USB requests @ref nrf_drv_usbd_setup_t::bmRequestType
 *
 * @note Requests are limited only to @ref APP_USBD_MSC_PROTOCOL_BULK protocol type.
 */
typedef enum {
    APP_USBD_MSC_REQ_BULK_RESET  = 0xFF, /**< Mass Storage Reset */
    APP_USBD_MSC_REQ_GET_MAX_LUN = 0xFE, /**< Get Max LUN        */
} app_usbd_msc_req_t;

#pragma pack(push, 1)

#define  APP_USBD_MSC_CBW_SIGNATURE          "USBC"      /**< CBW signature      */
#define  APP_USBD_MSC_CBW_DIRECTION_IN       (1u <<7)    /**< CBW direction flag */

/**
 * @brief Command Block Wrapper (CBW)
 */
typedef struct {
    uint8_t signature[4];           /**< "USBC" (hex: 0x43425355 little-endian)        */
    uint8_t tag[4];                 /**< Unique command tag                            */
    uint8_t datlen[4];              /**< Number of bytes that host expects to transfer */
    uint8_t flags;                  /**< Bit 7: Direction=IN                           */
    uint8_t lun;                    /**< Logical Unit Number, equals to @ref app_usbd_msc_inst_t :: block_devs_count*/
    uint8_t cdb_length;             /**< Length of cdb field                           */
    uint8_t cdb[16];                /**< Command Data Block payload                    */
} app_usbd_msc_cbw_t;

#define  APP_USBD_MSC_CSW_SIGNATURE          "USBS"  /**< CSW signature              */

#define  APP_USBD_MSC_CSW_STATUS_PASS        0x00    /**< CSW status: Command Passed */
#define  APP_USBD_MSC_CSW_STATUS_FAIL        0x01    /**< CSW status: Command Failed */
#define  APP_USBD_MSC_CSW_STATUS_PE          0x02    /**< CSW status: Phase Error    */

/**
 * @brief Command Status Wrapper (CSW)
 */
typedef struct  {
    uint8_t signature[4];           /**< "USBS" (hex: 0x53425355 little-endian)               */
    uint8_t tag[4];                 /**< Unique command tag (@ref app_usbd_msc_cbw_t :: tag)  */
    uint8_t residue[4];             /**< Amount not transferred                               */
    uint8_t status;                 /**< Status of transfer                                   */
} app_usbd_msc_csw_t;

#pragma pack(pop)


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_MSC_TYPES_H__ */
