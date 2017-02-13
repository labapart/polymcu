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

#ifndef ES_ADV_TIMING_H__
#define ES_ADV_TIMING_H__

#include <stdint.h>

/**
 * @file
 * @defgroup eddystone_adv_timing Timing
 * @brief Events and functions for advertisement timing.
 * @ingroup eddystone_adv
 * @{
 */

/**@brief Eddystone advertisement timing event types. */
typedef enum
{
    ES_ADV_TIMING_EVT_ADV_SLOT, //!< Advertising non-eTLM slot.
    ES_ADV_TIMING_EVT_ADV_ETLM  //!< Advertising eTLM slot.
} es_adv_timing_evt_id_t;

/**@brief Eddystone advertisement timing event. */
typedef struct
{
    es_adv_timing_evt_id_t evt_id;  //!< Event type ID.
    uint8_t                slot_no; /**< @brief Slot number.
                                     * @details For non-eTLM events: The slot number to advertise.
                                     *
                                     * For eTLM events: The slot number of the corresponding EID slot. */
} es_adv_timing_evt_t;

/**@brief Eddystone advertisement timing event callback.
 *
 * @param[in]   p_evt       Pointer to the Eddystone advertisement timing event.
 */
typedef void (*es_adv_timing_callback_t)(const es_adv_timing_evt_t * p_evt);

/**@brief Function for starting Eddystone advertisement timing event generation. */
void es_adv_timing_start(uint16_t adv_interval);


/**@brief Function for stopping Eddystone advertisement timing event generation. */
void es_adv_timing_stop(void);


/**@brief Function for initializing the Eddystone advertisement timing module.
 *
 * @param[in]   handler     Eddystone advertisement timing event handler to register.
 */
void es_adv_timing_init(es_adv_timing_callback_t handler);

/**
 * @}
 */

#endif // ES_ADV_TIMING_H__
