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

#include "uda1380.h"
#include <string.h>

ret_code_t uda1380_init(uda1380_iface_t const * p_iface,
                        uda1380_reg_t const * p_reg_config,
                        size_t reg_size)
{
    ret_code_t ret = NRF_SUCCESS;

    ret = nrf_drv_twi_init(&p_iface->twi, &p_iface->twi_cfg, NULL, NULL);
    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    nrf_drv_twi_enable(&p_iface->twi);

    /*Probe device*/
    uint8_t rx[] = {0};
    ret = nrf_drv_twi_rx(&p_iface->twi, p_iface->twi_addr, rx, sizeof(rx));
    if (ret != NRF_SUCCESS)
    {
        return ret;
    }

    for (size_t i = 0; i < reg_size; ++i)
    {
        uint8_t p_dat[sizeof(uda1380_reg_t)];
        memcpy(p_dat, &p_reg_config[i], sizeof(uda1380_reg_t));
        ret = nrf_drv_twi_tx(&p_iface->twi, p_iface->twi_addr, p_dat, sizeof(uda1380_reg_t), false);
        if (ret != NRF_SUCCESS)
        {
            break;
        }
    }

    return ret;
}


ret_code_t uda1380_enable(uda1380_iface_t const * p_iface)
{
    ret_code_t ret = NRF_SUCCESS;

    static const uda1380_reg_t enable[] = {
            UDA1380_REG_INIT(UDA1380_REG_PWR, 0xA500),
            UDA1380_REG_INIT(UDA1380_REG_CLK, 0x0332),
    };

    for (size_t i = 0; i < ARRAY_SIZE(enable); ++i)
    {
        uint8_t p_dat[sizeof(uda1380_reg_t)];
        memcpy(p_dat, &enable[i], sizeof(uda1380_reg_t));
        ret = nrf_drv_twi_tx(&p_iface->twi, p_iface->twi_addr, p_dat, sizeof(uda1380_reg_t), false);
        if (ret != NRF_SUCCESS)
        {
            break;
        }
    }

    return ret;
}


ret_code_t uda1380_disable(uda1380_iface_t const * p_iface)
{
    ret_code_t ret = NRF_SUCCESS;

    static const uda1380_reg_t disable[] = {
            UDA1380_REG_INIT(UDA1380_REG_PWR, 0x0000),
            UDA1380_REG_INIT(UDA1380_REG_CLK, 0x0000),
    };

    for (size_t i = 0; i < ARRAY_SIZE(disable); ++i)
    {
        const uint8_t * p_dat = (const uint8_t *)&disable[i];
        ret = nrf_drv_twi_tx(&p_iface->twi, p_iface->twi_addr, p_dat, sizeof(uda1380_reg_t), false);
        if (ret != NRF_SUCCESS)
        {
            break;
        }
    }

    return ret;
}
