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

#ifndef ANCS_TX_BUFFER_H__
#define ANCS_TX_BUFFER_H__

#include "nrf_ble_ancs_c.h"

/** @file
 *
 * @addtogroup ble_ancs_c
 * @{
 */

#define TX_BUFFER_MASK                   0x07                     //!< TX buffer mask. Must be a mask of contiguous zeroes followed by a contiguous sequence of ones: 000...111.
#define TX_BUFFER_SIZE                   (TX_BUFFER_MASK + 1)     //!< Size of the send buffer, which is 1 bigger than the mask.
#define WRITE_MESSAGE_LENGTH             20                       //!< Length of the write message for the CCCD/control point.

/**@brief ANCS request types.
 */
typedef enum
{
    READ_REQ = 1,  /**< Type identifying that this TX message is a read request. */
    WRITE_REQ      /**< Type identifying that this TX message is a write request. */
} tx_request_t;


/**@brief Structure for writing a message to the central, thus the Control Point or CCCD.
 */
typedef struct
{
  uint8_t                  gattc_value[WRITE_MESSAGE_LENGTH]; //!< The message to write.
  ble_gattc_write_params_t gattc_params;                      //!< GATTC parameters for this message. 
} write_params_t;


/**@brief Data to be transmitted to the connected master.
 */
typedef struct
{
  uint16_t          conn_handle;  //!< Connection handle to be used when transmitting this message. 
  tx_request_t type;         //!< Type of this message (read or write message). 
    union
    {
      uint16_t       read_handle;    //!< Read request message.
      write_params_t write_req; //!< Write request message.
    } req;
} tx_message_t;

/**@brief Function for clearing the TX buffer. 
 *
 * @details Always call this function before using the TX buffer.
*/
void tx_buffer_init(void);

/**@brief Function for moving the pointer of the ring buffer to the next element.
*/
void tx_buffer_insert(tx_message_t * p_msg);

/**@brief Function for passing any pending request from the buffer to the stack.
*/
void tx_buffer_process(void);

/** @} */

#endif // ANCS_TX_BUFFER_H__

