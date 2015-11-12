/*
 * @brief Virtual Comm port call back routines
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include <stdlib.h>
#include <string.h>

#include "Driver_USART.h"
#include "board.h"
#include "PolyMCU.h"
#include "USBD_ROM.h"

void _ttywrch(int ch);

#define VCOM_RX_BUF_SZ      512
#define VCOM_TX_CONNECTED   _BIT(8)		/* connection state is for both RX/Tx */
#define VCOM_TX_BUSY        _BIT(0)
#define VCOM_RX_DONE        _BIT(0)
#define VCOM_RX_BUF_FULL    _BIT(1)
#define VCOM_RX_BUF_QUEUED  _BIT(2)
#define VCOM_RX_DB_QUEUED   _BIT(3)

/**
 * Structure containing Virtual Comm port control data
 */
typedef struct VCOM_DATA {
	USBD_HANDLE_T hUsb;
	USBD_HANDLE_T hCdc;
	uint8_t *rx_buff;
	uint16_t rx_rd_count;
	uint16_t rx_count;
	volatile uint16_t tx_flags;
	volatile uint16_t rx_flags;
} VCOM_DATA_T;

/**
 * Global variable to hold Virtual COM port control data.
 */
static VCOM_DATA_T g_vCOM;

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0)  /* driver version */

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

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* VCOM bulk EP_IN endpoint handler */
static ErrorCode_t VCOM_bulk_in_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event) {
	VCOM_DATA_T *pVcom = (VCOM_DATA_T *) data;

	if (event == USB_EVT_IN) {
		pVcom->tx_flags &= ~VCOM_TX_BUSY;
	}
	return LPC_OK;
}

/* VCOM bulk EP_OUT endpoint handler */
static ErrorCode_t VCOM_bulk_out_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event) {
	VCOM_DATA_T *pVcom = (VCOM_DATA_T *) data;

	switch (event) {
	case USB_EVT_OUT:
		pVcom->rx_count = USBD_API->hw->ReadEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff);
		if (pVcom->rx_flags & VCOM_RX_BUF_QUEUED) {
			pVcom->rx_flags &= ~VCOM_RX_BUF_QUEUED;
			if (pVcom->rx_count != 0) {
				pVcom->rx_flags |= VCOM_RX_BUF_FULL;
			}

		}
		else if (pVcom->rx_flags & VCOM_RX_DB_QUEUED) {
			pVcom->rx_flags &= ~VCOM_RX_DB_QUEUED;
			pVcom->rx_flags |= VCOM_RX_DONE;
		}
		break;

	case USB_EVT_OUT_NAK:
		/* queue free buffer for RX */
		if ((pVcom->rx_flags & (VCOM_RX_BUF_FULL | VCOM_RX_BUF_QUEUED)) == 0) {
			USBD_API->hw->ReadReqEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff, VCOM_RX_BUF_SZ);
			pVcom->rx_flags |= VCOM_RX_BUF_QUEUED;
		}
		break;

	default:
		break;
	}

	return LPC_OK;
}

/* Set line coding call back routine */
static ErrorCode_t VCOM_SetLineCode(USBD_HANDLE_T hCDC, CDC_LINE_CODING *line_coding)
{
	VCOM_DATA_T *pVcom = &g_vCOM;

	if(!USB_IsConfigured(pVcom->hUsb)) {
		return ERR_BUSY;
	}

	/* Called when baud rate is changed/set. Using it to know host connection state */
	pVcom->tx_flags = VCOM_TX_CONNECTED;    /* reset other flags */


#if 0

	if(!(pVcom->tx_flags & VCOM_TX_CONNECTED) == 0) {
		for(int i = 0; i < TX_BUFFERS_COUNT; i++) {
			vcom.txbuf[i].busy = false;
			if( (vcom.txbuf[i].count > TX_SEND_TRESHOLD)
					|| (memchr(vcom.txbuf[i].buf, '\n', vcom.txbuf[i].count) != NULL) )
			{
				vcom.txbuf[i].busy = true;
				USBD_API->hw->WriteEP(vcom.hUsb, USB_CDC_IN_EP, vcom.txbuf[i].buf, vcom.txbuf[i].count);
				break;
			}
		}

		/* Called when baud rate is changed/set. Using it to know host connection state */
		pVcom->tx_flags = VCOM_TX_CONNECTED;	/* reset other flags */
	}
#endif
	return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Virtual com port init routine */
ErrorCode_t vcom_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam)
{
	USBD_CDC_INIT_PARAM_T cdc_param;
	ErrorCode_t ret = LPC_OK;
	uint32_t ep_indx;

	g_vCOM.hUsb = hUsb;
	memset((void *) &cdc_param, 0, sizeof(USBD_CDC_INIT_PARAM_T));
	cdc_param.mem_base = pUsbParam->mem_base;
	cdc_param.mem_size = pUsbParam->mem_size;
	cdc_param.cif_intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc, CDC_COMMUNICATION_INTERFACE_CLASS);
	cdc_param.dif_intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc, CDC_DATA_INTERFACE_CLASS);
	cdc_param.SetLineCode = VCOM_SetLineCode;

	ret = USBD_API->cdc->init(hUsb, &cdc_param, &g_vCOM.hCdc);

	if (ret == LPC_OK) {
		/* allocate transfer buffers */
		g_vCOM.rx_buff = (uint8_t *) cdc_param.mem_base;
		cdc_param.mem_base += VCOM_RX_BUF_SZ;
		cdc_param.mem_size -= VCOM_RX_BUF_SZ;

		/* register endpoint interrupt handler */
		ep_indx = (((USB_CDC_IN_EP & 0x0F) << 1) + 1);
		ret = USBD_API->core->RegisterEpHandler(hUsb, ep_indx, VCOM_bulk_in_hdlr, &g_vCOM);
		if (ret == LPC_OK) {
			/* register endpoint interrupt handler */
			ep_indx = ((USB_CDC_OUT_EP & 0x0F) << 1);
			ret = USBD_API->core->RegisterEpHandler(hUsb, ep_indx, VCOM_bulk_out_hdlr, &g_vCOM);

		}
		/* update mem_base and size variables for cascading calls. */
		pUsbParam->mem_base = cdc_param.mem_base;
		pUsbParam->mem_size = cdc_param.mem_size;
	}

	return ret;
}

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
	return ARM_DRIVER_OK;
}

int32_t ARM_USART_Uninitialize(void) {
	return ARM_DRIVER_OK;
}

int32_t ARM_USART_PowerControl(ARM_POWER_STATE state) {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t ARM_USART_Send(const void *data, uint32_t num) {
	const char *ptr = data; int len = num;
	char* new_data;
	VCOM_DATA_T *pVcom = &g_vCOM;
	uint32_t ret = 0;

	if ( (pVcom->tx_flags & VCOM_TX_CONNECTED) && ((pVcom->tx_flags & VCOM_TX_BUSY) == 0) ) {
		pVcom->tx_flags |= VCOM_TX_BUSY;

		/* enter critical section */
		NVIC_DisableIRQ(USB_IRQn);

		// Ensure we return carriage
		if ((len >= 2) && (ptr[len-1] == '\n') && (ptr[len-2] != '\r')) {
			new_data = (char*)malloc(num);
			if (new_data) {
				strcpy(new_data, data);
				new_data[num] = '\r';
				ret = USBD_API->hw->WriteEP(pVcom->hUsb, USB_CDC_IN_EP, (uint8_t*)new_data, num + 1);
				free(new_data);
			}
		} else {
			ret = USBD_API->hw->WriteEP(pVcom->hUsb, USB_CDC_IN_EP, (uint8_t*)data, num);
		}

		/* exit critical section */
		NVIC_EnableIRQ(USB_IRQn);
	}

	return ret;
}

int32_t ARM_USART_Receive(void *data, uint32_t num) {
	VCOM_DATA_T *pVcom = &g_vCOM;
	uint32_t ret;

	/* check if we queued Rx buffer */
	if (pVcom->rx_flags & (VCOM_RX_BUF_QUEUED | VCOM_RX_DB_QUEUED)) {
		// Device busy
		return 0;
	}

	/* enter critical section */
	NVIC_DisableIRQ(USB_IRQn);
	/* if not queue the request and return 0 bytes */
	ret = USBD_API->hw->ReadReqEP(pVcom->hUsb, USB_CDC_OUT_EP, data, num);
	/* exit critical section */
	NVIC_EnableIRQ(USB_IRQn);
	pVcom->rx_flags |= VCOM_RX_DB_QUEUED;

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
	return g_vCOM.rx_count;
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

uint32_t vcom_connected(void) {
	return g_vCOM.tx_flags & VCOM_TX_CONNECTED;
}
