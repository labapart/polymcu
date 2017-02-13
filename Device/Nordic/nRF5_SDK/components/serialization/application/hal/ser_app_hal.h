/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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

/**@file
 *
 * @defgroup ser_app_hal Serialization Application Hardware Abstraction Layer (HAL)
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief @tagAPI52832 Functions that set up hardware on Application Board and perform the reset of the Connectivity Board.
 */

#ifndef SER_APP_HAL_H_
#define SER_APP_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*ser_app_hal_flash_op_done_handler_t)(bool success);
/**@brief Function for initializing hardware modules.
 *
 * @details Function can initialize hardware modules on the Application Chip. It is optional to
 * implement. It is called once the Connectivity Chip is initialized.
 *
 * @param handler Flash operation event handler.
 *
 * @return @ref NRF_SUCCESS HAL initialized successfully.
 * @return @ref nrf_error "NRF_ERROR_..." HAL initialization failed.
 *
 */
uint32_t ser_app_hal_hw_init(ser_app_hal_flash_op_done_handler_t handler);

/**@brief Function for waiting for a given amount of time.
 *
 * @param[in] ms Number of milliseconds to wait.
 *
 */
void ser_app_hal_delay(uint32_t ms);

/**@brief Function for clearing the Connectivity Chip reset pin.
 *
 */
void ser_app_hal_nrf_reset_pin_clear(void);

/**@brief Function for setting the Connectivity Chip reset pin.
 *
 */
void ser_app_hal_nrf_reset_pin_set(void);


/**@brief Function for setting the SoftDevice event interrupt priority that serves the events incoming
 * from the Connectivity Chip.
 *
 * @note Serialization solution on the application side mimics a SoC solution where events are handled in
 * the interrupt context in two ways: either directly in the interrupt context or with a message being posted to
 * the scheduler. However, it is possible that the Application Chip does not use a dedicated interrupt
 * for connectivity events. In that case, this function can be left empty and
 * \ref ser_app_hal_nrf_evt_pending will directly call an interrupt handler function.
 */
void ser_app_hal_nrf_evt_irq_priority_set(void);

/**@brief Function for setting a pending interrupt for serving events incoming from the Connectivity Chip.
 *
 * @note The interrupt used for event from the Connectivity Chip mimics behavior of SoC and it is not
 * intended to be triggered by any hardware event. This function should be the only source of
 * interrupt triggering.
 */
void ser_app_hal_nrf_evt_pending(void);



#ifdef __cplusplus
}
#endif

#endif /* SER_APP_HAL_H_ */
/** @} */
