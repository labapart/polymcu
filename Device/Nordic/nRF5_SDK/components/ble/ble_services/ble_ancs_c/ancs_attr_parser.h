/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

#ifndef BLE_ANCS_ATTR_PARSER_H__
#define BLE_ANCS_ATTR_PARSER_H__

#include "nrf_ble_ancs_c.h"

/** @file
 *
 * @addtogroup ble_ancs_c
 * @{
 */

/**@brief Function for parsing notification or app attribute response data.
 *
 * @details The data that comes from the Notification Provider can be much longer than what
 *          would fit in a single GATTC notification. Therefore, this function relies on a
 *          state-oriented switch case.
 *          UID and command ID will be received only once at the beginning of the first
 *          GATTC notification of a new attribute request for a given iOS notification.
 *          After this, we can loop several ATTR_ID > LENGTH > DATA > ATTR_ID > LENGTH > DATA until
 *          we have received all attributes we wanted as a Notification Consumer.
 *          The Notification Provider can also simply stop sending attributes.
 *
 * 1 byte  |  4 bytes    |1 byte |2 bytes |... X bytes ... |1 bytes| 2 bytes| ... X bytes ...
 * --------|-------------|-------|--------|----------------|-------|--------|----------------
 * CMD_ID  |  NOTIF_UID  |ATTR_ID| LENGTH |    DATA        |ATTR_ID| LENGTH |    DATA
 *
 * @param[in] p_ancs       Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src   Pointer to data that was received from the Notification Provider.
 * @param[in] hvx_data_len Length of the data that was received from the Notification Provider.
 */
void ancs_parse_get_attrs_response(ble_ancs_c_t  * p_ancs,
                                   const uint8_t * p_data_src,
                                   const uint16_t  hvx_data_len);

/** @} */

#endif // BLE_ANCS_ATTR_PARSER_H__

