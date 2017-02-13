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

#ifndef es_slot_list_H__
#define es_slot_list_H__

#include <stdint.h>
#include "nrf_ble_escs.h"
#include "es_slot.h"
#include "es.h"

/**
 * @file
 * @addtogroup eddystone_slot
 * @{
 */

/** @brief Function for checking if an eTLM frame is required. 
 *
 * @param[in]   p_reg   Pointer to the slot registry.
 *
 * @retval      true    If an eTLM frame is required.
 * @retval      false   Otherwise.
 */
bool es_slot_reg_etlm_required(const es_slot_reg_t * p_reg);

/** @brief Function for clearing a slot.
 *
 * @param[in]   p_reg       Pointer to the slot registry.
 * @param[in]   slot_no     The slot number to clear.
 *
 * @retval      true        If an EID slot was cleared.
 */
bool es_slot_reg_clear_slot(es_slot_reg_t * p_reg, uint8_t slot_no);

/** @brief Function for updating the state of the slot registry after adding a slot.
 *
 * @param[in]   p_reg       Pointer to the slot registry.
 * @param[in]   slot_no     The slot number that was added.
 * @param[in]   frame_type  The frame type that was added.
 * @param[in]   init        Information if the data is loaded during initialization. Set this parameter to false if the call is a result of a write to the Eddystone Configuration Service.
 */
void es_slot_reg_update_slot_list_info_on_add(es_slot_reg_t * p_reg, uint8_t slot_no, es_frame_type_t frame_type, bool init);

/** @brief Function for initializing the slot registry.
 *
 * @param[in]   p_reg       Pointer to the slot registry to initialize.
 */
void es_slot_reg_init(es_slot_reg_t * p_reg);

/**
 * @}
 */

#endif // es_slot_list_H__
