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

#ifndef ES_ADV_TIMING_RESOLVER_H__
#define ES_ADV_TIMING_RESOLVER_H__

#include "sdk_errors.h"
#include "es_app_config.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @file
 * @addtogroup eddystone_adv_timing
 * @{
 */

/** @brief Timing parameters for a single slot. */
typedef struct
{
    bool    is_etlm;   //!< Flag that specifies if the slot is an eTLM.
    uint8_t slot_no;   /**< @brief Slot number. @details
                        * For non-eTLM slots: The slot number of the given frame.
                        *
                        * For eTLM slots: The slot number of the corresponding EID frame. */
    uint16_t delay_ms; //!< Delay from this frame to the next.
} es_adv_timing_resolver_adv_timing_t;

/**@brief Results of calculating advertisement delays. */
typedef struct
{
    es_adv_timing_resolver_adv_timing_t timing_results[APP_MAX_ADV_SLOTS - APP_MAX_EID_SLOTS +
                                                       (APP_MAX_EID_SLOTS * 2)]; //!< List of timing results.
    uint8_t len_timing_results;                                                  //!< Length of results.
} es_adv_timing_resolver_result_t;

/**@brief Input to the timing resolver. */
typedef struct
{
    uint16_t                          adv_interval;             //!< Global advertisement interval.
    uint8_t                           num_slots_configured;     //!< Number of configured slots.
    const uint8_t                   * p_slots_configured;       //!< Pointer to the list of configured slots.
    uint8_t                           num_eid_slots_configured; //!< Number of configured EID slots.
    const uint8_t                   * p_eid_slots_configured;   //!< Pointer to the list of configured EID slots.
    bool                              tlm_configured;           //!< Flag that specifies if TLM slot is configured.
    uint8_t                           tlm_slot;                 //!< Slot number of the TLM slot (if @p tlm_configured is true).
    es_adv_timing_resolver_result_t * p_result;                 //!< Output result.
} es_adv_timing_resolver_input_t;


/**@brief Function for getting the input for advertisement interval calculation.
 *
 * @param[in,out]    p_input Input to advertisement interval calculation (see @ref es_adv_timing_resolver_input_t).
 * @retval           NRF_SUCCESS If the operation was successful. Otherwise, an error code is returned.
 */
ret_code_t es_adv_timing_resolve(es_adv_timing_resolver_input_t * p_input);

/**
 * @}
 */

#endif // ES_ADV_TIMING_RESOLVER_H__
