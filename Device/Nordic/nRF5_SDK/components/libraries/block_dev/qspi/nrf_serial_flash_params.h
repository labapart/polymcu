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

#ifndef NRF_SERIAL_FLASH_PARAMS_H__
#define NRF_SERIAL_FLASH_PARAMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk_common.h"

/**@file
 *
 * @defgroup nrf_serial_flash_params Serial flash memory parameters
 * @ingroup nrf_block_dev
 * @{
 *
 */

/**
 * @brief Serial flash memory parameters
 *  */
typedef struct {
    uint8_t  read_id[3];    //!< Read identification command (0x9F) result
    uint8_t  capabilities;  //!< Serial flash memory capabilities
    uint32_t size;          //!< Serial flash memory size (bytes)
    uint32_t erase_size;    //!< Serial flash memory erase unit size (bytes)
    uint32_t program_size;  //!< Serial flash memory program size (bytes)
} nrf_serial_flash_params_t;


/**
 * @brief Returns serial flash memory identification descriptor
 *
 * @param p_read_params     Memory read identification command result
 *
 * @return Serial flash memory descriptor (NULL if not found)
 * */
nrf_serial_flash_params_t const * nrf_serial_flash_params_get(const uint8_t * p_read_params);

/** @} */
#ifdef __cplusplus
}
#endif

#endif /* NRF_SERIAL_FLASH_PARAMS_H__ */
