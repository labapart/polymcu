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

 /**@file
 *
 * @defgroup bsp_nfc NFC BSP Module
 * @{
 * @ingroup bsp
 *
 * @brief Module for setting the NFCT peripheral as a wakeup source.
 *
 * @details The application must notify this module before going into System OFF mode.
 * Based on this notification, the NFC BSP Module sets the NFCT peripheral as a wakeup source
 * through the Board Support Package. Additionally, any BSP Button can be configured to
 * generate BSP sleep events. This module is applicable only if NFCT is used exclusively for
 * wakeup. If NFCT is used for a different purpose, this module cannot be used.
 */

#ifndef BSP_NFC_H__
#define BSP_NFC_H__

#include <stdint.h>
#include "sdk_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Function for initializing the NFC Button Module.
 *
 * Before calling this function, the BSP module must be initialized with buttons. The chosen
 * button is used to generate @ref BSP_EVENT_SLEEP events.
 *
 * @param[in]   sleep_button    Button ID used to generate @ref BSP_EVENT_SLEEP event.
 *
 * @retval      NRF_SUCCESS     If initialization was successful. Otherwise, a propagated error
 *                              code is returned.
 */
ret_code_t bsp_nfc_btn_init(uint32_t sleep_button);

/**@brief Function for deinitializing the NFC Button Module.
 *
 * Before calling this function, the BSP module must be initialized with buttons. The chosen
 * button is used to generate default @ref BSP_EVENT_DEFAULT events.
 *
 * @param[in]   sleep_button    Button ID used to restore default event generation.
 *
 * @retval      NRF_SUCCESS     If initialization was successful. Otherwise, a propagated error
 *                              code is returned.
 */
ret_code_t bsp_nfc_btn_deinit(uint32_t sleep_button);

/**@brief Function for setting up NFCT peripheral as wake-up source.
 *
 * This function must be called before going into System OFF mode.
 *
 * @note This function is only applicable if NFCT is used exclusively for wakeup.
 *       If NFCT is used for a different purpose, this function cannot be used.
 *
 * @retval      NRF_SUCCESS     If NFCT peripheral was prepared successfully. Otherwise,
 *                              a propagated error code is returned.
 */
ret_code_t bsp_nfc_sleep_mode_prepare(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_NFC_H__ */

/** @} */
