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

#ifndef NRF_BLOCK_DEV_EMPTY_H__
#define NRF_BLOCK_DEV_EMPTY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_block_dev.h"


/**@file
 *
 * @defgroup nrf_block_dev_empty Empty implementation
 * @ingroup nrf_block_dev
 *
 * This module implements block device API. It works like:
 *           - /dev/empty for write operations
 *           - /dev/zero for read operations
 * @{
 *
 */

/**
 * @brief EMPTY block device operations
 * */
extern const nrf_block_dev_ops_t nrf_block_device_empty_ops;

/**
 * @brief Work structure of EMPTY block device.
 */
typedef struct {
    nrf_block_dev_geometry_t geometry;      //!< Block device geometry
    nrf_block_dev_ev_handler ev_handler;    //!< Block device event handler
    void const *             p_context;     //!< Context handle passed to event handler
} nrf_block_dev_empty_work_t;


/**
 * @brief EMPTY block device config initializer (@ref nrf_block_dev_empty_config_t)
 *
 * @param blk_size   Block size
 * @param blk_count  Block count
 * */
#define NRF_BLOCK_DEV_EMPTY_CONFIG(blk_size, blk_count)  {  \
        .block_size = (blk_size),                           \
        .block_count = (blk_count)                          \
}

/**
 * @brief EMPTY block device config
 */
typedef struct {
    uint32_t block_size;        //!< Desired block size
    uint32_t block_count;       //!< Desired block count
} nrf_block_dev_empty_config_t;

/**
 * @brief EMPTY block device
 * */
typedef struct {
    nrf_block_dev_t              block_dev;       //!< Block device
    nrf_block_dev_info_strings_t info_strings;    //!< Block device information strings
    nrf_block_dev_empty_config_t empty_config;    //!< EMPTY block device config
    nrf_block_dev_empty_work_t * p_work;          //!< EMPTY block device work structure
} nrf_block_dev_empty_t;

/**
 * @brief Defines a EMPTY block device.
 *
 * @param name          Instance name
 * @param config        Configuration @ref nrf_block_dev_empty_config_t
 * @param info          Info strings @ref NFR_BLOCK_DEV_INFO_CONFIG
 * */
#define NRF_BLOCK_DEV_EMPTY_DEFINE(name, config, info)                  \
    static nrf_block_dev_empty_work_t CONCAT_2(name, _work);            \
    static const nrf_block_dev_empty_t name = {                         \
        .block_dev = { .p_ops = &nrf_block_device_empty_ops },          \
        .info_strings = BRACKET_EXTRACT(info),                          \
        .empty_config = config,                                         \
        .p_work = &CONCAT_2(name, _work),                               \
    }

/**
 * @brief Returns block device API handle from EMPTY block device.
 *
 * @param[in] p_blk_empty EMPTY block device
 * @return Block device handle
 */
static inline nrf_block_dev_t const *
nrf_block_dev_empty_ops_get(nrf_block_dev_empty_t const * p_blk_empty)
{
    return &p_blk_empty->block_dev;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_BLOCK_DEV_EMPTY_H__ */
