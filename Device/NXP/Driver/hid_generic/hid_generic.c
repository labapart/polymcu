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

#include <assert.h>
#include <string.h>
#include "hid_generic.h"

static uint8_t *loopback_report;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

extern const uint8_t HID_ReportDescriptor[];
extern const uint16_t HID_ReportDescSize;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*  HID get report callback function. */
static ErrorCode_t HID_GetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t *plength) {
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		*pBuffer[0] = *loopback_report;
		*plength = 1;
		break;

	case HID_REPORT_OUTPUT:
		return ERR_USBD_STALL;			/* Not Supported */

	case HID_REPORT_FEATURE:
		return ERR_USBD_STALL;			/* Not Supported */
	}
	return LPC_OK;
}

/* HID set report callback function. */
static ErrorCode_t HID_SetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t length) {
	/* we will reuse standard EP0Buf */
	if (length == 0) {
		return LPC_OK;
	}

	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		return ERR_USBD_STALL;			/* Not Supported */

	case HID_REPORT_OUTPUT:
		*loopback_report = **pBuffer;
		break;

	case HID_REPORT_FEATURE:
		return ERR_USBD_STALL;			/* Not Supported */
	}
	return LPC_OK;
}

/* HID Interrupt endpoint event handler. */
static ErrorCode_t HID_Ep_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event) {
	USB_HID_CTRL_T *pHidCtrl = (USB_HID_CTRL_T *) data;

	switch (event) {
	case USB_EVT_IN:
		/* last report is successfully sent. Do something... */
		break;

	case USB_EVT_OUT:
		/* Read the new report received. */
		USBD_API->hw->ReadEP(hUsb, pHidCtrl->epout_adr, loopback_report);
		/* loopback the report received. */
		USBD_API->hw->WriteEP(hUsb, pHidCtrl->epin_adr, loopback_report, 1);
		break;
	}
	return LPC_OK;
}

ErrorCode_t hid_generic_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam) {
	USB_INTERFACE_DESCRIPTOR *pIntfDesc = find_IntfDesc(pDesc->high_speed_desc, USB_DEVICE_CLASS_HUMAN_INTERFACE);

	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;

	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	/* HID paramas */
	hid_param.max_reports = 1;
	/* Init reports_data */
	reports_data[0].len = HID_ReportDescSize;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &HID_ReportDescriptor[0];

	if (pIntfDesc == 0) {
		return ERR_FAILED;
	}

	hid_param.mem_base = pUsbParam->mem_base;
	hid_param.mem_size = pUsbParam->mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;
	/* user defined functions */
	hid_param.HID_GetReport = HID_GetReport;
	hid_param.HID_SetReport = HID_SetReport;
	hid_param.HID_EpIn_Hdlr  = HID_Ep_Hdlr;
	hid_param.HID_EpOut_Hdlr = HID_Ep_Hdlr;
	hid_param.report_data  = reports_data;

	ret = USBD_API->hid->init(hUsb, &hid_param);
	/* allocate USB accessable memory space for report data */
	loopback_report =  (uint8_t *) hid_param.mem_base;
	hid_param.mem_base += 4;
	hid_param.mem_size += 4;

	/* update memory variables */
	pUsbParam->mem_base = hid_param.mem_base;
	pUsbParam->mem_size = hid_param.mem_size;

	puts("hid_generic_init: Initialization Done!");
	assert(ret == 0);
	return ret;
}
