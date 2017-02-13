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

#ifndef ESCS_DEFS_H__
#define ESCS_DEFS_H__

#include "es.h"

/*@file Contains definitions specific to the Eddystone Configuration Service */

#define ESCS_UID_READ_LENGTH          (ES_UID_LENGTH)
#define ESCS_UID_WRITE_LENGTH         (ES_UID_NAMESPACE_LENGTH + \
                                       ES_UID_INSTANCE_LENGTH + ES_FRAME_TYPE_LENGTH)

#define ESCS_TLM_READ_LENGTH          (ESCS_TLM_READ_LENGTH)
#define ESCS_TLM_WRITE_LENGTH         (ES_FRAME_TYPE_LENGTH)

#define ESCS_EID_READ_LENGTH            (14)
#define ESCS_EID_WRITE_ECDH_LENGTH      (34)
#define ESCS_EID_WRITE_PUB_KEY_INDEX    (1)
#define ESCS_EID_WRITE_ENC_ID_KEY_INDEX (1)
#define ESCS_EID_WRITE_IDK_LENGTH       (18)

#define ESCS_URL_WRITE_LENGTH           (19)

#ifdef NRF52
#define ESCS_NUM_OF_SUPPORTED_TX_POWER  (9)
/**@brief TX power levels, based on nRF52 specifications. */
#define ESCS_SUPPORTED_TX_POWER         {-40, -20, -16, -12,  -8,  -4,  0,  3,  4}
#elif NRF51
/**@brief TX power levels, based on nRF51 specifications. */
#define ESCS_NUM_OF_SUPPORTED_TX_POWER  (8)
#define ESCS_SUPPORTED_TX_POWER         {-30, -20, -16, -12,  -8,  -4,  0,  4}
#else
#error MISSING TX POWER
#endif

// Defined in Eddystone Specifications
#define ESCS_AES_KEY_SIZE               (16)
#define ESCS_ECDH_KEY_SIZE              (32)

#define ESCS_ADV_SLOT_CHAR_LENGTH_MAX   (34) // Corresponds to when the slots is configured as an EID slot

// Characteristic: Broadcast Capabilities

// Field: nrf_ble_escs_init_params_t.broadcast_cap.cap_bitfield
#define ESCS_BROADCAST_VAR_ADV_SUPPORTED_Yes          (1) // Set if the beacon supports individual per-slot adv intervals
#define ESCS_BROADCAST_VAR_ADV_SUPPORTED_No           (0)
#define ESCS_BROADCAST_VAR_ADV_SUPPORTED_Pos          (0)
#define ESCS_BROADCAST_VAR_ADV_SUPPORTED_Msk          (1 << ESCS_BROADCAST_VAR_ADV_SUPPORTED_Pos)
#define ESCS_BROADCAST_VAR_TX_POWER_SUPPORTED_Yes     (1) // Set if the beacon supports individual per-slot TX intervals
#define ESCS_BROADCAST_VAR_TX_POWER_SUPPORTED_No      (0)
#define ESCS_BROADCAST_VAR_TX_POWER_SUPPORTED_Pos     (1)
#define ESCS_BROADCAST_VAR_TX_POWER_SUPPORTED_Msk     (1 << ESCS_BROADCAST_VAR_TX_POWER_SUPPORTED_Pos)
#define ESCS_BROADCAST_VAR_RFU_MASK                   (0x03) // AND Mask to guarantee that bits 0x04 to 0x80 (RFU) are cleared

// Field: nrf_ble_escs_init_params_t.broadcast_cap.supp_frame_types
#define ESCS_FRAME_TYPE_UID_SUPPORTED_Yes             (1)
#define ESCS_FRAME_TYPE_UID_SUPPORTED_No              (0)
#define ESCS_FRAME_TYPE_UID_SUPPORTED_Pos             (0)
#define ESCS_FRAME_TYPE_UID_SUPPORTED_Msk             (1 << ESCS_FRAME_TYPE_UID_SUPPORTED_Pos)

#define ESCS_FRAME_TYPE_URL_SUPPORTED_Yes             (1)
#define ESCS_FRAME_TYPE_URL_SUPPORTED_No              (0)
#define ESCS_FRAME_TYPE_URL_SUPPORTED_Pos             (1)
#define ESCS_FRAME_TYPE_URL_SUPPORTED_Msk             (1 << ESCS_FRAME_TYPE_URL_SUPPORTED_Pos)

#define ESCS_FRAME_TYPE_TLM_SUPPORTED_Yes             (1)
#define ESCS_FRAME_TYPE_TLM_SUPPORTED_No              (0)
#define ESCS_FRAME_TYPE_TLM_SUPPORTED_Pos             (2)
#define ESCS_FRAME_TYPE_TLM_SUPPORTED_Msk             (1 << ESCS_FRAME_TYPE_TLM_SUPPORTED_Pos)

#define ESCS_FRAME_TYPE_EID_SUPPORTED_Yes             (1)
#define ESCS_FRAME_TYPE_EID_SUPPORTED_No              (0)
#define ESCS_FRAME_TYPE_EID_SUPPORTED_Pos             (3)
#define ESCS_FRAME_TYPE_EID_SUPPORTED_Msk             (1 << ESCS_FRAME_TYPE_EID_SUPPORTED_Pos)

#define ESCS_FRAME_TYPE_RFU_MASK                      (0x000F) // AND Mask to guarantee that bits 0x0010 to 0x8000 (RFU) are cleared

// Characteristic: Lock State: Lock State (READ)
#define ESCS_LOCK_STATE_LOCKED                        (0x00)
#define ESCS_LOCK_STATE_UNLOCKED                      (0x01)
#define ESCS_LOCK_STATE_UNLOCKED_AUTO_RELOCK_DISABLED (0x02)

// Characteristic: Lock State: Lock Byte (WRITE)
#define ESCS_LOCK_BYTE_LOCK                           (0x00)
#define ESCS_LOCK_BYTE_DISABLE_AUTO_RELOCK            (0x02)


// Charcteristic: Remain Connectable
#define ESCS_FUNCT_REMAIN_CONNECTABLE_SUPPORTED_Yes   (0x01)
#define ESCS_FUNCT_REMAIN_CONNECTABLE_SUPPORTED_No    (0x00)

#endif // ESCS_DEFS_H__
