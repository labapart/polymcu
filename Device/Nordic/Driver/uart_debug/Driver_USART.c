/*
 * Copyright (c) 2015, Lab A Part
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * o list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "PolyMCU.h"
#include "Driver_USART.h"
#include "boards.h"
#include "app_error.h"
#include "app_uart.h"

#ifdef NORDIC_UART_FIFO
  #define UART_TX_BUF_SIZE           256                                /**< UART TX buffer size. */
  #define UART_RX_BUF_SIZE           1                                  /**< UART RX buffer size. */
#endif

static const app_uart_comm_params_t comm_params = {
	RX_PIN_NUMBER,
	TX_PIN_NUMBER,
	RTS_PIN_NUMBER,
	CTS_PIN_NUMBER,
	APP_UART_FLOW_CONTROL_DISABLED,
	false,
	UART_BAUDRATE_BAUDRATE_Baud115200
	};

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)  /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    1, /* supports UART (Asynchronous) mode */
    0, /* supports Synchronous Master mode */
    0, /* supports Synchronous Slave mode */
    0, /* supports UART Single-wire mode */
    0, /* supports UART IrDA mode */
    0, /* supports UART Smart Card mode */
    0, /* Smart Card Clock generator available */
    0, /* RTS Flow Control available */
    0, /* CTS Flow Control available */
    0, /* Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE */
    0, /* Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT */
    0, /* RTS Line: 0=not available, 1=available */
    0, /* CTS Line: 0=not available, 1=available */
    0, /* DTR Line: 0=not available, 1=available */
    0, /* DSR Line: 0=not available, 1=available */
    0, /* DCD Line: 0=not available, 1=available */
    0, /* RI Line: 0=not available, 1=available */
    0, /* Signal CTS change event: \ref ARM_USART_EVENT_CTS */
    0, /* Signal DSR change event: \ref ARM_USART_EVENT_DSR */
    0, /* Signal DCD change event: \ref ARM_USART_EVENT_DCD */
    0  /* Signal RI change event: \ref ARM_USART_EVENT_RI */
};

static ARM_USART_SignalEvent_t m_SignalEvent;

//
//   Functions
//

ARM_DRIVER_VERSION ARM_USART_GetVersion(void) {
	return DriverVersion;
}

ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void) {
	return DriverCapabilities;
}

void uart_error_handle(app_uart_evt_t * p_event) {
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event) {
	uint32_t err_code;

	m_SignalEvent = cb_event;

#if NORDIC_UART_FIFO
	APP_UART_FIFO_INIT(&comm_params,
					UART_RX_BUF_SIZE,
					UART_TX_BUF_SIZE,
					uart_error_handle,
					APP_IRQ_PRIORITY_LOW,
					err_code);
#else
	APP_UART_INIT(&comm_params,
					uart_error_handle,
					APP_IRQ_PRIORITY_LOW,
					err_code);
#endif

	if (err_code != NRF_SUCCESS) {
		set_led(0, 1);
		return ARM_DRIVER_ERROR;
	} else {
		return ARM_DRIVER_OK;
	}
}

int32_t ARM_USART_Uninitialize(void) {
    uint32_t err_code = app_uart_close();
    if (err_code != NRF_SUCCESS) {
    	return ARM_DRIVER_ERROR;
    } else {
    	return ARM_DRIVER_OK;
    }
}

int32_t ARM_USART_PowerControl(ARM_POWER_STATE state) {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t ARM_USART_Send(const void *data, uint32_t num) {
	int i;
	const char* p_char = data;
	uint32_t err_code;

	for (i = 0; i < num; i++) {
		do {
			err_code = app_uart_put(*p_char);
		} while (err_code != NRF_SUCCESS);
		p_char++;
	}

	// Add a return carriage
	if (*(p_char - 1) == '\n') {
		do {
			err_code = app_uart_put('\r');
		} while (err_code != NRF_SUCCESS);
	}

	if (m_SignalEvent) {
		m_SignalEvent(ARM_USART_EVENT_SEND_COMPLETE);
	}
	return num;
}

int32_t ARM_USART_Receive(void *data, uint32_t num) {
	int i;
	const char* p_char = data;
	uint32_t err_code;

	for (i = 0; i < num; i++) {
		do {
			err_code = app_uart_get((uint8_t *)p_char);
		} while (err_code != NRF_SUCCESS);
		p_char++;
	}

	if (m_SignalEvent) {
		m_SignalEvent(ARM_USART_EVENT_RECEIVE_COMPLETE);
	}
	return num;
}

int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num) {
	return ARM_DRIVER_ERROR_UNSUPPORTED;
}

uint32_t ARM_USART_GetTxCount(void) {
	DEBUG_NOT_IMPLEMENTED();
	return 0;
}

uint32_t ARM_USART_GetRxCount(void) {
	DEBUG_NOT_IMPLEMENTED();
	return 0;
}

int32_t ARM_USART_Control(uint32_t control, uint32_t arg) {
	return ARM_DRIVER_ERROR_UNSUPPORTED;
}

ARM_USART_STATUS ARM_USART_GetStatus(void) {
	ARM_USART_STATUS status = { 0 };
	DEBUG_NOT_IMPLEMENTED();
	return status;
}

int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control) {
	return ARM_DRIVER_ERROR_UNSUPPORTED;
}

ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(void) {
	ARM_USART_MODEM_STATUS status = { 0 };
	DEBUG_NOT_IMPLEMENTED();
	return status;
}

void ARM_USART_SignalEvent(uint32_t event) {
	DEBUG_NOT_IMPLEMENTED();
}

// End USART Interface

const ARM_DRIVER_USART Driver_UART_DEBUG = {
    ARM_USART_GetVersion,
    ARM_USART_GetCapabilities,
    ARM_USART_Initialize,
    ARM_USART_Uninitialize,
    ARM_USART_PowerControl,
    ARM_USART_Send,
    ARM_USART_Receive,
    ARM_USART_Transfer,
    ARM_USART_GetTxCount,
    ARM_USART_GetRxCount,
    ARM_USART_Control,
    ARM_USART_GetStatus,
    ARM_USART_SetModemControl,
    ARM_USART_GetModemStatus
};
