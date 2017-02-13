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


/**@file
 *
 * @defgroup nrf_block_dev_sdc SDC implementation
 * @ingroup nrf_block_dev
 * @{
 *
 */


#ifndef NRF_BLOCK_DEV_SDC_H__
#define NRF_BLOCK_DEV_SDC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_block_dev.h"
#include "app_sdcard.h"

/**
 * @brief SDC block device operations
 * */
extern const nrf_block_dev_ops_t nrf_block_device_sdc_ops;

/**
 * @brief Work structure of SDC block device
 */
typedef struct {
    nrf_block_dev_geometry_t geometry;      //!< Block device geometry
    nrf_block_dev_ev_handler ev_handler;    //!< Block device event handler
    nrf_block_req_t          req;           //!< Block READ/WRITE request
    void const *             p_context;     //!< Context handle passed to event handler
} nrf_block_dev_sdc_work_t;

/**
 * @brief SDC block device config initializer (@ref nrf_block_dev_sdc_config_t)
 *
 * @param blk_size          Block size
 * @param sdc_lib_config    SDC library config (@ref app_sdc_config_t)
 * */
#define NRF_BLOCK_DEV_SDC_CONFIG(blk_size, sdc_lib_config)  \
{                                                           \
        .block_size = (blk_size),                           \
        .sdc_config = sdc_lib_config                        \
}


/**
 * @brief SDC block device config
 */
typedef struct {
    uint32_t block_size;              //!< Desired block size
    app_sdc_config_t    sdc_config;   //!< SDC library configuration
} nrf_block_dev_sdc_config_t;

/**
 * @brief SDC block device
 * */
typedef struct {
    nrf_block_dev_t              block_dev;         //!< Block device
    nrf_block_dev_info_strings_t info_strings;      //!< Block device information strings
    nrf_block_dev_sdc_config_t   sdc_bdev_config;   //!< SDC block device config
    nrf_block_dev_sdc_work_t *   p_work;            //!< SDC block device work structure
} nrf_block_dev_sdc_t;

/**
 * @brief Defines a SDC block device.
 *
 * @param name    Instance name
 * @param config  Configuration @ref nrf_block_dev_sdc_config_t
 * @param info    Info strings @ref NFR_BLOCK_DEV_INFO_CONFIG
 * */
#define NRF_BLOCK_DEV_SDC_DEFINE(name, config, info)                 \
    static nrf_block_dev_sdc_work_t CONCAT_2(name, _work);           \
    static const nrf_block_dev_sdc_t name = {                        \
            .block_dev = { .p_ops = &nrf_block_device_sdc_ops },     \
            .info_strings = BRACKET_EXTRACT(info),                   \
            .sdc_bdev_config = config,                               \
            .p_work = &CONCAT_2(name, _work),                        \
    }

/**
 * @brief Returns block device API handle from SDC block device.
 *
 * @param[in] p_blk_sdc SDC block device
 * @return Block device handle
 */
static inline nrf_block_dev_t const *
nrf_block_dev_sdc_ops_get(nrf_block_dev_sdc_t const * p_blk_sdc)
{
    return &p_blk_sdc->block_dev;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_BLOCK_DEV_SDC_H__ */
