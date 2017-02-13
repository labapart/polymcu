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

#ifndef HAL_NFC_H__
#define HAL_NFC_H__

/** @file
 * @defgroup nfc_t4t_hal NFC Type 4 Tag HAL
 * @{
 * @ingroup nfc_t4t
 * @brief @tagAPI52 Hardware abstraction layer for the NFC Type 4 Tag library.
 *
 * @note Before the NFCT peripheral enters ACTIVATED state, the HFXO must be running.
 * To fulfill this requirement and allow other software modules to also request the HFXO, the NFC Type 4 Tag HAL uses @ref nrf_drv_clock module.
 *
 */

#include <stdint.h>
#include <string.h>
#include <sdk_errors.h>


/** @brief Events passed to the upper-layer callback function. */
typedef enum {
    HAL_NFC_EVENT_FIELD_ON,           ///< Field is detected.
    HAL_NFC_EVENT_FIELD_OFF,          ///< Field is lost.
    HAL_NFC_EVENT_DATA_RECEIVED,      ///< Data is received.
    HAL_NFC_EVENT_DATA_TRANSMITTED    ///< Data is transmitted.
} hal_nfc_event_t;


/** @brief Parameter IDs for the set/get function. */
typedef enum {
    HAL_NFC_PARAM_ID_TESTING,         ///< Used for unit tests.
    HAL_NFC_PARAM_FWI,                ///< Frame Wait Time parameter.
    HAL_NFC_PARAM_SELRES,             ///< Parameter for setting the 'Protocol' bits for SEL_RES packet.
    HAL_NFC_PARAM_ID_UNKNOWN
} hal_nfc_param_id_t;


/** @brief Callback from HAL_NFC layer into the upper layer.
  *
  * If event == HAL_NFC_EVENT_DATA_RECEIVED:
  * p_data points to the received packet. The memory belongs to the HAL_NFC layer and
  * is guaranteed to be valid only until the callback returns.
  *
  * If event == HAL_NFC_EVENT_DATA_TRANSMITTED:
  * p_data points to the transmitted packet. The memory belongs to the application.
  *
  * If event == \<Other event\>:
  * p_data definition is event-specific (to be defined).
  *
  * @param[in] p_context    Context for callback execution.
  * @param[in] event        The event that occurred.
  * @param[in] p_data       Received/transmitted data or NULL.
  * @param[in] data_length  Size of the received/transmitted packet.
  */
typedef void (* hal_nfc_callback_t)(void          * p_context,
                                    hal_nfc_event_t event,
                                    const uint8_t * p_data,
                                    size_t          data_length);


/** @brief Function for initializing the NFC layer.
  *
  * This function provides a pointer to a callback function and the callback context
  * to the NFC layer.
  *
  * @param[in] callback     Pointer to the callback function.
  * @param[in] p_context    Context of callback.
  *
  * @retval NRF_SUCCESS If the NFC layer was initialized successfully.  If one 
  *                     of the arguments was invalid, an error code is returned.
  */
ret_code_t hal_nfc_setup(hal_nfc_callback_t callback, void * p_context);


/** @brief Function for setting a HAL_NFC parameter.
  *
  * This function allows to set any parameter defined as available by HAL_NFC.
  *
  * @param[in] id           ID of the parameter to set.
  * @param[in] p_data       Pointer to the buffer containing the data to set.
  * @param[in] data_length  Size of the buffer containing the data to set.
  *
  * @retval NRF_SUCCESS If the parameter was set successfully. If one of the arguments
  *                     was invalid (for example, wrong data length), an error code
  *                     is returned.
  */
ret_code_t hal_nfc_parameter_set(hal_nfc_param_id_t id, void * p_data, size_t data_length);


/** @brief Function for querying a HAL_NFC parameter value.
  *
  * The queried value will be placed into the passed data buffer. If the buffer
  * is too small, p_max_data_length will contain the required buffer size.
  *
  * @param[in]      id                ID of the parameter to query.
  * @param[in, out] p_data            Pointer to a buffer receiving the queried data.
  * @param[in, out] p_max_data_length Size of the buffer. It receives the required size if buffer is too small.
  *
  * @retval NRF_SUCCESS If the parameter was received successfully. If one of the arguments
  *                     was invalid (for example, the buffer was too small), an error code
  *                     is returned.
  */
ret_code_t hal_nfc_parameter_get(hal_nfc_param_id_t id, void * p_data, size_t * p_max_data_length);


/** @brief Function for starting the NFC subsystem.
  *
  * After this function completes, NFC readers will be able to detect the chip.
  *
  * @retval NRF_SUCCESS If the NFC subsystem was started successfully. If the NFC 
  *                     subsystem could not be started, an error code is returned.
  */
ret_code_t hal_nfc_start(void);


/** @brief Function for sending a packet to the connected NFC reader.
  *
  * The provided data buffer belongs to the caller and is guaranteed to be
  * valid until the HAL_NFC_EVENT_DATA_TRANSMITTED event is received by the
  * callback.
  *
  * @param[in] p_data       The data packet to send.
  * @param[in] data_length  Size of the packet in bytes.
  *
  * @retval NRF_SUCCESS If the packet was sent. Otherwise, an error code is returned.
  */
ret_code_t hal_nfc_send(const uint8_t * p_data, size_t data_length);


/** @brief Function for stopping the NFC subsystem.
  *
  * After this function returns, NFC readers will no longer be able to connect
  * to the chip.
  *
  * @retval NRF_SUCCESS If the NFC subsystem was stopped. Otherwise, an error code
  *                     is returned.
  */
ret_code_t hal_nfc_stop(void);


/** @brief Function for releasing resources.
  *
  * After this call returns, the callback is considered invalid and no more
  * events will be posted to it.
  *
  * @retval NRF_SUCCESS This function always succeeds.
  */
ret_code_t hal_nfc_done(void);


/** @} */
#endif /* HAL_NFC_H__ */

