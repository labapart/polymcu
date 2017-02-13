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
#if NRF_MODULE_ENABLED(ANT_BPWR)

#include <string.h>
#include "ant_bpwr_page_1.h"

#define NRF_LOG_MODULE_NAME "ANT_BPWR_PAGE_1"
#if ANT_BPWR_PAGE_1_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_BPWR_PAGE_1_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_BPWR_PAGE_1_INFO_COLOR
#else // ANT_BPWR_PAGE_1_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_BPWR_PAGE_1_LOG_ENABLED
#include "nrf_log.h"

/**@brief bicycle power page 1 data layout structure. */
typedef struct
{
    uint8_t calibration_id; ///< Calibration request type
    union
    {
        struct
        {
            uint8_t reserved[6]; ///< Unused, fill by 0xFF.
        } general_calib_request;
        struct
        {
            uint8_t auto_zero_status; ///< Status of automatic zero feature of power sensor.
            uint8_t reserved[5];      ///< Unused, fill by 0xFF.
        } auto_zero_config;
        struct
        {
            uint8_t auto_zero_status; ///< Status of automatic zero feature of power sensor.
            uint8_t reserved[3];      ///< Unused, fill by 0xFF.
            uint8_t data[2];          ///< Calibration Data.
        } general_calib_response;
        struct
        {
            uint8_t enable      : 1;
            uint8_t status      : 1;
            uint8_t reserved0   : 6; ///< Unused, fill by 0x00.
            uint8_t reserved1[5];    ///< Unused, fill by 0xFF.
        } auto_zero_support;
        struct
        {
            uint8_t manufac_spec[6]; ///< Manufacture Specyfic Data.
        } custom_calib;
    } data;
} ant_bpwr_page1_data_layout_t;


static void page1_data_log(ant_bpwr_page1_data_t const * p_page_data)
{
    NRF_LOG_INFO("Calibration id:                      %u\r\n", p_page_data->calibration_id);

    switch (p_page_data->calibration_id)
    {
        case ANT_BPWR_CALIB_ID_MANUAL:
            // No implementation needed
            break;

        case ANT_BPWR_CALIB_ID_MANUAL_SUCCESS:
        /* fall through */
        case ANT_BPWR_CALIB_ID_FAILED:
            NRF_LOG_INFO("General calibration data:            %u\r\n",
                         p_page_data->data.general_calib);
        /* fall through */
        case ANT_BPWR_CALIB_ID_AUTO:
        /* fall through */
        case ANT_BPWR_CALIB_ID_AUTO_SUPPORT:

            switch (p_page_data->auto_zero_status)
            {
                case ANT_BPWR_AUTO_ZERO_NOT_SUPPORTED:
                    NRF_LOG_INFO("Auto zero not supported\r\n\n");
                    break;

                case ANT_BPWR_AUTO_ZERO_OFF:
                    NRF_LOG_INFO("Auto zero off\r\n\n");
                    break;

                case ANT_BPWR_AUTO_ZERO_ON:
                    NRF_LOG_INFO("Auto zero on\r\n\n");
                    break;
            }
            break;

        case ANT_BPWR_CALIB_ID_CTF:
            NRF_LOG_INFO("Not supported\r\n\n");
            break;

        case ANT_BPWR_CALIB_ID_CUSTOM_REQ:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_REQ_SUCCESS:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE_SUCCESS:
            NRF_LOG_INFO("Manufacture specyfic:            ");
            NRF_LOG_HEXDUMP_INFO((uint8_t*)p_page_data->data.custom_calib,
                               sizeof (p_page_data->data.custom_calib));
            break;

        default: // shouldn't occur
            NRF_LOG_INFO("Unsupported calibration ID\r\n\n");
            break;
    }
}


void ant_bpwr_page_1_encode(uint8_t                     * p_page_buffer,
                            ant_bpwr_page1_data_t const * p_page_data)
{
    ant_bpwr_page1_data_layout_t * p_outcoming_data = (ant_bpwr_page1_data_layout_t *)p_page_buffer;

    page1_data_log(p_page_data);

    p_outcoming_data->calibration_id = p_page_data->calibration_id;

    switch (p_page_data->calibration_id)
    {
        case ANT_BPWR_CALIB_ID_MANUAL:
            memset(p_outcoming_data->data.general_calib_request.reserved, 0xFF,
                   sizeof (p_outcoming_data->data.general_calib_request.reserved));
            break;

        case ANT_BPWR_CALIB_ID_AUTO:
            memset(p_outcoming_data->data.auto_zero_config.reserved, 0xFF,
                   sizeof (p_outcoming_data->data.auto_zero_config.reserved));
            p_outcoming_data->data.auto_zero_config.auto_zero_status =
                p_page_data->auto_zero_status;
            break;

        case ANT_BPWR_CALIB_ID_MANUAL_SUCCESS:
        /* fall through */
        case ANT_BPWR_CALIB_ID_FAILED:
            memset(p_outcoming_data->data.general_calib_response.reserved, 0xFF,
                   sizeof (p_outcoming_data->data.general_calib_response.reserved));
            p_outcoming_data->data.general_calib_response.auto_zero_status =
                p_page_data->auto_zero_status;
            UNUSED_PARAMETER(uint16_encode(p_page_data->data.general_calib,
                                           p_outcoming_data->data.general_calib_response.data));
            break;

        case ANT_BPWR_CALIB_ID_CTF:
            NRF_LOG_INFO("Not supported\r\n");
            break;

        case ANT_BPWR_CALIB_ID_AUTO_SUPPORT:
            memset(p_outcoming_data->data.auto_zero_support.reserved1, 0xFF,
                   sizeof (p_outcoming_data->data.auto_zero_support.reserved1));
            p_outcoming_data->data.auto_zero_support.reserved0 = 0x00;
            p_outcoming_data->data.auto_zero_support.enable    =
                (p_page_data->auto_zero_status == ANT_BPWR_AUTO_ZERO_NOT_SUPPORTED) ? false : true;
            p_outcoming_data->data.auto_zero_support.status =
                (p_page_data->auto_zero_status == ANT_BPWR_AUTO_ZERO_ON) ? true : false;
            break;

        case ANT_BPWR_CALIB_ID_CUSTOM_REQ:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_REQ_SUCCESS:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE_SUCCESS:
            memcpy(p_outcoming_data->data.custom_calib.manufac_spec,
                   (void *)p_page_data->data.custom_calib,
                   sizeof (p_page_data->data.custom_calib));
            break;

        default: // shouldn't occur
            break;
    }
}


void ant_bpwr_page_1_decode(uint8_t const         * p_page_buffer,
                            ant_bpwr_page1_data_t * p_page_data)
{
    ant_bpwr_page1_data_layout_t const * p_incoming_data =
        (ant_bpwr_page1_data_layout_t *)p_page_buffer;

    p_page_data->calibration_id = (ant_bpwr_calib_id_t)p_incoming_data->calibration_id;

    switch (p_incoming_data->calibration_id)
    {
        case ANT_BPWR_CALIB_ID_MANUAL:
            // No implementation needed
            break;

        case ANT_BPWR_CALIB_ID_AUTO:
            /* fall through */
            p_page_data->auto_zero_status =
                (ant_bpwr_auto_zero_status_t)p_incoming_data->data.auto_zero_config.auto_zero_status;
            break;

        case ANT_BPWR_CALIB_ID_MANUAL_SUCCESS:
        /* fall through */
        case ANT_BPWR_CALIB_ID_FAILED:
            p_page_data->auto_zero_status =
                (ant_bpwr_auto_zero_status_t)p_incoming_data->data.general_calib_response.
                auto_zero_status;
            p_page_data->data.general_calib = uint16_decode(
                p_incoming_data->data.general_calib_response.data);
            break;

        case ANT_BPWR_CALIB_ID_CTF:
            NRF_LOG_INFO("Not supported\r\n");
            break;

        case ANT_BPWR_CALIB_ID_AUTO_SUPPORT:

            if (p_incoming_data->data.auto_zero_support.enable == false)
            {
                p_page_data->auto_zero_status = ANT_BPWR_AUTO_ZERO_NOT_SUPPORTED;
            }
            else if (p_incoming_data->data.auto_zero_support.status)
            {
                p_page_data->auto_zero_status = ANT_BPWR_AUTO_ZERO_ON;
            }
            else
            {
                p_page_data->auto_zero_status = ANT_BPWR_AUTO_ZERO_OFF;
            }
            break;

        case ANT_BPWR_CALIB_ID_CUSTOM_REQ:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_REQ_SUCCESS:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE:
        /* fall through */
        case ANT_BPWR_CALIB_ID_CUSTOM_UPDATE_SUCCESS:
            memcpy((void *)p_page_data->data.custom_calib,
                   p_incoming_data->data.custom_calib.manufac_spec,
                   sizeof (p_page_data->data.custom_calib));
            break;

        default: // shouldn't occur
            break;
    }

    page1_data_log(p_page_data);
}

#endif // NRF_MODULE_ENABLED(ANT_BPWR)
