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

#ifndef ES_FLASH_H__
#define ES_FLASH_H__

#include <stdint.h>
#include <stdbool.h>
#include "fds.h"
#include "nrf_ble_escs.h"
#include "es_app_config.h"
#include "es_slot.h"

/**
 * @file
 * @defgroup eddystone_flash Flash access
 * @brief Types and functions to access the flash of the Eddystone beacon.
 * @ingroup eddystone
 * @{
 */

#define WORD_SIZE 4

#define FLASH_ACCES_ERROR_CHECK_ALLOW_NOT_FOUND(err_code) \
    if (err_code != (FDS_ERR_NOT_FOUND)) APP_ERROR_CHECK(err_code);


#define FLASH_OP_WAIT() uint32_t pending_ops = es_flash_num_pending_ops(); \
    while (pending_ops != 0)                                               \
    {                                                                      \
        pending_ops = es_flash_num_pending_ops();                          \
    }                                                                      \

/**@brief Beacon configuration. */
typedef struct
{
    nrf_ble_escs_adv_interval_t    adv_interval;       //!< Advertising interval.
    bool                           remain_connectable; //!< Flag that specifies if the beacon should remain connectable.
} es_flash_beacon_config_t;


/**@brief Structure for keeping track of which slot has a configuration that must be restored upon reboot.
 * @details The size of this structure must be word aligned and match the flash block size of 32 bytes.
 */
typedef struct
{
    bool    slot_is_empty[APP_MAX_ADV_SLOTS];   //!< Flag that indicates whether the slot is empty.
    uint8_t padding[WORD_SIZE - ((APP_MAX_ADV_SLOTS + 1) % WORD_SIZE)]; //!< Padding used to ensure word alignment.
} es_flash_flags_t;

/**@brief Flash access types.
 */
typedef enum
{
    ES_FLASH_ACCESS_READ,    //!< Read data.
    ES_FLASH_ACCESS_WRITE,   //!< Write data.
    ES_FLASH_ACCESS_CLEAR    //!< Clear data.
} es_flash_access_t;


/**@brief Function for accessing beacon configurations.
 *
 * @param[out,in]   p_config Pointer to the beacon configuration buffer.
 * @param[in]       access_type    Access type (see @ref es_flash_access_t).
 * @return          For possible return values, see:
 *                  - @ref fds_record_find_by_key
 *                  - @ref fds_record_open
 *                  - @ref fds_record_close
 *                  - @ref fds_record_write
 *                  - @ref fds_record_update
 *                  - @ref fds_record_delete
 */
ret_code_t es_flash_access_beacon_config(es_flash_beacon_config_t * p_config,
                                         es_flash_access_t          access_type);

/**@brief Function for accessing public ECDH key of last configured EID slot.
 *
 * @param[out,in]   p_priv_key     Pointer to the private key buffer.
 * @param[out,in]   p_pub_key      Pointer to the public key buffer.
 * @param[in]       access_type    Access type (see @ref es_flash_access_t).
 * @return          For possible return values, see:
 *                  - @ref fds_record_find_by_key
 *                  - @ref fds_record_open
 *                  - @ref fds_record_close
 *                  - @ref fds_record_write
 *                  - @ref fds_record_update
 *                  - @ref fds_record_delete
 */
ret_code_t es_flash_access_ecdh_key_pair(uint8_t         * p_priv_key,
                                         uint8_t         * p_pub_key,
                                         es_flash_access_t access_type);


/**@brief Function for accessing slot configuration from flash.
 *
 * @param[in]       slot_no        Slot index.
 * @param[out,in]   p_slot         Pointer to the slot configuration buffer.
 * @param[in]       access_type    Access type (see @ref es_flash_access_t).
 * @return          For possible return values, see:
 *                  - @ref fds_record_find_by_key
 *                  - @ref fds_record_open
 *                  - @ref fds_record_close
 *                  - @ref fds_record_write
 *                  - @ref fds_record_update
 *                  - @ref fds_record_delete
 */
ret_code_t es_flash_access_slot_configs(uint8_t           slot_no,
                                        es_slot_t   * p_slot,
                                        es_flash_access_t access_type);


/**@brief Function for accessing the beacon lock key from flash.
 *
 * @param[out,in]   p_lock_key     Pointer to the lock key buffer.
 * @param[in]       access_type    Access type (see @ref es_flash_access_t).
 * @return          For possible return values, see:
 *                  - @ref fds_record_find_by_key
 *                  - @ref fds_record_open
 *                  - @ref fds_record_close
 *                  - @ref fds_record_write
 *                  - @ref fds_record_update
 *                  - @ref fds_record_delete
 */
ret_code_t es_flash_access_lock_key(uint8_t * p_lock_key, es_flash_access_t access_type);


/**@brief Function for accessing the flash configuration flag from flash.
 *
 * @param[out,in]   p_flags        Pointer to the flag buffer.
 * @param[in]       access_type    Access type (see @ref es_flash_access_t).
 * @return          For possible return values, see:
 *                  - @ref fds_record_find_by_key
 *                  - @ref fds_record_open
 *                  - @ref fds_record_close
 *                  - @ref fds_record_write
 *                  - @ref fds_record_update
 *                  - @ref fds_record_delete
  */
ret_code_t es_flash_access_flags(es_flash_flags_t * p_flags, es_flash_access_t access_type);


/**@brief Function for retrieving the number of queued operations.
 * @return The number of operations that are queued.
 */
uint32_t es_flash_num_pending_ops(void);

/**@brief Function for performing a factory reset.
 * @return FDS return code.
 */
ret_code_t es_flash_factory_reset(void);

void es_flash_on_ble_evt(ble_evt_t * p_evt);

/**@brief Function for initializing the flash module.
 *
 * @return See @ref fds_init for possible return values.
 */
ret_code_t es_flash_init(void);

/**
 * @}
 */

#endif // ES_FLASH_H__
