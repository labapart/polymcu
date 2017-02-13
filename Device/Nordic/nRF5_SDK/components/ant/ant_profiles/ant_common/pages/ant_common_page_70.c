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
#if NRF_MODULE_ENABLED(ANT_COMMON_PAGE_70)

#include <string.h>
#include "ant_common_page_70.h"

#define NRF_LOG_MODULE_NAME "ANT_COMMON_PAGE_70"
#if ANT_COMMON_PAGE_70_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_COMMON_PAGE_70_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_COMMON_PAGE_70_INFO_COLOR
#else // ANT_COMMON_PAGE_70_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_COMMON_PAGE_70_LOG_ENABLED
#include "nrf_log.h"

/**@brief ANT+ common page 70 data layout structure. */
typedef struct
{
    uint8_t reserved[2];
    uint8_t descriptor[2];
    uint8_t req_trans_response;
    uint8_t req_page_number;
    uint8_t command_type;
}ant_common_page70_data_layout_t;

/**@brief Function for tracing page 70 data.
 *
 * @param[in]  p_page_data      Pointer to the page 70 data.
 */
static void page70_data_log(volatile ant_common_page70_data_t const * p_page_data)
{
    NRF_LOG_INFO("Page %d request\r\n", p_page_data->page_number);

    switch (p_page_data->transmission_response.specyfic)
    {
        case ANT_PAGE70_RESPONSE_TRANSMIT_UNTIL_SUCCESS:
            NRF_LOG_INFO("Try to send until ACK\r\n");
            break;

        case ANT_PAGE70_RESPONSE_INVALID:
            NRF_LOG_INFO("Invalid requested transmission response\r\n");
            break;

        default:

            if (p_page_data->transmission_response.items.ack_resposne)
            {
                NRF_LOG_INFO("Answer with acknowledged messages\r\n");
            }
            NRF_LOG_INFO("Requested number of transmissions: %d\r\n",
                       p_page_data->transmission_response.items.transmit_count);
    }

    switch (p_page_data->command_type)
    {
        case ANT_PAGE70_COMMAND_PAGE_DATA_REQUEST:
            NRF_LOG_INFO("Request Data Page\r\n");
            break;

        case ANT_PAGE70_COMMAND_ANT_FS_SESSION_REQUEST:
            NRF_LOG_INFO("Request ANT-FS Session\r\n");
            break;

        default:
            NRF_LOG_INFO("Invalid request\r\n");
    }
    NRF_LOG_INFO("Descriptor %x\r\n\n", p_page_data->descriptor);
}


void ant_common_page_70_encode(uint8_t                                 * p_page_buffer,
                               volatile ant_common_page70_data_t const * p_page_data)
{
    ant_common_page70_data_layout_t * p_outcoming_data =
        (ant_common_page70_data_layout_t *)p_page_buffer;

    memset(p_outcoming_data->reserved, UINT8_MAX, sizeof (p_outcoming_data->reserved));
    UNUSED_PARAMETER(uint16_encode(p_page_data->descriptor, p_outcoming_data->descriptor));
    p_outcoming_data->req_trans_response = p_page_data->transmission_response.byte;
    p_outcoming_data->req_page_number    = p_page_data->page_number;
    p_outcoming_data->command_type       = p_page_data->command_type;

    page70_data_log(p_page_data);
}


void ant_common_page_70_decode(uint8_t const                     * p_page_buffer,
                               volatile ant_common_page70_data_t * p_page_data)
{
    ant_common_page70_data_layout_t const * p_incoming_data =
        (ant_common_page70_data_layout_t *)p_page_buffer;

    p_page_data->descriptor                 = uint16_decode(p_incoming_data->descriptor);
    p_page_data->transmission_response.byte = p_incoming_data->req_trans_response;
    p_page_data->page_number                = p_incoming_data->req_page_number;
    p_page_data->command_type               = (ant_page70_command_t)p_incoming_data->command_type;

    page70_data_log(p_page_data);
}

#endif // NRF_MODULE_ENABLED(ANT_COMMON_PAGE_70)
