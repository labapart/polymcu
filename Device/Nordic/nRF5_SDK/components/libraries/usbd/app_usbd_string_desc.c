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
#include "sdk_config.h"
#if APP_USBD_ENABLED
#include "app_usbd_string_desc.h"
#include "app_usbd_langid.h"
#include "app_usbd_core.h"
#include "nordic_common.h"

/**
 * @defgroup app_usbd_string_desc
 * @ingroup app_usbd
 *
 * USBD string descriptors management
 * @{
 */

/**
 * @brief Array with language identifiers
 *
 * This array would be used to search the proper string for selected language.
 */
static const uint16_t m_langids[] = { APP_USBD_STRINGS_LANGIDS };

/**
 * @brief Language ID descriptor
 *
 * Language
 */


/**
 * @brief Mnemonics for the string positions in the array
 *
 * The mnemonics for the indexes of the strings inside the string array.
 */
enum {
    APP_USBD_STRING_ID_LANGIDS_ARRAY_POS = 0,  /**< Supported language identifiers */
    APP_USBD_STRING_ID_MANUFACTURER_ARRAY_POS, /**< Manufacturer name */
    APP_USBD_STRING_ID_PRODUCT_ARRAY_POS,      /**< Product name */
    APP_USBD_STRING_ID_SERIAL_ARRAY_POS,       /**< Serial number */
#define X(mnemonic, str_idx, ...) CONCAT_2(mnemonic, _ARRAY_POS),
    APP_USBD_STRINGS_USER
#undef X
};

/**
 * @brief String index into internal array index conversion table
 *
 * The array that transforms the USB string indexes into internal array position.
 * @note Value 0 used to mark non-existing string.
 */
static const uint8_t m_string_translation[APP_USBD_STRING_ID_CNT] =
{
    [APP_USBD_STRING_ID_LANGIDS     ] = APP_USBD_STRING_ID_LANGIDS_ARRAY_POS,
    [APP_USBD_STRING_ID_MANUFACTURER] = APP_USBD_STRING_ID_MANUFACTURER_ARRAY_POS,
    [APP_USBD_STRING_ID_PRODUCT     ] = APP_USBD_STRING_ID_PRODUCT_ARRAY_POS,
    [APP_USBD_STRING_ID_SERIAL      ] = APP_USBD_STRING_ID_SERIAL_ARRAY_POS,
#define X(mnemonic, str_idx, ...) [mnemonic] = CONCAT_2(mnemonic, _ARRAY_POS),
    APP_USBD_STRINGS_USER
#undef X
};

#ifndef APP_USBD_STRINGS_MANUFACTURER_EXTERN
#define APP_USBD_STRINGS_MANUFACTURER_EXTERN 0
#endif

#ifndef APP_USBD_STRINGS_PRODUCT_EXTERN
#define APP_USBD_STRINGS_PRODUCT_EXTERN 0
#endif

#ifndef APP_USBD_STRING_SERIAL_EXTERN
#define APP_USBD_STRING_SERIAL_EXTERN 0
#endif

#if APP_USBD_STRINGS_MANUFACTURER_EXTERN
extern uint16_t APP_USBD_STRINGS_MANUFACTURER[];
#endif

#if APP_USBD_STRINGS_PRODUCT_EXTERN
extern uint16_t APP_USBD_STRINGS_PRODUCT[];
#endif

#if APP_USBD_STRING_SERIAL_EXTERN
extern uint16_t APP_USBD_STRING_SERIAL[];
#endif


/**
 * @brief String descriptors table
 * */
static const uint16_t * m_string_dsc[APP_USBD_STRING_ID_CNT][ARRAY_SIZE(m_langids)] =
{
    [APP_USBD_STRING_ID_LANGIDS_ARRAY_POS]      = { APP_USBD_STRING_DESC(APP_USBD_STRINGS_LANGIDS) },
    [APP_USBD_STRING_ID_MANUFACTURER_ARRAY_POS] = { APP_USBD_STRINGS_MANUFACTURER },
    [APP_USBD_STRING_ID_PRODUCT_ARRAY_POS]      = { APP_USBD_STRINGS_PRODUCT },
    [APP_USBD_STRING_ID_SERIAL_ARRAY_POS]       = { APP_USBD_STRING_SERIAL },
#define X(mnemonic, str_idx, ...) [CONCAT_2(mnemonic, _ARRAY_POS)] = { __VA_ARGS__ },
    APP_USBD_STRINGS_USER
#undef X
};



uint16_t const * app_usbd_string_desc_get(app_usbd_string_desc_idx_t idx, uint16_t langid)
{
    /* LANGID string */
    if (APP_USBD_STRING_ID_LANGIDS == idx)
    {
        return m_string_dsc[APP_USBD_STRING_ID_LANGIDS_ARRAY_POS][0];
    }

    /* Searching for the language */
    uint8_t lang_idx = 0;
    if (ARRAY_SIZE(m_langids) > 1)
    {
        while(m_langids[lang_idx] != langid)
        {
            if(++lang_idx >= ARRAY_SIZE(m_langids))
            {
                return NULL;
            }
        }
    }

    /* Get the string index in array */
    if (idx >= ARRAY_SIZE(m_string_translation))
    {
        return NULL;
    }

    uint8_t str_pos = m_string_translation[idx];
    if (str_pos == 0)
    {
        return NULL;
    }

    uint16_t const * p_str = m_string_dsc[str_pos][lang_idx];
    if ((ARRAY_SIZE(m_langids) > 1) && (lang_idx != 0))
    {
        if (p_str == NULL)
        {
            p_str = m_string_dsc[str_pos][0];
        }
    }

    return p_str;
}

/** @} */
#endif // APP_USBD_ENABLED
