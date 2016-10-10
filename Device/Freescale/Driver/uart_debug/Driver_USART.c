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

#include "board.h"
#include "Driver_USART.h"

#if FREESCALE_DEBUG_UART
  #if defined(FSL_FEATURE_SOC_UART_COUNT) && (FSL_FEATURE_SOC_UART_COUNT > 0)
    #include "fsl_uart.h"
  #else
    #error UART not supported for this MCU
  #endif /* FSL_FEATURE_SOC_UART_COUNT */
#endif

#if FREESCALE_DEBUG_LPSCI
  #if defined(FSL_FEATURE_SOC_LPSCI_COUNT) && (FSL_FEATURE_SOC_LPSCI_COUNT > 0)
    #include "fsl_lpsci.h"
  #else
    #error LPSCI not supported for this MCU
  #endif /* FSL_FEATURE_SOC_LPSCI_COUNT */
#endif

#if FREESCALE_DEBUG_LPUART
  #if defined(FSL_FEATURE_SOC_LPUART_COUNT) && (FSL_FEATURE_SOC_LPUART_COUNT > 0)
    #include "fsl_lpuart.h"
  #else
    #error LPUART not supported for this MCU
  #endif /* FSL_FEATURE_SOC_LPUART_COUNT */
#endif

#include "PolyMCU.h"

#ifndef DEBUG_UART_BAUDRATE
  #define DEBUG_UART_BAUDRATE 115200
#endif

#define WAIT_FOREVER 0xFFFFFFFFU

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0)  /* driver version */

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

#if FREESCALE_DEBUG_UART
  #error UART not supported yet
#endif

#if FREESCALE_DEBUG_LPSCI
/* Array of LPSCI peripheral base address. */
static UART0_Type *const g_lpsci_bases[] = UART0_BASE_PTRS;

// The data section is not initialized when retarget_init() is called.
// That's why this structure must be declared as 'static const'
static const lpsci_config_t m_lpsciInitialConfig = {
	DEBUG_UART_BAUDRATE,   /*!< LPSCI baud rate  */
	kLPSCI_ParityDisabled, /*!< Parity mode, disabled (default), even, odd */
#if defined(FSL_FEATURE_LPSCI_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_LPSCI_HAS_STOP_BIT_CONFIG_SUPPORT
	kLPSCI_OneStopBit,     /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
#endif
	1,                     /*!< Enable TX */
	1,                     /*!< Enable RX */
};

static lpsci_config_t m_lpsciUserConfig = {
	DEBUG_UART_BAUDRATE,   /*!< LPSCI baud rate  */
	kLPSCI_ParityDisabled, /*!< Parity mode, disabled (default), even, odd */
#if defined(FSL_FEATURE_LPSCI_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_LPSCI_HAS_STOP_BIT_CONFIG_SUPPORT
	kLPSCI_OneStopBit,     /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
#endif
	1,                     /*!< Enable TX */
	1,                     /*!< Enable RX */
};
#endif

#if FREESCALE_DEBUG_LPUART
/* Array of LPSCI peripheral base address. */
static LPUART_Type *const g_lpuart_bases[] = LPUART_BASE_PTRS;

// The data section is not initialized when retarget_init() is called.
// That's why this structure must be declared as 'static const'
static const lpuart_config_t m_lpuartInitialConfig = {
	DEBUG_UART_BAUDRATE,    /*!< LPUART baud rate  */
	kLPUART_ParityDisabled, /*!< Parity mode, disabled (default), even, odd */
#if defined(FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT
	kLPUART_OneStopBit,     /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
#endif
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    0,                     /*!< TX FIFO watermark */
    0,                     /*!< RX FIFO watermark */
#endif
	1,                     /*!< Enable TX */
	1,                     /*!< Enable RX */
};

static lpuart_config_t m_lpuartUserConfig = {
	DEBUG_UART_BAUDRATE,    /*!< LPUART baud rate  */
	kLPUART_ParityDisabled, /*!< Parity mode, disabled (default), even, odd */
#if defined(FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT
	kLPUART_OneStopBit,     /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
#endif
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
	0,                     /*!< TX FIFO watermark */
	0,                     /*!< RX FIFO watermark */
#endif
	1,                     /*!< Enable TX */
	1,                     /*!< Enable RX */
};
#endif

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
#if FREESCALE_DEBUG_UART
	#error UART not supported yet
#endif

#if FREESCALE_DEBUG_LPSCI
	status_t status;

	assert(BOARD_DEBUG_UART_INSTANCE < FSL_FEATURE_SOC_LPSCI_COUNT);

	m_SignalEvent = cb_event;

	/* SIM_SOPT2[27:26]:
	 *  00: Clock Disabled
	 *  01: IRC48M
	 *  10: OSCERCLK
	 *  11: MCGIRCCLK
	 */
	CLOCK_SetLpsci0Clock(1);

    // Enable clock and initial UART module follow user configure structure.
    status = LPSCI_Init(g_lpsci_bases[BOARD_DEBUG_UART_INSTANCE], &m_lpsciInitialConfig, BOARD_DEBUG_UART_FREQUENCY);
	if (status == kStatus_Success) {
	    LPSCI_EnableTx(g_lpsci_bases[BOARD_DEBUG_UART_INSTANCE], true);
	    LPSCI_EnableRx(g_lpsci_bases[BOARD_DEBUG_UART_INSTANCE], true);

	    return ARM_DRIVER_OK;
	} else {
		return ARM_DRIVER_ERROR_UNSUPPORTED;
	}
#endif

#if FREESCALE_DEBUG_LPUART
	assert(BOARD_DEBUG_UART_INSTANCE < FSL_FEATURE_SOC_LPUART_COUNT);

	m_SignalEvent = cb_event;

	/* SIM_SOPT2[27:26]:
	 *  00: Clock Disabled
	 *  01: IRC48M
	 *  10: OSCERCLK
	 *  11: MCGIRCCLK
	 */
#if BOARD_DEBUG_UART_INSTANCE == 0
	CLOCK_SetLpuart0Clock(1);
#elif BOARD_DEBUG_UART_INSTANCE == 1
	CLOCK_SetLpuart1Clock(1);
#endif

	LPUART_Init(g_lpuart_bases[BOARD_DEBUG_UART_INSTANCE], &m_lpuartInitialConfig, BOARD_DEBUG_UART_FREQUENCY);
	LPUART_EnableTx(g_lpuart_bases[BOARD_DEBUG_UART_INSTANCE], true);
	LPUART_EnableRx(g_lpuart_bases[BOARD_DEBUG_UART_INSTANCE], true);

	return ARM_DRIVER_OK;
#endif
}

int32_t ARM_USART_Uninitialize(void) {
#if FREESCALE_DEBUG_UART
	#error UART not supported yet
#endif

#if FREESCALE_DEBUG_LPSCI
	LPSCI_Deinit(g_lpsci_bases[BOARD_DEBUG_UART_INSTANCE]);
#endif

#if FREESCALE_DEBUG_LPUART
	LPUART_Deinit(g_lpuart_bases[BOARD_DEBUG_UART_INSTANCE]);
#endif

	return ARM_DRIVER_OK;
}

int32_t ARM_USART_PowerControl(ARM_POWER_STATE state) {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t ARM_USART_Send(const void *data, uint32_t num) {
	const char *ptr = data;

#if FREESCALE_DEBUG_UART
	#error UART not supported yet
#endif

#if FREESCALE_DEBUG_LPSCI
	LPSCI_WriteBlocking(g_lpsci_bases[BOARD_DEBUG_UART_INSTANCE], data, num);
#endif

#if FREESCALE_DEBUG_LPUART
	LPUART_WriteBlocking(g_lpuart_bases[BOARD_DEBUG_UART_INSTANCE], data, num);
#endif

	// Ensure we return carriage
	if ((num >= 2) && (ptr[num-1] == '\n') && (ptr[num-2] != '\r')) {
		_ttywrch('\r');
	}

	if (m_SignalEvent) {
		m_SignalEvent(ARM_USART_EVENT_SEND_COMPLETE);
	}
	return num;
}

int32_t ARM_USART_Receive(void *data, uint32_t num) {
	status_t status;

#if FREESCALE_DEBUG_UART
	#error UART not supported yet
#endif

#if FREESCALE_DEBUG_LPSCI
	status = LPSCI_ReadBlocking(g_lpsci_bases[BOARD_DEBUG_UART_INSTANCE], data, num);
#endif

#if FREESCALE_DEBUG_LPUART
	status = LPUART_ReadBlocking(g_lpuart_bases[BOARD_DEBUG_UART_INSTANCE], data, num);
#endif

	if (status == kStatus_Success) {
		if (m_SignalEvent) {
			m_SignalEvent(ARM_USART_EVENT_RECEIVE_COMPLETE);
		}
		return ARM_DRIVER_OK;
	} else {
		return ARM_DRIVER_ERROR_UNSUPPORTED;
	}
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
#if FREESCALE_DEBUG_UART
	#error UART not supported yet
#endif

#if FREESCALE_DEBUG_LPSCI
	int32_t status = ARM_DRIVER_OK;

	// Set baud rate
	m_lpsciUserConfig.baudRate_Bps = arg;

	switch (control & ARM_USART_DATA_BITS_Msk) {
	//OM:case ARM_USART_DATA_BITS_8:
	//OM:	m_lpsciUserConfig.bitCountPerChar = kLPSCI_8BitsPerChar;
	//OM:	break;
	//OM:case ARM_USART_DATA_BITS_9:
	//OM:	m_lpsciUserConfig.bitCountPerChar = kLPSCI_9BitsPerChar;
	//OM:	break;
	default:
		status = ARM_DRIVER_ERROR_UNSUPPORTED;
		DEBUG_NOT_SUPPORTED();
		break;
	}

	switch (control & ARM_USART_PARITY_Msk) {
	case ARM_USART_PARITY_NONE:
		m_lpsciUserConfig.parityMode = kLPSCI_ParityDisabled;
		break;
	case ARM_USART_PARITY_EVEN:
		m_lpsciUserConfig.parityMode = kLPSCI_ParityEven;
		break;
	case ARM_USART_PARITY_ODD:
		m_lpsciUserConfig.parityMode = kLPSCI_ParityOdd;
		break;
    }

	switch (control & ARM_USART_STOP_BITS_Msk) {
	case ARM_USART_STOP_BITS_1:
		m_lpsciUserConfig.stopBitCount = kLPSCI_OneStopBit;
		break;
	case ARM_USART_STOP_BITS_2:
		m_lpsciUserConfig.stopBitCount = kLPSCI_TwoStopBit;
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

	return status;
#endif

#if FREESCALE_DEBUG_LPUART
	int32_t status = ARM_DRIVER_OK;

	// Set baud rate
	m_lpuartUserConfig.baudRate_Bps = arg;

	switch (control & ARM_USART_DATA_BITS_Msk) {
	//OM:case ARM_USART_DATA_BITS_8:
	//OM:	m_lpuartUserConfig.bitCountPerChar = kLPUART_8BitsPerChar;
	//OM:	break;
	//OM:case ARM_USART_DATA_BITS_9:
	//OM:	m_lpuartUserConfig.bitCountPerChar = kLPUART_9BitsPerChar;
	//OM:	break;
	default:
		status = ARM_DRIVER_ERROR_UNSUPPORTED;
		DEBUG_NOT_SUPPORTED();
		break;
	}

	switch (control & ARM_USART_PARITY_Msk) {
	case ARM_USART_PARITY_NONE:
		m_lpuartUserConfig.parityMode = kLPUART_ParityDisabled;
		break;
	case ARM_USART_PARITY_EVEN:
		m_lpuartUserConfig.parityMode = kLPUART_ParityEven;
		break;
	case ARM_USART_PARITY_ODD:
		m_lpuartUserConfig.parityMode = kLPUART_ParityOdd;
		break;
    }

	switch (control & ARM_USART_STOP_BITS_Msk) {
	case ARM_USART_STOP_BITS_1:
		m_lpuartUserConfig.stopBitCount = kLPUART_OneStopBit;
		break;
	case ARM_USART_STOP_BITS_2:
		m_lpuartUserConfig.stopBitCount = kLPUART_TwoStopBit;
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

	return status;
#endif
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
