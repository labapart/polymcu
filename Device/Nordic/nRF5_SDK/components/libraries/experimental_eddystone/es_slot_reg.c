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

#include "es_slot_reg.h"

#define ES_SLOT_NOT_CONFIGURED 0xab /** Value set in configured lists to indicate not configured slot. */

/**@brief Function updating 'tlm_configured' property of slot registry when slot is being cleared.
 *
 * @param[in] p_reg       Pointer to slot registry.
 * @param[in] slot_no     Slot number to be used.
 */
static void update_tlm_configured_on_clearing(es_slot_reg_t * p_reg, uint8_t slot_no)
{
    if (p_reg->tlm_configured && slot_no == p_reg->tlm_slot)
    {
        p_reg->tlm_configured = false;
    }
}


/**@brief Function updating 'num_configured_slots' and 'slots_configured' properties of slot registry when slot is being cleared.
 *
 * @param[in] p_configured              Pointer to list of configured slots.
 * @param[in] p_num_configured_slots    Pointer to number of configured slots.
 * @param[in] slot_no                   Slot number to clear.
 */
static void configured_slots_on_clear_update(uint8_t * p_configured, uint8_t * p_num_configured_slots, uint8_t slot_no)
{
    uint8_t index_of_last_configured_slot = *p_num_configured_slots - 1;
    
    for (uint8_t i = 0; i < APP_MAX_ADV_SLOTS; ++i)
    {
        if (p_configured[i] == slot_no)
        {
            // Copy all values 'to the right' of the cleared slot one step to the left.
            if(i < index_of_last_configured_slot)
            {
                memcpy(&p_configured[i], 
                       &p_configured[i + 1], 
                       index_of_last_configured_slot - i);
                
                // Write ES_SLOT_NOT_CONFIGURED to all rightmost not configured indexes.
                memset(&p_configured[index_of_last_configured_slot], 
                       ES_SLOT_NOT_CONFIGURED, 
                       APP_MAX_ADV_SLOTS - index_of_last_configured_slot);
            }
            
            else
            {
                // There are no values 'to the right', simply overwrite with ES_SLOT_NOT_CONFIGURED
                p_configured[i] = ES_SLOT_NOT_CONFIGURED;
            }

            *p_num_configured_slots -= 1;
            
            return;
        }
    }
    
    
}


bool es_slot_reg_etlm_required(const es_slot_reg_t * p_reg)
{
    return (p_reg->num_configured_eid_slots > 0 && p_reg->tlm_configured);
}


bool es_slot_reg_clear_slot(es_slot_reg_t * p_reg, uint8_t slot_no)
{
    bool eid_has_been_cleared = false;
    
    if (p_reg->slots[slot_no].configured)
    {
        update_tlm_configured_on_clearing(p_reg, slot_no);

        configured_slots_on_clear_update(p_reg->slots_configured, 
                                         &p_reg->num_configured_slots, 
                                         slot_no);

        if (p_reg->slots[slot_no].adv_frame.type == ES_FRAME_TYPE_EID)
        {
            configured_slots_on_clear_update(p_reg->eid_slots_configured, 
                                             &p_reg->num_configured_eid_slots, 
                                             slot_no);

            eid_has_been_cleared = true;
        }
        
        p_reg->slots[slot_no].configured = false;
    }

    memset(&p_reg->slots[slot_no], 0, sizeof(p_reg->slots[slot_no]));
    
    return eid_has_been_cleared;
}


void es_slot_reg_update_slot_list_info_on_add(es_slot_reg_t * p_reg, uint8_t slot_no, es_frame_type_t frame_type, bool init)
{
    if (frame_type == ES_FRAME_TYPE_TLM)
    {
        p_reg->tlm_configured = true;
        p_reg->tlm_slot       = slot_no;
    }

    if (!p_reg->slots[slot_no].configured || init)
    {
        p_reg->slots[slot_no].configured = true;

        // Note, we use 'num_configured_slots' before incrementing it, so it is pointing to the correct index.
        p_reg->slots_configured[p_reg->num_configured_slots] = slot_no;

        p_reg->num_configured_slots++;

        if (frame_type == ES_FRAME_TYPE_EID)
        {
            p_reg->eid_slots_configured[p_reg->num_configured_eid_slots] = slot_no;

            p_reg->num_configured_eid_slots++;
        }
    }

    // If an already configured slot has changed from anything TO an EID slot.
    else if (frame_type == ES_FRAME_TYPE_EID && \
             p_reg->slots[slot_no].adv_frame.type != ES_FRAME_TYPE_EID)
    {
        p_reg->eid_slots_configured[p_reg->num_configured_eid_slots] = slot_no;

        p_reg->num_configured_eid_slots++;
    }
}


void es_slot_reg_init(es_slot_reg_t * p_reg)
{
    p_reg->tlm_configured   = false;
    memset(p_reg->slots_configured, ES_SLOT_NOT_CONFIGURED, sizeof(p_reg->slots_configured));
    memset(p_reg->eid_slots_configured, ES_SLOT_NOT_CONFIGURED, sizeof(p_reg->eid_slots_configured));
    p_reg->num_configured_eid_slots = 0;
    p_reg->num_configured_slots     = 0;

}
