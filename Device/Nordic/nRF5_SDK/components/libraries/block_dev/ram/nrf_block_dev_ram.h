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

#ifndef NRF_BLOCK_DEV_RAM_H__
#define NRF_BLOCK_DEV_RAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_block_dev.h"

/**@file
 *
 * @defgroup nrf_block_dev_ram RAM implementation
 * @ingroup nrf_block_dev
 * @{

 *
 * @brief This module implements block device API. It should be used as a reference block device.
 */

/**
 * @brief RAM block device operations
 * */
extern const nrf_block_dev_ops_t nrf_block_device_ram_ops;

/**
 * @brief Work structure of RAM block device
 */
typedef struct {
    nrf_block_dev_geometry_t geometry;      //!< Block device geometry
    nrf_block_dev_ev_handler ev_handler;    //!< Block device event handler
    void const *             p_context;     //!< Context handle passed to event handler
} nrf_block_dev_ram_work_t;


/**
 * @brief RAM block device config initializer (@ref nrf_block_dev_ram_config_t)
 *
 * @param blk_size      Block size
 * @param buffer        RAM work buffer
 * @param buffer_size   RAM work buffer size
 * */
#define NRF_BLOCK_DEV_RAM_CONFIG(blk_size, buffer, buffer_size)  {      \
        .block_size = (blk_size),                                       \
        .p_work_buffer = (buffer),                                      \
        .size = (buffer_size),                                          \
}

/**
 * @brief Ram block device config
 */
typedef struct {
    uint32_t block_size;        //!< Desired block size
    void *   p_work_buffer;     //!< Ram work buffer
    size_t   size;              //!< Ram work buffer size
} nrf_block_dev_ram_config_t;

/**
 * @brief Ram block device
 * */
typedef struct {
    nrf_block_dev_t              block_dev;     //!< Block device
    nrf_block_dev_info_strings_t info_strings;  //!< Block device information strings
    nrf_block_dev_ram_config_t   ram_config;    //!< Ram block device config
    nrf_block_dev_ram_work_t *   p_work;        //!< Ram block device work structure
} nrf_block_dev_ram_t;

/**
 * @brief Defines a RAM block device.
 *
 * @param name          Instance name
 * @param config        Configuration @ref nrf_block_dev_ram_config_t
 * @param info          Info strings @ref NFR_BLOCK_DEV_INFO_CONFIG
 * */
#define NRF_BLOCK_DEV_RAM_DEFINE(name, config, info)                \
    static nrf_block_dev_ram_work_t CONCAT_2(name, _work);          \
    static const nrf_block_dev_ram_t name = {                       \
        .block_dev = { .p_ops = &nrf_block_device_ram_ops },        \
        .info_strings = BRACKET_EXTRACT(info),                      \
        .ram_config = config,                                       \
        .p_work = &CONCAT_2(name, _work),                           \
    }

/**
 * @brief Returns block device API handle from RAM block device.
 *
 * @param[in] p_blk_ram Ram block device
 * @return Block device handle
 */
static inline nrf_block_dev_t const *
nrf_block_dev_ram_ops_get(nrf_block_dev_ram_t const * p_blk_ram)
{
    return &p_blk_ram->block_dev;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_BLOCK_DEV_RAM_H__ */
