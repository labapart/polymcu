/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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
 
#ifndef NFC_FIXES_H__
#define NFC_FIXES_H__

#include <stdint.h>

/** @file
 * @defgroup nfc_fixes NFC fixes and workarounds
 * @{
 * @ingroup nfc_t2t
 * @brief @tagAPI52 Fixes for hardware-related anomalies.
 *
 * If you are using PCA10040 (part of nRF52 Development Kit), 
 * you must define the macro HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND in order to apply 
 * workarounds for the following anomalies:
 * - 79. NFCT: A false EVENTS_FIELDDETECTED event occurs after the field is lost.
 * - 116. NFCT does not release HFCLK when switching from ACTIVATED to SENSE mode.
 *
 * If you are using PCA10056 (part of nRF52840 Development Kit), 
 * you must define the macro HAL_NFC_NRF52840_ENGINEERING_A_WORKAROUND in order to apply 
 * workarounds for the following anomalies:
 * - 98. NFCT: The NFCT is not able to communicate with the peer.
 * - 116. NFCT does not release HFCLK when switching from ACTIVATED to SENSE mode.
 *
 * The current code contains a patch for anomaly 25 (NFCT: Reset value of 
 * SENSRES register is incorrect), so that it now works on Windows Phone.
 */
 
#ifdef BOARD_PCA10040           // assume nRF52832 chip in IC rev. Engineering B or Engineering C
    #define HAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND
#elif defined(BOARD_PCA10056)   // assume nRF52840 chip in IC rev. Engineering A
    #define HAL_NFC_NRF52840_ENGINEERING_A_WORKAROUND
#endif

/** @} */
#endif /* NFC_FIXES_H__ */

