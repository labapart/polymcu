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
#if NRF_MODULE_ENABLED(ANT_HRM)

#include "ant_hrm_page_0.h"
#include "ant_hrm_utils.h"

#define NRF_LOG_MODULE_NAME "ANT_HRM_PAGE_0"
#if ANT_HRM_PAGE_0_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_HRM_PAGE_0_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_HRM_PAGE_0_INFO_COLOR
#else // ANT_HRM_PAGE_0_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_HRM_PAGE_0_LOG_ENABLED
#include "nrf_log.h"

/**@brief HRM page 0 data layout structure. */
typedef struct
{
    uint8_t reserved[3];
    uint8_t heart_beat_evt_time_LSB;
    uint8_t heart_beat_evt_time_MSB;
    uint8_t heart_beat_count;
    uint8_t computed_heart_rate;
}ant_hrm_page0_data_layout_t;

/**@brief Function for tracing page 0 and common data.
 *
 * @param[in]  p_common_data    Pointer to the common data.
 * @param[in]  p_page_data      Pointer to the page 0 data.
 */
static void page0_data_log(ant_hrm_page0_data_t const * p_page_data)
{
    NRF_LOG_INFO("Heart beat count:                 %u\r\n", (unsigned int)p_page_data->beat_count);
    NRF_LOG_INFO("Computed heart rate:              %u\r\n",
                 (unsigned int) p_page_data->computed_heart_rate);
    NRF_LOG_INFO("Heart beat event time:            %u.%03us\r\n\n",
                 (unsigned int) ANT_HRM_BEAT_TIME_SEC(p_page_data->beat_time),
                 (unsigned int) ANT_HRM_BEAT_TIME_MSEC(p_page_data->beat_time));
}


void ant_hrm_page_0_encode(uint8_t                    * p_page_buffer,
                           ant_hrm_page0_data_t const * p_page_data)
{
    ant_hrm_page0_data_layout_t * p_outcoming_data = (ant_hrm_page0_data_layout_t *)p_page_buffer;
    uint32_t                      beat_time        = p_page_data->beat_time;

    p_outcoming_data->reserved[0]             = UINT8_MAX;
    p_outcoming_data->reserved[1]             = UINT8_MAX;
    p_outcoming_data->reserved[2]             = UINT8_MAX;
    p_outcoming_data->heart_beat_evt_time_LSB = (uint8_t)(beat_time & UINT8_MAX);
    p_outcoming_data->heart_beat_evt_time_MSB = (uint8_t)((beat_time >> 8) & UINT8_MAX);
    p_outcoming_data->heart_beat_count        = (uint8_t)p_page_data->beat_count;
    p_outcoming_data->computed_heart_rate     = (uint8_t)p_page_data->computed_heart_rate;

    page0_data_log(p_page_data);
}


void ant_hrm_page_0_decode(uint8_t const        * p_page_buffer,
                           ant_hrm_page0_data_t * p_page_data)
{
    ant_hrm_page0_data_layout_t const * p_incoming_data =
        (ant_hrm_page0_data_layout_t *)p_page_buffer;

    uint32_t beat_time = (uint32_t)((p_incoming_data->heart_beat_evt_time_MSB << 8)
                                    + p_incoming_data->heart_beat_evt_time_LSB);

    p_page_data->beat_count          = (uint32_t)p_incoming_data->heart_beat_count;
    p_page_data->computed_heart_rate = (uint32_t)p_incoming_data->computed_heart_rate;
    p_page_data->beat_time           = beat_time;

    page0_data_log(p_page_data);
}

#endif // NRF_MODULE_ENABLED(ANT_HRM)
