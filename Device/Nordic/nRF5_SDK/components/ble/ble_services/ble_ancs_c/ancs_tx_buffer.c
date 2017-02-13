/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */
 
/* Disclaimer: This client implementation of the Apple Notification Center Service can and will be changed at any time by Nordic Semiconductor ASA.
 * Server implementations such as the ones found in iOS can be changed at any time by Apple and may cause this client implementation to stop working.
 */

 #include "nrf_ble_ancs_c.h"
 #include "ancs_tx_buffer.h"
 #include "sdk_macros.h"
 #include "nrf_log.h"
 #include "string.h"
 
 
static tx_message_t m_tx_buffer[TX_BUFFER_SIZE];                           /**< Transmit buffer for messages to be transmitted to the Notification Provider. */
static uint32_t     m_tx_insert_index = 0;                                 /**< Current index in the transmit buffer where the next message should be inserted. */
static uint32_t     m_tx_index        = 0;                                 /**< Current index in the transmit buffer from where the next message to be transmitted resides. */


void tx_buffer_init(void)
{
    memset(m_tx_buffer, 0, sizeof(m_tx_buffer));
}


void tx_buffer_insert(tx_message_t * p_msg)
{

    memset(&(m_tx_buffer[m_tx_insert_index]), 0, sizeof(m_tx_buffer)/sizeof(tx_message_t));
    
    m_tx_buffer[m_tx_insert_index].conn_handle = p_msg->conn_handle;
    m_tx_buffer[m_tx_insert_index].type        = p_msg->type;
    
    m_tx_buffer[m_tx_insert_index].req.write_req.gattc_params.handle   = p_msg->req.write_req.gattc_params.handle;
    m_tx_buffer[m_tx_insert_index].req.write_req.gattc_params.len      = p_msg->req.write_req.gattc_params.len;
    m_tx_buffer[m_tx_insert_index].req.write_req.gattc_params.write_op = p_msg->req.write_req.gattc_params.write_op;
    m_tx_buffer[m_tx_insert_index].req.write_req.gattc_params.flags    = p_msg->req.write_req.gattc_params.flags;
    m_tx_buffer[m_tx_insert_index].req.write_req.gattc_params.p_value  = m_tx_buffer[m_tx_insert_index].req.write_req.gattc_value;
    m_tx_buffer[m_tx_insert_index].req.write_req.gattc_params.offset   = p_msg->req.write_req.gattc_params.offset;

    if(p_msg->type == WRITE_REQ)
    {
        memcpy(m_tx_buffer[m_tx_insert_index].req.write_req.gattc_value,
               p_msg->req.write_req.gattc_value,
               WRITE_MESSAGE_LENGTH);
    }

    m_tx_insert_index++;
    m_tx_insert_index &= TX_BUFFER_MASK;
}



void tx_buffer_process(void)
{
    if (m_tx_index != m_tx_insert_index)
    {
        uint32_t err_code;

        if (m_tx_buffer[m_tx_index].type == READ_REQ)
        {
            err_code = sd_ble_gattc_read(m_tx_buffer[m_tx_index].conn_handle,
                                         m_tx_buffer[m_tx_index].req.read_handle,
                                         0);
        }
        else
        {
            err_code = sd_ble_gattc_write(m_tx_buffer[m_tx_index].conn_handle,
                                          &m_tx_buffer[m_tx_index].req.write_req.gattc_params);
        }
        if (err_code == NRF_SUCCESS)
        {
            ++m_tx_index;
            m_tx_index &= TX_BUFFER_MASK;
        }
    }
}
