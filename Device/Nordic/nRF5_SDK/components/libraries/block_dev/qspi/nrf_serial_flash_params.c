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

#include "nrf_serial_flash_params.h"

static const nrf_serial_flash_params_t m_sflash_params[] = {
    {    /*MXIC MX25R6435F*/
        .read_id = { 0xC2, 0x28, 0x17 },
        .capabilities = 0x00,
        .size = 8 * 1024 * 1024,
        .erase_size = 4 * 1024,
        .program_size = 256,
    }
};

nrf_serial_flash_params_t const * nrf_serial_flash_params_get(const uint8_t * p_read_id)
{
    size_t i;

    for (i = 0; i < ARRAY_SIZE(m_sflash_params); ++i)
    {
        if (memcmp(m_sflash_params[i].read_id, p_read_id, sizeof(m_sflash_params[i].read_id)) == 0)
        {
            return &m_sflash_params[i];
        }
    }

    return NULL;
}
