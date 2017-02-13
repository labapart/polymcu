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

#ifndef APP_USBD_STRING_DESC_H__
#define APP_USBD_STRING_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "sdk_common.h"
#include "app_usbd.h"
#include "app_usbd_string_config.h"

/**
 * @defgroup app_usbd_string_desc USBD string descriptors
 * @ingroup app_usbd
 *
 * @brief @tagAPI52840 USBD string descriptors management.
 * @{
 */

/**
 * @brief USB string initialization
 *
 * Macro that creates initialization values for USB string.
 * The format contains header and string itself.
 * The string should be declared as an array of uint16_t type.
 *
 * @param[in] ... Comma separated string letters or language ID.
 * @return String descriptor initialization data.
 */
#define APP_USBD_STRING_DESC(...) (const uint16_t[]){  \
    (0xff & (sizeof((uint16_t[]){__VA_ARGS__}) + 2)) | \
    ((uint16_t)APP_USBD_DESCRIPTOR_STRING) << 8,       \
    __VA_ARGS__ }

/**
 * @brief USB string descriptors ID's
 */
typedef enum {
    APP_USBD_STRING_ID_LANGIDS = 0,  /**< Supported language identifiers */
    APP_USBD_STRING_ID_MANUFACTURER, /**< Manufacturer name */
    APP_USBD_STRING_ID_PRODUCT,      /**< Product name */
    APP_USBD_STRING_ID_SERIAL,       /**< Serial number */

#define X(mnemonic, str_idx, ...) mnemonic str_idx,
    APP_USBD_STRINGS_USER
#undef X

    APP_USBD_STRING_ID_CNT           /**< Total number of identifiers */
} app_usbd_string_desc_idx_t;

/**
 * @brief Get string descriptor
 *
 * @param[in] idx    String descriptor index
 * @param[in] langid Selected language for the string
 * @return String descriptor or NULL if not exist
 * */
uint16_t const * app_usbd_string_desc_get(app_usbd_string_desc_idx_t idx, uint16_t langid);

/**
 * @brief Get string length
 *
 * Function to get string length from descriptor (descriptor returned by @ref app_usbd_string_desc_get)
 *
 * @param[in] p_str String descriptor pointer
 * @return Total descriptor length in bytes
 */
static inline size_t app_usbd_string_desc_length(uint16_t const * p_str)
{
    return ((const app_usbd_descriptor_string_t *)p_str)->bLength;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_STRING_DESC_H__ */
