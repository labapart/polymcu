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

#ifndef MAX9850_H__
#define MAX9850_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "nrf_drv_twi.h"

/**
 * @brief Default MAX9850 TWI configuration
 *
 * @param scl_pin   SCL pin number
 * @param sda_pin   SDA pin number
 */
#define MAX9850_DEFAULT_TWI_CONFIG(scl_pin, sda_pin)     {      \
        .scl                = scl_pin,                          \
        .sda                = sda_pin,                          \
        .frequency          = NRF_TWI_FREQ_100K,                \
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,            \
        .clear_bus_init     = false,                            \
        .hold_bus_uninit    = false                             \
}

/**
 * @brief   Internal MAX9850 register map
 * */
typedef struct {
    uint8_t status_a;           //!< Status register A (R)
    uint8_t status_b;           //!< Status register B (R)
    uint8_t volume;             //!< Volume control (RW)
    uint8_t general_purpose;    //!< General purpose register (RW)
    uint8_t interrupt_enable;   //!< Interrupt enable (RW)
    uint8_t enable;             //!< Enable register (RW)
    uint8_t clock;              //!< Clock control (RW)
    uint8_t charge_pump;        //!< Charge pump (RW)
    uint8_t lrclk_msb;          //!< LRCLK MSB register (RW)
    uint8_t lrclk_lsb;          //!< LRCLK LSB register (RW)
    uint8_t digital_audio;      //!< Digital audio (RW)
} max9850_regmap_t;

/**
 * @brief MAX9850 register map after reset
 * */
#define MAX9850_DEFAULT_REGMAP()    {   \
        .status_a = 0,                  \
        .status_b = 0,                  \
        .volume = 0x0C,                 \
        .general_purpose = 0,           \
        .interrupt_enable = 0,          \
        .enable = 0,                    \
        .clock = 0,                     \
        .charge_pump = 0,               \
        .lrclk_msb = 0,                 \
        .lrclk_lsb = 0,                 \
        .digital_audio = 0,             \
}

/**
 * @brief   Helper macro for creating MAX9850 TWI address
 * */
#define MAX9850_TWI_ADDR(v) (0x10 + (v))


/**
 * @brief MAX9850 configuration
 * */
typedef struct {
    nrf_drv_twi_t        twi;       //!< TWI instance
    nrf_drv_twi_config_t twi_cfg;   //!< TWI configuration
    max9850_regmap_t     regmap;    //!< MAX9850 register map
    uint8_t              twi_addr;  //!< MAX9850 TWI address
} max9850_config_t;

/**
 * @brief Initializes MAX9850 IC
 *
 * @param p_max9850 MAX9850 configuration
 *
 * @return Standard error code
 * */
ret_code_t max9850_init(max9850_config_t const * p_max9850);


#ifdef __cplusplus
}
#endif

#endif /* MAX9850_H__ */
