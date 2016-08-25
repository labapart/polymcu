/*
 * Copyright (c) 2015-2016, Lab A Part
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

#include <assert.h>
#include "board.h"
#include "PolyMCU.h"
#include "Driver_USART.h"
#ifdef CHIP_LPC11UXX
  #include "uart_11xx.h"
#elif CHIP_LPC11U6X
  #include "uart_0_11u6x.h"
#elif CHIP_LPC175X_6X
  #include "uart_17xx_40xx.h"
#else
  #error "Chip not recognized"
#endif

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)  /* driver version */

#ifdef CHIP_LPC11UXX
  #define LPC_UART      LPC_USART
#elif CHIP_LPC11U6X
  #define LPC_UART      LPC_USART0

  #define Chip_UART_Init           Chip_UART0_Init
  #define Chip_UART_ConfigData     Chip_UART0_ConfigData
  #define Chip_UART_SetBaud        Chip_UART0_SetBaud
  #define Chip_UART_TXEnable       Chip_UART0_TXEnable
  #define Chip_UART_SendBlocking   Chip_UART0_SendBlocking
  #define Chip_UART_Read           Chip_UART0_Read
  #define Chip_UART_ReadLineStatus Chip_UART0_ReadLineStatus
  #define Chip_UART_DeInit         Chip_UART0_DeInit

  #define UART_LCR_WLEN8           UART0_LCR_WLEN8
  #define UART_LCR_SBS_1BIT        UART0_LCR_SBS_1BIT
  #define UART_LCR_PARITY_DIS      UART0_LCR_PARITY_DIS
  #define UART_LCR_WLEN5           UART0_LCR_WLEN5
  #define UART_LCR_WLEN6           UART0_LCR_WLEN6
  #define UART_LCR_WLEN7           UART0_LCR_WLEN7
  #define UART_LCR_WLEN8           UART0_LCR_WLEN8
  #define UART_LCR_PARITY_DIS      UART0_LCR_PARITY_DIS
  #define UART_LCR_PARITY_EN       UART0_LCR_PARITY_EN
  #define UART_LCR_PARITY_EVEN     UART0_LCR_PARITY_EVEN
  #define UART_LCR_PARITY_ODD      UART0_LCR_PARITY_ODD
  #define UART_LCR_SBS_1BIT        UART0_LCR_SBS_1BIT
  #define UART_LCR_SBS_2BIT        UART0_LCR_SBS_2BIT
  #define UART_LSR_TEMT            UART0_LSR_TEMT
  #define UART_LSR_RDR             UART0_LSR_RDR
  #define UART_LSR_OE              UART0_LSR_OE
  #define UART_LSR_BI              UART0_LSR_BI
  #define UART_LSR_FE              UART0_LSR_FE
  #define UART_LSR_PE              UART0_LSR_PE

#elif CHIP_LPC175X_6X
  #define LPC_UART      LPC_UART0
#endif

void _ttywrch(int ch);

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

int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event) {
#ifdef CHIP_LPC11UXX
	// PinMux
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18, IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_18 used for RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 19, IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_19 used for TXD */
#elif CHIP_LPC11U6X
	// PinMux
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 13, IOCON_FUNC5 | IOCON_MODE_INACT);	/* PIO0_13 used for RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 14, IOCON_FUNC5 | IOCON_MODE_INACT);	/* PIO0_14 used for TXD */
#endif

	Chip_UART_Init(LPC_UART);
#ifdef DEBUG_UART_BAUDRATE
	Chip_UART_SetBaud(LPC_UART, DEBUG_UART_BAUDRATE);
#else
	Chip_UART_SetBaud(LPC_UART, 115200);
#endif
	Chip_UART_ConfigData(LPC_UART, UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);

	// Enable UART Transmit
	Chip_UART_TXEnable(LPC_UART);

	m_SignalEvent = cb_event;

	return ARM_DRIVER_OK;
}

int32_t ARM_USART_Uninitialize(void) {
	Chip_UART_DeInit(LPC_UART);
	return ARM_DRIVER_OK;
}

int32_t ARM_USART_PowerControl(ARM_POWER_STATE state) {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t ARM_USART_Send(const void *data, uint32_t num) {
	const char *ptr = data; int len = num;
	int32_t sent = Chip_UART_SendBlocking(LPC_UART, data, num);

	// Ensure we return carriage
	if ((len >= 2) && (ptr[len-1] == '\n') && (ptr[len-2] != '\r')) {
		_ttywrch('\r');
	}

	if (m_SignalEvent) {
		m_SignalEvent(ARM_USART_EVENT_SEND_COMPLETE);
	}
	return sent;
}

int32_t ARM_USART_Receive(void *data, uint32_t num) {
	int32_t ret = Chip_UART_Read(LPC_UART, data, num);

	if (m_SignalEvent) {
		m_SignalEvent(ARM_USART_EVENT_RECEIVE_COMPLETE);
	}
	return ret;
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
	int32_t status = ARM_DRIVER_OK;
	uint32_t lcr = 0;

	// Set baud rate
	Chip_UART_SetBaud(LPC_UART, arg);

	switch (control & ARM_USART_DATA_BITS_Msk) {
	case ARM_USART_DATA_BITS_5:
		lcr |= UART_LCR_WLEN5;
		break;
	case ARM_USART_DATA_BITS_6:
		lcr |= UART_LCR_WLEN6;
		break;
	case ARM_USART_DATA_BITS_7:
		lcr |= UART_LCR_WLEN7;
		break;
	case ARM_USART_DATA_BITS_8:
		lcr |= UART_LCR_WLEN8;
		break;
	default:
		status = ARM_DRIVER_ERROR_UNSUPPORTED;
		DEBUG_NOT_SUPPORTED();
		break;
	}

	switch (control & ARM_USART_PARITY_Msk) {
	case ARM_USART_PARITY_NONE:
		lcr |= UART_LCR_PARITY_DIS;
		break;
	case ARM_USART_PARITY_EVEN:
		lcr |= UART_LCR_PARITY_EN | UART_LCR_PARITY_EVEN;
		break;
	case ARM_USART_PARITY_ODD:
		lcr |= UART_LCR_PARITY_EN | UART_LCR_PARITY_ODD;
		break;
    }

	switch (control & ARM_USART_STOP_BITS_Msk) {
	case ARM_USART_STOP_BITS_1:
		lcr |= UART_LCR_SBS_1BIT;
		break;
	case ARM_USART_STOP_BITS_2:
		lcr |= UART_LCR_SBS_2BIT;
		break;
	default:
		status = ARM_DRIVER_ERROR_UNSUPPORTED;
		DEBUG_NOT_SUPPORTED();
		break;
	}

	if (control & ARM_USART_CONTROL_Msk) {
		assert (ARM_DRIVER_ERROR_UNSUPPORTED);
	}

	if (control & ARM_USART_FLOW_CONTROL_Msk) {
		assert (ARM_DRIVER_ERROR_UNSUPPORTED);
	}

	Chip_UART_ConfigData(LPC_UART, lcr);

	return status;
}

ARM_USART_STATUS ARM_USART_GetStatus(void) {
	ARM_USART_STATUS status = { 0 };
	uint32_t line_status = Chip_UART_ReadLineStatus(LPC_UART);

	// If the UART transmitter is not empty then it is busy
	if ((line_status & UART_LSR_TEMT) == 0) {
		status.tx_busy = 1;
	}

	// If the UART Transmitter is not ready then it is busy
	if ((line_status & UART_LSR_RDR) == 0) {
		status.rx_busy = 1;
	}

	// Check Overrun error
	if (line_status & UART_LSR_OE) {
		status.rx_overflow = 1;
	}

	// Check Break interrupt
	if (line_status & UART_LSR_BI) {
		status.rx_break = 1;
	}

	// Check Framing error
	if (line_status & UART_LSR_FE) {
		status.rx_framing_error = 1;
	}

	// Check Parity error
	if (line_status & UART_LSR_PE) {
		status.rx_parity_error = 1;
	}

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
