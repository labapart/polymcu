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
#if NRF_MODULE_ENABLED(ANT_COMMON_PAGE_81)

#include "ant_common_page_81.h"

#define NRF_LOG_MODULE_NAME "ANT_COMMON_PAGE_81"
#if ANT_COMMON_PAGE_81_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_COMMON_PAGE_81_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_COMMON_PAGE_81_INFO_COLOR
#else // ANT_COMMON_PAGE_81_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_COMMON_PAGE_81_LOG_ENABLED
#include "nrf_log.h"

/**@brief ant+ common page 81 data layout structure. */
typedef struct
{
    uint8_t reserved; ///< unused, fill by 0xFF
    uint8_t sw_revision_minor;
    uint8_t sw_revision_major;
    uint8_t serial_number[4];
}ant_common_page81_data_layout_t;


/**@brief Function for tracing page 80 data.
 *
 * @param[in]  p_page_data      Pointer to the page 80 data.
 */
static void page81_data_log(volatile ant_common_page81_data_t const * p_page_data)
{
    if (p_page_data->sw_revision_minor != UINT8_MAX)
    {
        NRF_LOG_INFO("sw revision:                      %u.%u\r\n",
                   ((ant_common_page81_data_t const *) p_page_data)->sw_revision_major,
                   ((ant_common_page81_data_t const *) p_page_data)->sw_revision_minor);
    }
    else
    {
        NRF_LOG_INFO("sw revision:                      %u\r\n", p_page_data->sw_revision_major);
    }

    NRF_LOG_INFO("serial number:                    %u\r\n\n", (unsigned int) p_page_data->serial_number);
}


void ant_common_page_81_encode(uint8_t                                 * p_page_buffer,
                               volatile ant_common_page81_data_t const * p_page_data)
{
    ant_common_page81_data_layout_t * p_outcoming_data =
        (ant_common_page81_data_layout_t *)p_page_buffer;

    p_outcoming_data->reserved = UINT8_MAX;

    p_outcoming_data->sw_revision_minor = p_page_data->sw_revision_minor;
    p_outcoming_data->sw_revision_major = p_page_data->sw_revision_major;

    UNUSED_PARAMETER(uint32_encode(p_page_data->serial_number, p_outcoming_data->serial_number));

    page81_data_log(p_page_data);
}


void ant_common_page_81_decode(uint8_t const                     * p_page_buffer,
                               volatile ant_common_page81_data_t * p_page_data)
{
    ant_common_page81_data_layout_t const * p_incoming_data =
        (ant_common_page81_data_layout_t *)p_page_buffer;

    p_page_data->sw_revision_minor = p_incoming_data->sw_revision_minor;
    p_page_data->sw_revision_major = p_incoming_data->sw_revision_major;

    p_page_data->serial_number = uint32_decode(p_incoming_data->serial_number);

    page81_data_log(p_page_data);
}

#endif // NRF_MODULE_ENABLED(ANT_COMMON_PAGE_81)
