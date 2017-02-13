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

#include "max9850.h"

#include <string.h>

ret_code_t max9850_init(max9850_config_t const * p_max9850)
{
    ret_code_t ret = NRF_SUCCESS;

    ret = nrf_drv_twi_init(&p_max9850->twi, &p_max9850->twi_cfg, NULL, NULL);
    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    nrf_drv_twi_enable(&p_max9850->twi);

    /*Probe device*/
    uint8_t rx[] = {0};
    ret = nrf_drv_twi_rx(&p_max9850->twi, p_max9850->twi_addr, rx, sizeof(rx));
    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    uint8_t regs[sizeof(max9850_regmap_t) + 1];

    regs[0] = 0x00;
    memcpy(regs + 1, &p_max9850->regmap, sizeof(max9850_regmap_t));

    /*Write configuration*/
    return nrf_drv_twi_tx(&p_max9850->twi, p_max9850->twi_addr, regs, sizeof(regs), false);
}
