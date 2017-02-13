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

#include "es_adv_timing_resolver.h"
#include "es_app_config.h"

/**@brief Function for finding delay to use after each non-eTLM advertisement.
 *
 * @param[in] adv_interval          Configured advertisement interval.
 * @param[in] num_slots_configured  Number of configured slots.
 * @param[in] eTLM_required         Is there an eTLM slot.
 */
static uint16_t get_adv_delay(uint16_t adv_interval,
                              uint8_t  num_slots_configured,
                              bool     eTLM_required)
{
    // If eTLM is required, don't count this when calculating delay.
    return adv_interval / (num_slots_configured - (eTLM_required ? 1 : 0));
}


ret_code_t es_adv_timing_resolve(es_adv_timing_resolver_input_t * p_input)
{
    uint8_t  result_index  = 0;
    bool     eTLM_required = p_input->tlm_configured && p_input->num_eid_slots_configured > 0;
    uint16_t base_delay;

    if (p_input->num_slots_configured == 0)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    base_delay = get_adv_delay(p_input->adv_interval,
                               p_input->num_slots_configured,
                               eTLM_required);

    for (uint8_t i = 0; i < p_input->num_slots_configured; ++i)
    {
        uint8_t slot_no = p_input->p_slots_configured[i];

        // Add all slots, except the TLM slot in the case where eTLM is required.
        if (!(eTLM_required && (slot_no == p_input->tlm_slot)))
        {
            p_input->p_result->timing_results[result_index].slot_no = slot_no;
            p_input->p_result->timing_results[result_index].is_etlm = false;

            if (result_index > 0)
            {
                p_input->p_result->timing_results[result_index - 1].delay_ms = base_delay;
            }

            result_index++;
        }
    }

    if (eTLM_required)
    {
        for (uint8_t i = 0; i < p_input->num_eid_slots_configured; ++i)
        {
            uint8_t slot_no = p_input->p_eid_slots_configured[i];

            p_input->p_result->timing_results[result_index].slot_no = slot_no;
            p_input->p_result->timing_results[result_index].is_etlm = true;

            if (result_index > 0)
            {
                p_input->p_result->timing_results[result_index - 1].delay_ms = APP_ETLM_DELAY_MS;
            }

            result_index++;
        }
    }

    p_input->p_result->len_timing_results                        = result_index; // Note: index has been increased to equal length of result.
    p_input->p_result->timing_results[result_index - 1].delay_ms = 0;            // Last Slot does not need delay.

    return NRF_SUCCESS;
}
