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

#include "ant_sdm_page_3.h"
#include "ant_sdm_utils.h"

#define NRF_LOG_MODULE_NAME "ANT_SDM_PAGE_3"
#if ANT_SDM_PAGE_3_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_SDM_PAGE_3_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_SDM_PAGE_3_INFO_COLOR
#else // ANT_SDM_PAGE_3_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_SDM_PAGE_3_LOG_ENABLED
#include "nrf_log.h"


/**@brief SDM page 3 data layout structure. */
typedef struct
{
    uint8_t reserved0[5];
    uint8_t calories;
    uint8_t reserved1;
}ant_sdm_page3_data_layout_t;

static void page_3_data_log(ant_sdm_page3_data_t const * p_page_data)
{
    NRF_LOG_INFO("Calories:                         %u\r\n\n", p_page_data->calories);
}


void ant_sdm_page_3_encode(uint8_t                    * p_page_buffer,
                           ant_sdm_page3_data_t const * p_page_data)
{
    ant_sdm_page3_data_layout_t * p_outcoming_data = (ant_sdm_page3_data_layout_t *)p_page_buffer;

    p_outcoming_data->calories = p_page_data->calories;
    page_3_data_log(p_page_data);
}


void ant_sdm_page_3_decode(uint8_t const        * p_page_buffer,
                           ant_sdm_page3_data_t * p_page_data)
{
    ant_sdm_page3_data_layout_t const * p_incoming_data =
        (ant_sdm_page3_data_layout_t *)p_page_buffer;

    uint8_t prev_calories = (uint8_t) p_page_data->calories;

    p_page_data->calories += ((p_incoming_data->calories - prev_calories) & UINT8_MAX);

    page_3_data_log(p_page_data);
}

#endif // NRF_MODULE_ENABLED(ANT_SDM)
