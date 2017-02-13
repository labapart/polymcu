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

#ifndef ES_SECURITY_H__
#define ES_SECURITY_H__

#include "nrf_ble_escs.h"
#include "app_error.h"

/**
 * @file
 * @defgroup eddystone_security Security
 * @brief Types and functions for dealing with security of Eddystone beacons.
 * @ingroup eddystone
 * @{
 */

/**@brief   Security events.
 */
typedef enum
{
    ES_SECURITY_MSG_UNLOCKED,  //!<  Beacon is unlocked.
    ES_SECURITY_MSG_EID,       //!<  EID has been generated.
    ES_SECURITY_MSG_IK,        //!<  IK has been generated.
    ES_SECURITY_MSG_ECDH,      //!<  Public ECDH has been generated.
    ES_SECURITY_MSG_STORE_TIME //!<  EID slot time must be stored.
} es_security_msg_t;

/* @brief Callback for security events. */
typedef void (*es_security_msg_cb_t)(uint8_t slot_no, es_security_msg_t msg_type);

/**@brief EID configuration.
 * 
 * @details This structure is used to preserve or restore an EID slot. 
 *
 * @note This is a packed structure. Therefore, you should not change it.
*/
typedef PACKED_STRUCT
{
    es_frame_type_t frame_type;
    uint8_t         k_scaler;
    uint32_t        time_counter;
    uint8_t         ik[ESCS_AES_KEY_SIZE];
} es_eid_config_t;

/**@brief   Eddystone beacon lock state.
 */
typedef nrf_ble_escs_lock_state_read_t es_security_lock_state_t;

/**@brief Function for initializing the security module.
 *
 * @param[in] msg_cb Callback function.
 *
 * @return See @ref app_timer_start for possible return values.
 */
ret_code_t es_security_init(es_security_msg_cb_t msg_cb);


/**@brief Function for updating the lock code and storing it to flash.
 *
 * @param[in] p_encrypted_key       Pointer to the new lock code.
 * 
 * @return See @ref es_flash_access_lock_key for possible return values.
 */
ret_code_t es_security_lock_code_update(uint8_t * p_encrypted_key);


/**@brief Function for reading the challenge and encrypting it with AES_ECB.
 *
 * @details The result of the encryption is compared with the provided unlock token
 *         in @ref es_security_unlock_verify.
 *
 * @param[in] p_challenge       Pointer to the challenge buffer.
 *
 * @return See @ref sd_ecb_block_encrypt for possible return values.
 */
void es_security_unlock_prepare(uint8_t * p_challenge);


/**@brief Function for unlocking the beacon.
 *
 * @details This function compares the result from @ref es_security_unlock_prepare to the input unlock token and unlocks the beacon if matching.
 *
 * @param[in] p_unlock_token    The unlock token written by the client.
 */
void es_security_unlock_verify(uint8_t * p_unlock_token);


/**@brief Function for generating a random challenge for the unlock characteristic.
 *
 * @param[out]   p_rand_chlg_buff    Pointer to a buffer to which the random challenge is copied.
 *
 * @return See @ref sd_rand_application_vector_get for possible return values.
 */
ret_code_t es_security_random_challenge_generate(uint8_t * p_rand_chlg_buff);


/**@brief Function for storing the public ECDH key from the client in the beacon registration process.
 *
 * @details This function starts a series of cryptographic activities, including the generation of temporary keys and EIDs.
 *
 * @param[in] slot_no         The index of the slot whose public ECDH key is retrieved.
 * @param[in] p_pub_ecdh      Pointer to the public ECDH.
 * @param[in] scaler_k        K rotation scaler.
 */
ret_code_t es_security_client_pub_ecdh_receive(uint8_t   slot_no,
                                               uint8_t * p_pub_ecdh,
                                               uint8_t   scaler_k);


/**@brief Function for storing the shared IK from the client in the beacon registration process.
 *
 * @details This function starts a series of cryptographic activities, including the generation of temporary keys and EIDs.
 *
 * @param[in] slot_no         The index of the slot whose public ECDH key is retrieved.
 * @param[in] p_encrypted_ik  Pointer to the received IK.
 * @param[in] scaler_k        K rotation scaler.
 */
void es_security_shared_ik_receive(uint8_t   slot_no,
                                   uint8_t * p_encrypted_ik,
                                   uint8_t   scaler_k);


/**@brief Function for copying the 32-byte ECDH key into the provided buffer.
 *
 * @param[in]  slot_no         The index of the slot whose public ECDH key is retrieved.
 * @param[out] p_edch_buffer   Pointer to the buffer.
 */
void es_security_pub_ecdh_get(uint8_t slot_no, uint8_t * p_edch_buffer);


/**@brief Function for returning the beacon clock value (in little endian).
 *
 * @param[in] slot_no   The index of the slot.
 *
 * @return    32-bit clock value.
 */
uint32_t es_security_clock_get(uint8_t slot_no);


/**@brief Function for updating the beacon time counter. 
 *
 * @details This function checks how much time has passed since the last
 * invocation and, if required, updates the EID, the temporary key, or both.
 * The function generates an @ref ES_SECURITY_MSG_STORE_TIME event 
 * for each active security slot every 24 hours.
 */
void es_security_update_time(void);


/**@brief Function for returning the rotation exponent scaler value.
 *
 * @param[in] slot_no   The index of the slot.
 *
 * @return    K rotation scaler.
 */
uint8_t es_security_scaler_get(uint8_t slot_no);


/**@brief Function for copying the 8-byte EID into the provided buffer.
 *
 * @param[in] slot_no        The index of the slot whose EID is retrieved.
 * @param[out] p_eid_buffer  Pointer to the buffer.
 */
void es_security_eid_get(uint8_t slot_no, uint8_t * p_eid_buffer);


/**@brief Function for restoring an EID slot.
 *
 * @param[in] slot_no               The index of the slot to restore.
 * @param[in] k_scaler              K rotation scaler.
 * @param[in] time_counter          EID slot time counter value (in seconds).
 * @param[in] p_ik                  Pointer to the identity key of the specified slot.
 */
void es_security_eid_slots_restore(uint8_t         slot_no,
                                   uint8_t         k_scaler,
                                   uint32_t        time_counter,
                                   const uint8_t * p_ik);


/**@brief Function for destroying stored EID states.
 *
 * @details This function should be called when the slot is either overwritten as another slot or cleared by writing an empty byte or a single 0.
 *
 * @param[in] slot_no  The index of the slot to destroy.
 */
void es_security_eid_slot_destroy(uint8_t slot_no);


/**@brief Function for fetching the EID configuration.
 *
 * @param[in] slot_no  The index of the slot for which to load the configuration.
 * @param[out] p_config  Pointer to the EID configuration.
 */
void es_security_eid_config_get(uint8_t slot_no, es_eid_config_t * p_config);


/**@brief Function for preserving an ECDH key pair by writing it to flash.
 *
 * @return See @ref es_flash_access_ecdh_key_pair for possible return values.
 */
ret_code_t es_security_ecdh_pair_preserve(void);


/**@brief Function for copying the 16-byte EID ID key into the provided buffer.
 *
 * @param[in]   slot_no         The index of the EID slot whose IK is retrieved.
 * @param[out]  p_key_buffer    Buffer for the key.
 */
void es_security_plain_eid_id_key_get(uint8_t slot_no, uint8_t * p_key_buffer);


/**@brief Function for copying the 16-byte LK encrypted EID ID key into the provided buffer.
 *
 * @param[in]   slot_no         The index of the EID slot whose encrypted IK is retrieved.
 * @param[out]  p_key_buffer    Buffer for the key.
 */
void es_security_encrypted_eid_id_key_get(uint8_t slot_no, uint8_t * p_key_buffer);


/**@brief Function for converting a TLM frame into an eTLM frame using the EIK of the specified slot.
 *
 * @param[in]   ik_slot_no  The index of the EID slot whose IK is paired with the eTLM.
 * @param[in]   p_tlm       Pointer to the TLM frame buffer.
 * @param[out]  p_etlm      Pointer to the eTLM frame buffer.
 */
void es_security_tlm_to_etlm(uint8_t ik_slot_no, es_tlm_frame_t * p_tlm, es_etlm_frame_t * p_etlm);

/**
 * @}
 */

#endif // ES_SECURITY_H__
