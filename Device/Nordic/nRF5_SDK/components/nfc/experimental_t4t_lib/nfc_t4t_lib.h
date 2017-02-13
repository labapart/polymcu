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

#ifndef NFC_T4T_LIB_H__
#define NFC_T4T_LIB_H__

/** @file
 *
 * @addtogroup nfc_api
 *
 * @defgroup nfc_t4t NFC Type 4 Tag
 * @ingroup nfc_api
 * @brief Implementation of NFC Type 4 Tag.
 *
 * @defgroup nfc_t4t_lib NFC tag 4 type emulation library
 * @{
 * @ingroup nfc_t4t
 * @brief The T4T emulation library interface
 *
 * This is the NFC Forum NDEF tag 4 type emulation library. It implements the ISO14443-4A protocol
 * (ISO-DEP) and additionally can emulate a read-writable NDEF content. If the emulation of the NDEF
 * content is not needed, the library works in a raw mode where all APDUs are delivered to the user,
 * who is then responsible to generate a timely RPDU as a response.
 *
 * The sequence of initializing functions determines whether the NDEF emulation will run or whether
 * the raw mode is used.
 *
 *   - E.g. NDEF emulation
 *        * @ref nfc_t4t_setup
 *        * @ref nfc_t4t_ndef_rwpayload_set or @ref nfc_t4t_ndef_staticpayload_set
 *        * @ref nfc_t4t_emulation_start
 *        * ... running in NDEF emulation mode ...
 *   - E.g. RAW mode 
 *        * @ref nfc_t4t_setup
 *        * @ref nfc_t4t_emulation_start
 *        * ... running in RAW emulation mode ...
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sdk_errors.h"

#define NFC_T4T_MAX_PAYLOAD_SIZE  0xFFF0U

/**< @brief Emulation mode. */
typedef enum
{
    NFC_T4T_EMUMODE_NDEF,   ///< Emulated NDEF AID and EF-Files.
    NFC_T4T_EMUMODE_PICC    ///< Run just ISO-DEP, deliver I-Frames up.
} nfc_t4t_emu_mode_t;


/**< @brief Event identifiers used by the @ref nfc_t4t_callback_t */
typedef enum 
{
    NFC_T4T_EVENT_NONE,
    ///< This ID is never used. Dummy value for completeness.

    NFC_T4T_EVENT_FIELD_ON,
    ///< External Reader polling detected.

    NFC_T4T_EVENT_FIELD_OFF,
    ///< External Reader polling ended.

    NFC_T4T_EVENT_NDEF_READ,
    ///< External Reader has read static NDEF-Data from Emulation.
    /**<
     * A Read operation happened on last byte of NDEF-Data.
     */

    NFC_T4T_EVENT_NDEF_UPDATED,
    ///< External Reader has written to length information of NDEF-Data from Emulation.
    /**<
     * The usual behavior of a Reader-Writer that accesses NDEF information for update is to set
     * the length to zero at the beginning of the update process. It then writes the content
     * of NDEF-Data. When all content is written it will update the length information inside
     * the NDEF file. This event will be generated every time an update to  the length is happening.
     * This length information is residing in the first 2 bytes of the NDEF-Content container and is called 'NLEN'.
     * Since this callback is triggered on any access to these bytes  the returned data_length
     * information might not be consistent (e.g. in case of only a single byte write to the length).
     *
     * @param[out] data_length Current value of NDEF content length
     *                         information i.e. 'NLEN' field.
     */

    NFC_T4T_EVENT_DATA_TRANSMITTED, 
    ///< In Raw mode it signals that the data from @ref nfc_t4t_response_pdu_send have been sent out.

    NFC_T4T_EVENT_DATA_IND,
    ///< In Raw mode delivers the APDU fragments
    /**<
     * All @ref NFC_T4T_EVENT_DATA_IND events that have  the @ref NFC_T4T_DI_FLAG_MORE flag set 
     * belong to the same APDU. The first @ref NFC_T4T_EVENT_DATA_IND without @ref NFC_T4T_DI_FLAG_MORE
     * flag signals the completeness of the APDU. The Application then has to reply with a call
     * to @ref nfc_t4t_response_pdu_send. The library will handle internally the fragmentation of
     * the response towards the Reader-Writer. The data of the response PDU must be kept 
     * valid until the next callback from the library happens (e.g. next @ref NFC_T4T_EVENT_DATA_IND
     * or @ref NFC_T4T_EVENT_FIELD_OFF).
     *
     * @param[out] p_data       Pointer to the fragment of APDU.
     * @param[out] data_length  Length of data.
     * @param[out] flags        @ref nfc_t4t_data_ind_flags_t.
     */
} nfc_t4t_event_t;

/**< @brief Flags coming with nfc_t4t_callback_t at @ref NFC_T4T_EVENT_DATA_IND event.*/
typedef enum
{
    NFC_T4T_DI_FLAG_NONE = 0x00,    ///< Dummy value.
    NFC_T4T_DI_FLAG_MORE = 0x01     ///< This signals that more data is expected to be received.
} nfc_t4t_data_ind_flags_t;

/**< @brief Parameter IDs that can be set/get with @ref nfc_t4t_parameter_set or 
 *          @ref nfc_t4t_parameter_get.
 */
typedef enum
{
    NFC_T4T_PARAM_TESTING,      ///< Internal usage only for Unit-Testing.
    NFC_T4T_PARAM_FWI,          ///< Frame Wait Time parameter
    NFC_T4T_PARAM_SELRES        ///< Parameter for setting 'Protocol' bits for SEL_RES packet
} nfc_t4t_param_id_t;

/** @brief Callback to pass events from NFCLib to application.
 *
 * @param[out] p_context    Application context for callback execution.
 * @param[out] event        The event that occurred. see @ref nfc_t4t_event_t.
 * @param[out] p_data       Data to send to the application (event specific).
 * @param[out] data_length  Length of the data. In case of @ref NFC_T4T_EVENT_NDEF_UPDATED, this parameter
 *                          contains the value of the 'NLEN' field of the NDEF File;
 *                          if the value is non-zero, it corresponds to the new size of the NDEF Message in the updated NDEF File.
 * @param[out] flags        Some events deliver flags. see @ref nfc_t4t_event_t for details.
 */
typedef void (*nfc_t4t_callback_t)(void          * p_context,
                                   nfc_t4t_event_t event,
                                   const uint8_t * p_data, 
                                   size_t          data_length,
                                   uint32_t        flags);

/** @brief Register the application callback for event signaling.
 *
 * The callback will be called by NFCLib to notify the application of relevant events. It will be
 * called from the HAL_NFC callback context. The library support 3 different Modes of Emulation:
 *  - Raw ISO-Dep exchanges. All PDUs are signaled through the callback.
 *  - Read-Only T4T NDEF-Tag. A static buffer is served. Only Field-Status callbacks.
 *  - Read-Write T4T NDEF-Tag. A mutable buffer is used. Only Field-Status callbacks.
 *
 * The default mode is Raw ISO-Dep mode. The two other NDEF T4T modes are activated through 
 * the corresponding @ref nfc_t4t_ndef_rwpayload_set/ @ref nfc_t4t_ndef_staticpayload_set functions.
 * The mode is locked in with a call to @ref nfc_t4t_emulation_start.
 *
 * @param[in] callback     Function pointer to the callback.
 * @param[in] p_context    Pointer to a memory area used by the callback for execution (optional).
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_INVALID_STATE  If emulation is in running state.
 */
ret_code_t nfc_t4t_setup(nfc_t4t_callback_t callback, void * p_context);

/** @brief Set emulation buffer and content for a NDEF Tag emulation that is Read/Writable.
 *
 * The buffer needs to be kept accessible for the lifetime of the emulation.
 * If an external Reader-Writer changes the NDEF content it is signaled through the app-callback.
 *
 * @param[in] p_emulation_buffer      Buffer pointer
 * @param[in] buffer_length           Length of buffer (maximum writable NDEF size)
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_DATA_SIZE      Invalid argument (e.g. wrong data length).
 * @retval NRF_ERROR_INVALID_PARAM  Invalid argument (e.g. NULL pointer).
 * @retval NRF_ERROR_INVALID_STATE  If emulation is in running state.
 */
ret_code_t nfc_t4t_ndef_rwpayload_set(uint8_t * p_emulation_buffer, size_t buffer_length);

/** @brief Set emulationBuffer and Content for a NDEF Tag emulation that is Read-Only.
 *
 * The buffer needs to be kept accessible for the lifetime of the emulation.
 * Since no write access is done to the buffer, the content could reside in flash memory.
 *
 * @param[in] p_emulation_buffer      Const buffer pointer
 * @param[in] buffer_length           Length of contained NDEF payload message
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_DATA_SIZE      Invalid argument (e.g. wrong data length).
 * @retval NRF_ERROR_INVALID_PARAM  Invalid argument (e.g. NULL pointer).
 * @retval NRF_ERROR_INVALID_STATE  Emulation is in running stated.
 */
ret_code_t nfc_t4t_ndef_staticpayload_set(const uint8_t * p_emulation_buffer, size_t buffer_length);

/** @brief Send a raw response PDU after getting a Request PDU callback.
 *
 * When the library works in raw ISO-DEP mode it will signal request PDUs through the callback.
 * The application then has to answer with a response PDU. It will use this function to send back
 * the response PDU. This function can not be used in T4T NDEF (RW / STATIC) emulation modes.
 *
 * The lower ISODEP layer will handle the defragmentation of a long response PDU into smaller
 * pieces that the PCD can understand.
 *
 * @param[in] p_pdu                   Const PDU pointer.
 * @param[in] pdu_length              Length of PDU.
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_DATA_SIZE      Invalid argument (e.g. wrong data length).
 * @retval NRF_ERROR_INVALID_PARAM  Invalid argument (e.g. NULL pointer).
 * @retval NRF_ERROR_INVALID_STATE  Emulation is in running state.
 */
ret_code_t nfc_t4t_response_pdu_send(const uint8_t * p_pdu, size_t pdu_length);

/** @brief Set an NFC parameter.
 *
 * Allows to set any parameter defined as available by HAL_NFC.
 *
 * @param[in] id                      ID of the parameter to set.
 * @param[in] p_data                  Pointer to a buffer containing the data to set.
 * @param[in] data_length             Size of the buffer containing the data to set.
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_DATA_SIZE      Invalid argument (e.g. wrong data length).
 * @retval NRF_ERROR_INVALID_PARAM  Invalid argument (e.g. NULL pointer).
 */
ret_code_t nfc_t4t_parameter_set(nfc_t4t_param_id_t id, void * p_data, size_t data_length);

/** @brief Query an NFC parameter value.
 *
 * The queried value will be placed into the passed data buffer.
 * If the buffer is too small, p_max_data_length will contain the required buffer size.
 * If the buffer is big enough, p_max_data_length will contain the actual size of the data.
 *
 * @param[in]  id                     ID of the parameter to query.
 * @param[out] p_data                 Pointer to a buffer receiving the queried data.
 * @param[out] p_max_data_length      Size of the buffer, receives actual size of queried data.
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_DATA_SIZE      Invalid argument (e.g. wrong data length).
 * @retval NRF_ERROR_INVALID_PARAM  Invalid argument (e.g. NULL pointer).
 */
ret_code_t nfc_t4t_parameter_get(nfc_t4t_param_id_t id, void * p_data, size_t * p_max_data_length);

/** @brief Activate the NFC frontend.
 *
 * Only after calling this function, events will be posted to the application callback.
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_INVALID_STATE  Already started.
 */
ret_code_t nfc_t4t_emulation_start(void);

/**
 * @brief Deactivate the NFC frontend.
 *
 * After calling this function, no more events will be posted to the application callback.
 *
 * @retval NRF_SUCCESS              Success.
 * @retval NRF_ERROR_INVALID_STATE  Emulation was already stopped
 */
ret_code_t nfc_t4t_emulation_stop(void);

/**
 * @brief Release reference to application callback.
 *
 * After calling this function, the passed callback pointer is no longer considered valid.
 * After calling this function, the passed p_ndef pointer is no longer considered valid.
 *
 * You need to restart with @ref nfc_t4t_setup to run a new Emulation.
 *
 * @retval NRF_SUCCESS  Always succeeds.
 */
ret_code_t nfc_t4t_done(void);

/**
  @}
*/

#endif // NFC_T4T_LIB_H__
