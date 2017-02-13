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

#include "sdk_common.h"
#if NRF_MODULE_ENABLED(ANT_SDM)

#include "ant_sdm_page_16.h"
#include "ant_sdm_utils.h"

#define NRF_LOG_MODULE_NAME "ANT_SDM_PAGE_16"
#if ANT_SDM_PAGE_16_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_SDM_PAGE_16_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_SDM_PAGE_16_INFO_COLOR
#else // ANT_SDM_PAGE_16_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_SDM_PAGE_16_LOG_ENABLED
#include "nrf_log.h"


/**@brief SDM page 16 data layout structure. */
typedef struct
{
    uint8_t strides[3];
    uint8_t distance[4];
}ant_sdm_page16_data_layout_t;

STATIC_ASSERT(ANT_SDM_DISTANCE_DISP_PRECISION == 10); ///< Display format need to be updated

/**@brief Function for tracing common data.
 *
 * @param[in]  p_common_data      Pointer to the common data.
 */
static void page_16_data_log(ant_sdm_common_data_t const * p_common_data)
{
    uint64_t distance = ANT_SDM_DISTANCE_RESCALE(p_common_data->distance);

    NRF_LOG_INFO("Distance                             %u.%01u m\r\n",
                 (unsigned int)(distance / ANT_SDM_DISTANCE_DISP_PRECISION),
                 (unsigned int)(distance % ANT_SDM_DISTANCE_DISP_PRECISION));
    NRF_LOG_INFO("Strides                              %u\r\n\n",
                 (unsigned int)p_common_data->strides);
}


void ant_sdm_page_16_encode(uint8_t                     * p_page_buffer,
                            ant_sdm_common_data_t const * p_common_data)
{
    ant_sdm_page16_data_layout_t * p_outcoming_data = (ant_sdm_page16_data_layout_t *)p_page_buffer;

    UNUSED_PARAMETER(uint24_encode(p_common_data->strides, p_outcoming_data->strides));
    UNUSED_PARAMETER(uint32_encode(p_common_data->distance << 4, p_outcoming_data->distance));

    page_16_data_log(p_common_data);
}


void ant_sdm_page_16_decode(uint8_t const         * p_page_buffer,
                            ant_sdm_common_data_t * p_common_data)
{
    ant_sdm_page16_data_layout_t const * p_incoming_data =
        (ant_sdm_page16_data_layout_t *)p_page_buffer;

    p_common_data->strides  = uint24_decode(p_incoming_data->strides);
    p_common_data->distance = uint32_decode(p_incoming_data->distance) >> 4;

    page_16_data_log(p_common_data);
}

#endif // NRF_MODULE_ENABLED(ANT_SDM)
