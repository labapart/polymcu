/*
 * @brief HID generic example's callabck routines
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

#include "generic_hid_internal.h"
#include "USBD_ROM.h"
#include "app_usbd_cfg.h"

#ifdef CHIP_LPC11UXX
  #define LPC_TIMER0	LPC_TIMER32_0
  #define TIMER0_IRQn	TIMER_32_0_IRQn
#endif

#define TICKRATE_HZ1    5                   /* 5 ticks per second */ //OM: Check the value

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

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
	puts("HID_GetReport");
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
		puts("HID_SetReport length=0");
		return LPC_OK;
	} else {
		puts("HID_SetReport");
	}

	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		puts("HID_REPORT_INPUT");
		return ERR_USBD_STALL;			/* Not Supported */

	case HID_REPORT_OUTPUT:
		puts("HID_REPORT_OUTPUT");
		*loopback_report = **pBuffer;
		break;

	case HID_REPORT_FEATURE:
		puts("HID_REPORT_FEATURE");
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
		read_report(loopback_report);
		break;
	}
	return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

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
	/* allocate USB accessible memory space for report data */
	loopback_report =  (uint8_t *) hid_param.mem_base;
	hid_param.mem_base += DEVICE_USB_HID_INPUT_REPORT_SIZE;
	hid_param.mem_size += DEVICE_USB_HID_INPUT_REPORT_SIZE;

	/* update memory variables */
	pUsbParam->mem_base = hid_param.mem_base;
	pUsbParam->mem_size = hid_param.mem_size;

	return ret;
}

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
#ifdef CHIP_LPC11UXX
void TIMER32_0_IRQHandler(void) {
#else
void TIMER0_IRQHandler(void) {
#endif
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
		// Call the application timer function
		on_timer_tick(NULL);
	}
}

void board_timer_initialize(void) {
	uint32_t timerFreq;

	/* Enable timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, (timerFreq / TICKRATE_HZ1));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 1);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void board_timer_start(void) {
	Chip_TIMER_Enable(LPC_TIMER0);
}

void board_timer_stop(void) {
	Chip_TIMER_Disable(LPC_TIMER0);
}

void board_send_usb_hid_report(uint8_t* report, uint32_t size) {
	memcpy(loopback_report, report, size);
	USBD_API->hw->WriteEP(g_hUsb, HID_EP_IN, loopback_report, size);
}
