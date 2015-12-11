/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#include "ant_channel_config.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_error.h"

uint32_t ant_channel_init(ant_channel_config_t const * p_config)
{
    uint32_t err_code;
    // Set Channel Number.
    err_code = sd_ant_channel_assign(p_config->channel_number, 
                                     p_config->channel_type, 
                                     p_config->network_number,
                                     p_config->ext_assign);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Set Channel ID.
    err_code = sd_ant_channel_id_set(p_config->channel_number, 
                                     p_config->device_number, 
                                     p_config->device_type, 
                                     p_config->transmission_type);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Set Channel RF frequency.
    err_code = sd_ant_channel_radio_freq_set(p_config->channel_number, p_config->rf_freq);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Set Channel period.
    if (!(p_config->ext_assign & EXT_PARAM_ALWAYS_SEARCH))
    {
        err_code = sd_ant_channel_period_set(p_config->channel_number, p_config->channel_period);
    }
    
    
#if ANT_CONFIG_ENCRYPTED_CHANNELS > 0
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    err_code = ant_channel_encrypt_config(p_config->channel_type , p_config->channel_number, p_config->p_crypto_settings);
#endif
    
    return err_code;
}
