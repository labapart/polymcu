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

#ifndef ES_TLM_H__
#define ES_TLM_H__

#include "es.h"

/**
 * @file
 * @defgroup eddystone_tlm TLM
 * @brief Functions for the Eddystone telemetry (TLM) manager.
 * @ingroup eddystone_adv
 * @{
 */

/**@brief Function for initializing the TLM manager.
 *
 * @return See @ref app_timer_start for possible return values.
 */
void es_tlm_init(void);

/**@brief Function for getting the current TLM.
 *
 * @param[in] p_tlm_frame   Pointer to the TLM frame to which the frame is retrieved.
 */
void es_tlm_tlm_get(es_tlm_frame_t * p_tlm_frame);

/**@brief Function for incrementing the ADV_CNT field of the TLM frame.
 *
 * @details This function should be called every time a frame is advertised.
 *
 */
void es_tlm_adv_cnt_inc(void);

/**
 * @}
 */

#endif // ES_TLM_H__
