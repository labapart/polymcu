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

/** @file
 *
 * @defgroup ser_phy Serialization PHY
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   @tagAPI52832 PHY layer for serialization.
 *
 * @details Here you can find declarations of functions and definitions of data structures and
 *          identifiers (typedef enum) used as API of the serialization PHY layer.
 *
 * \par Rationale
 * Each specific PHY layer (SPI, I2C, UART, low power UART etc.) should provide the same API. This
 * allows the layer above (the HAL Transport layer), which is responsible for controlling the PHY
 * layer, memory management, crc, retransmission etc., to be hardware independent.
 *
 *
 * \par Interlayer communication and control
 * The PHY layer is controlled by the HAL Transport layer by calling functions declared in this
 * file.
 * The PHY layer communicates events to the HAL Transport layer by calling a callback function.
 * A handler to this function is passed in the @ref ser_phy_open function. This callback function
 * should be called with parameter of type @ref ser_phy_evt_t, filled accordingly to an event to be
 * passed. Types of supported events are defined in @ref ser_phy_evt_type_t.
 * For example, to pass an event indicating that RX packet has been successfully received, first a
 * struct of type @ref ser_phy_evt_t has to be filled:
 * @code
 * ser_phy_evt_t phy_evt;
 * phy_evt.evt_type = SER_PHY_EVT_RX_PKT_RECEIVED;
 * phy_evt.evt_params.rx_pkt_received.p_buffer = (pointer to the RX buffer);
 * phy_evt.evt_params.rx_pkt_received.num_of_bytes = (number of received bytes);
 * @endcode
 * , and then the callback function has to be called:
 * @code
 * events_handler(phy_evt);
 * @endcode
 * All functions declared in this file are obligatory to implement. Some events specified in 
 * @ref ser_phy_evt_type_t are optional to implement.
 *
 * \par Transmitting a packet
 * Each PHY layer is responsible for adding the PHY header to a packet to be sent. This header
 * consists of a 16-bit field that carries the packet length (the uint16_encode function defined in
 * app_util.h should be used to ensure endianness independence). A pointer to a packet to be sent
 * and length of the packet are parameters of the @ref ser_phy_tx_pkt_send function. When a packet
 * has been transmitted, an event of type @ref SER_PHY_EVT_TX_PKT_SENT should be emitted.
 *
 * \image html ser_phy_transport_tx.svg "TX - interlayer communication"
 *
 *
 * \par Receiving a packet
 * The PHY layer should be able to store only the PHY header (16-bit field carrying the packet
 * length). After the PHY header has been received, the transmission is stopped and the PHY
 * layer has to send a request to the HAL Transport layer for memory to store the packet - an event
 * of type @ref SER_PHY_EVT_RX_BUF_REQUEST with event parameters defined in
 * @ref ser_phy_evt_rx_buf_request_params_t (the uint16_decode function defined in app_util.h should
 * be used for header decoding to ensure endianness independence). The transmission should be
 * resumed when the @ref ser_phy_rx_buf_set function has been called.
 *
 * When the @ref ser_phy_rx_buf_set function parameter equals NULL, it means that there is not
 * enough memory to store the packet, however the packet will be received to dummy location to
 * ensure continuous communication. After receiving has finished, an event of type
 * @ref SER_PHY_EVT_RX_PKT_DROPPED is generated.
 *
 * \image html ser_phy_transport_rx_dropped.svg "RX dropping - interlayer communication"
 *
 * When the @ref ser_phy_rx_buf_set function parameter is different than NULL, the packet is
 * received to a buffer pointed by it. After receiving has finished, an event of type
 * @ref SER_PHY_EVT_RX_PKT_RECEIVED is generated with event parameters defined in
 * @ref ser_phy_evt_rx_pkt_received_params_t.
 *
 * \image html ser_phy_transport_rx_received.svg "RX - interlayer communication"
 *
 *
 * \par PHY layer errors
 * PHY layer errors can be signaled by an event of type @ref SER_PHY_EVT_RX_OVERFLOW_ERROR or
 * @ref SER_PHY_EVT_TX_OVERREAD_ERROR or @ref SER_PHY_EVT_HW_ERROR with event parameters defined in
 * @ref ser_phy_evt_hw_error_params_t.
 *
 */

#ifndef SER_PHY_H__
#define SER_PHY_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Serialization PHY module event types. */
typedef enum
{
    SER_PHY_EVT_TX_PKT_SENT = 0,   /**< Obligatory to implement. An event indicating that a TX packet
                                    *   has been transmitted. */
    SER_PHY_EVT_RX_BUF_REQUEST,    /**< Obligatory to implement. An event indicating that the PHY layer
                                    *   needs a buffer for an RX packet. The PHY flow should be blocked
                                    *   until the @ref ser_phy_rx_buf_set function is called. */
    SER_PHY_EVT_RX_PKT_RECEIVED,   /**< Obligatory to implement. An event indicating that an RX packet
                                    *   has been successfully received. */
    SER_PHY_EVT_RX_PKT_DROPPED,    /**< Obligatory to implement. An event indicating that the RX packet
                                    *   receiving has been finished but the packet was discarded because
                                    *   it was longer than available the buffer. */

    SER_PHY_EVT_RX_OVERFLOW_ERROR, /**< Optional to implement. An event indicating that more
                                    *   information has been transmitted than the PHY module could
                                    *   handle. */
    SER_PHY_EVT_TX_OVERREAD_ERROR, /**< Optional to implement. An event indicating that the PHY module
                                    *   was forced to transmit more information than possessed. */
    SER_PHY_EVT_HW_ERROR,          /**< Optional to implement. An event indicating a hardware error
                                    *   in the PHY module.  */
    SER_PHY_EVT_TYPE_MAX           /**< Enumeration upper bound. */
} ser_phy_evt_type_t;


/**@brief A struct containing parameters of event of type @ref SER_PHY_EVT_RX_BUF_REQUEST. */
typedef struct
{
    uint16_t num_of_bytes;  /**< Length of a buffer in octets that the layer above the PHY module should
                             *   deliver, so that the PHY module can receive a packet. */
} ser_phy_evt_rx_buf_request_params_t;


/**@brief A struct containing parameters of event of type @ref SER_PHY_EVT_RX_PKT_RECEIVED. */
typedef struct
{
    uint8_t * p_buffer;     /**< Pointer to a buffer containing the received packet. */
    uint16_t  num_of_bytes; /**< Length of the received packet in octets. */
} ser_phy_evt_rx_pkt_received_params_t;


/**@brief A struct containing parameters of event of type @ref SER_PHY_EVT_HW_ERROR. */
typedef struct
{
    uint32_t error_code; /**< Hardware error code - specific for a microcontroller. */
    uint8_t * p_buffer;  /**< Pointer to the buffer that was processed when error occured. */
} ser_phy_evt_hw_error_params_t;


/**@brief A struct containing events from a Serialization PHY module.
 *
 * @note  Some events do not have parameters, then whole information is contained in the evt_type.
 */
typedef struct
{
    ser_phy_evt_type_t                       evt_type; /**< Type of event. */

    union  /**< Union alternative identified by evt_type in enclosing struct. */
    {
        /** Parameters of event of type @ref SER_PHY_EVT_RX_BUF_REQUEST. */
        ser_phy_evt_rx_buf_request_params_t  rx_buf_request;
        /** Parameters of event of type @ref SER_PHY_EVT_RX_PKT_RECEIVED. */
        ser_phy_evt_rx_pkt_received_params_t rx_pkt_received;
        /** Parameters of the event of type @ref SER_PHY_EVT_HW_ERROR. */
        ser_phy_evt_hw_error_params_t        hw_error;
    } evt_params;
} ser_phy_evt_t;


/**@brief A type of generic callback function handler to be used by all PHY module events.
 *
 * @param[in] event    Serialization PHY module event.
 */
typedef void (*ser_phy_events_handler_t)(ser_phy_evt_t event);


/**@brief Function for opening and initializing the PHY module.
 *
 * @note  The function initializes hardware and internal module states, and registers callback
 *        function to be used by all PHY module events.
 *
 * @warning If the function has been already called, the function @ref ser_phy_close has to be
 *          called before ser_phy_open can be called again.
 *
 * @param[in] events_handler    Generic callback function handler to be used by all PHY module
 *                              events.
 *
 * @retval NRF_SUCCESS                Operation success.
 * @retval NRF_ERROR_INVALID_STATE    Operation failure. The function has been already called.
 *                                    To call it again, the function @ref ser_phy_close has to be
 *                                    called first.
 * @retval NRF_ERROR_NULL             Operation failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_PARAM    Operation failure. Hardware initialization parameters are not
 *                                    supported.
 */
uint32_t ser_phy_open(ser_phy_events_handler_t events_handler);


/**@brief Function for transmitting a packet.
 *
 * @note  The function adds a packet pointed by p_buffer parameter to a transmission queue and
 *        schedules generation of an event of type @ref SER_PHY_EVT_TX_PKT_SENT upon transmission
 *        completion.
 *
 * @param[in] p_buffer        Pointer to a buffer to transmit.
 * @param[in] num_of_bytes    Number of octets to transmit. Must be more than 0.
 *
 * @retval NRF_SUCCESS                Operation success. Packet was added to the transmission queue
 *                                    and event will be send upon transmission completion.
 * @retval NRF_ERROR_NULL             Operation failure. NULL pointer supplied.
 * @retval NRF_ERROR_INVALID_PARAM    Operation failure. The num_of_bytes parameter equal to 0.
 * @retval NRF_ERROR_BUSY             Operation failure. Transmitting of a packet in progress.
 */
uint32_t ser_phy_tx_pkt_send(const uint8_t * p_buffer, uint16_t num_of_bytes);


/**@brief Function for setting an RX buffer and enabling reception of data (the PHY flow).
 *
 * @note The function has to be called as a response to an event of type
 *       @ref SER_PHY_EVT_RX_BUF_REQUEST. The function sets an RX buffer and enables reception of
 *       data (enables the PHY flow).
 *       Size of a buffer pointed by the p_buffer parameter should be at least equal to the
 *       num_of_bytes parameter passed within the event (@ref ser_phy_evt_rx_buf_request_params_t),
 *       or p_buffer should be equal to NULL if there is not enough memory.
 *       When p_buffer is different from NULL and num_of_bytes octets have been received, an event of
 *       type @ref SER_PHY_EVT_RX_PKT_RECEIVED is generated
 *       (@ref ser_phy_evt_rx_pkt_received_params_t).
 *       When p_buffer is equal to NULL, data is received to dummy location to ensure continuous
 *       communication. Then, if num_of_bytes octets have been received, an event of type
 *       @ref SER_PHY_EVT_RX_PKT_DROPPED is generated.
 *
 * @param[in] p_buffer    Pointer to an RX buffer in which to receive.
 *
 * @retval NRF_SUCCESS                Operation success.
 * @retval NRF_ERROR_INVALID_STATE    Operation failure. A buffer was set without request.
 */
uint32_t ser_phy_rx_buf_set(uint8_t * p_buffer);


/**@brief Function for closing the PHY module.
 *
 * @note  The function disables hardware, resets internal module states, and unregisters the events
 *        callback function.
 */
void ser_phy_close(void);


/**@brief Function for enabling the PHY module interrupts.
 *
 * @note  The function enables all interrupts that are used by the PHY module (and only those).
 */
void ser_phy_interrupts_enable(void);


/**@brief Function for disabling the PHY module interrupts.
 *
 * @note  The function disables all interrupts that are used by the PHY module (and only those).
 */
void ser_phy_interrupts_disable(void);



#ifdef __cplusplus
}
#endif

#endif /* SER_PHY_H__ */
/** @} */
