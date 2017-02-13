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

#include "nrf_drv_adc.h"
#include "app_error.h"
#include "es_battery_voltage.h"

static uint16_t m_batt_lvl_in_milli_volts;

#define ADC_REF_VOLTAGE_IN_MILLIVOLTS     600  // /< Reference voltage (in milli volts) used by ADC while doing conversion.
#define DIODE_FWD_VOLT_DROP_MILLIVOLTS    270  // /< Typical forward voltage drop of the diode (Part no: SD103ATW-7-F) that is connected in series with the voltage supply. This is the voltage drop when the forward current is 1mA. Source: Data sheet of 'SURFACE MOUNT SCHOTTKY BARRIER DIODE ARRAY' available at www.diodes.com.
#define ADC_RES_10BIT                     1024 // /< Maximum digital value for 10-bit ADC conversion.
#define ADC_PRE_SCALING_COMPENSATION      6    // /< The ADC is configured to use VDD with 1/3 prescaling as input. And hence the result of conversion is to be multiplied by 3 to get the actual value of the battery voltage.
#define ADC_REF_VBG_VOLTAGE_IN_MILLIVOLTS 1200 // /< Value in millivolts for voltage used as reference in ADC conversion on NRF51.
#define ADC_INPUT_PRESCALER               3    // /< Input prescaler for ADC convestion on NRF51.

static nrf_adc_value_t adc_buf[1];
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE) \
    ((((ADC_VALUE) *ADC_REF_VBG_VOLTAGE_IN_MILLIVOLTS) / ADC_RES_10BIT) * ADC_INPUT_PRESCALER)

static void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_ADC_EVT_DONE)
    {
        nrf_adc_value_t adc_result;
        uint32_t        err_code;

        adc_result = p_event->data.done.p_buffer[0];

        err_code = nrf_drv_adc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);

        m_batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(adc_result) +
                                    DIODE_FWD_VOLT_DROP_MILLIVOLTS;
    }
}


void es_battery_voltage_init(void)
{
    ret_code_t err_code = nrf_drv_adc_init(NULL, adc_event_handler);

    APP_ERROR_CHECK(err_code);

    static nrf_drv_adc_channel_t channel = NRF_DRV_ADC_DEFAULT_CHANNEL(
        NRF_ADC_CONFIG_INPUT_DISABLED);
    channel.config.config.input = (uint32_t)NRF_ADC_CONFIG_SCALING_SUPPLY_ONE_THIRD;
    nrf_drv_adc_channel_enable(&channel);

    err_code = nrf_drv_adc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);
    nrf_drv_adc_sample();
}


void es_battery_voltage_get(uint16_t * p_vbatt)
{
    *p_vbatt = m_batt_lvl_in_milli_volts;
    if (!nrf_adc_is_busy())
    {
        nrf_drv_adc_sample();
    }
}


