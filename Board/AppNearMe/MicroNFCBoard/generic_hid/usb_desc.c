/*
 * @brief Virtual Comm port USB descriptors
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

#include "usbd/usbd_rom_api.h"

// This VID/PID pair is ONLY for MicroNFCBoard
#define MICRONFCBOARD_USB_VID				0x1FC9 // NXP VID
#define MICRONFCBOARD_USB_PID				0x8039 // Licensed by NXP to AppNearMe
#define MICRONFCBOARD_USB_PRODUCT_VERSION 	0x0101

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

#define HID_INPUT_REPORT_BYTES       64				/* size of report in Bytes */
#define HID_OUTPUT_REPORT_BYTES      64				/* size of report in Bytes */
#define HID_FEATURE_REPORT_BYTES     64				/* size of report in Bytes */

/**
 * HID Report Descriptor
 */
const uint8_t HID_ReportDescriptor[] = {
	HID_UsagePageVendor(0x00),
	HID_Usage(0x01),
	HID_Collection(HID_Application),
	HID_LogicalMin(0),	/* value range: 0 - 0xFF */
	HID_LogicalMaxS(0xFF),
	HID_ReportSize(8),	/* 8 bits */
	HID_ReportCount(HID_INPUT_REPORT_BYTES),
	HID_Usage(0x01),
	HID_Input(HID_Data | HID_Variable | HID_Absolute),
	HID_ReportCount(HID_OUTPUT_REPORT_BYTES),
	HID_Usage(0x01),
	HID_Output(HID_Data | HID_Variable | HID_Absolute),
	HID_ReportCount(HID_FEATURE_REPORT_BYTES),
	HID_Usage(0x01),
	HID_Feature(HID_Data | HID_Variable | HID_Absolute),
	HID_EndCollection,
};
const uint16_t HID_ReportDescSize = sizeof(HID_ReportDescriptor);

/**
 * USB Standard Device Descriptor
 */
ALIGNED(4) const uint8_t USB_DeviceDescriptor[] = {
	USB_DEVICE_DESC_SIZE,				/* bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,			/* bDescriptorType */
	WBVAL(0x0200),						/* bcdUSB */
	0x00,								/* bDeviceClass */
	0x00,								/* bDeviceSubClass */
	0x00,								/* bDeviceProtocol */
	USB_MAX_PACKET0,					/* bMaxPacketSize0 */
	WBVAL(MICRONFCBOARD_USB_VID),		/* idVendor */
	WBVAL(MICRONFCBOARD_USB_PID),		/* idProduct */
	WBVAL(MICRONFCBOARD_USB_PRODUCT_VERSION),	/* bcdDevice */
	0x01,								/* iManufacturer */
	0x02,								/* iProduct */
	0x03,								/* iSerialNumber */
	0x01								/* bNumConfigurations */
};

/**
 * USB FSConfiguration Descriptor
 * All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
 */
ALIGNED(4) uint8_t USB_FsConfigDescriptor[] = {
	/* Configuration 1 */
	USB_CONFIGURATION_DESC_SIZE,			/* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(									/* wTotalLength */
		USB_CONFIGURATION_DESC_SIZE     +
		/* HID class related descriptors */
		USB_INTERFACE_DESC_SIZE         +
		HID_DESC_SIZE                   +
		USB_ENDPOINT_DESC_SIZE      +
		0
		),
	0x03,									/* bNumInterfaces */
	0x01,									/* bConfigurationValue */
	0x00,									/* iConfiguration */
	USB_CONFIG_SELF_POWERED,				/* bmAttributes  */
	USB_CONFIG_POWER_MA(500),				/* bMaxPower */

	/* Interface 0, Alternate Setting 0, HID Class */
	USB_INTERFACE_DESC_SIZE,		/* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,	/* bDescriptorType */
	USB_HID_IF_NUM,					/* bInterfaceNumber */
	0x00,							/* bAlternateSetting */
	0x01,							/* bNumEndpoints */
	USB_DEVICE_CLASS_HUMAN_INTERFACE,	/* bInterfaceClass */
	HID_SUBCLASS_NONE,				/* bInterfaceSubClass */
	HID_PROTOCOL_NONE,				/* bInterfaceProtocol */
	0x04,							/* iInterface */
	/* HID Class Descriptor */
	/* HID_DESC_OFFSET = 0x0012 */
	HID_DESC_SIZE,					/* bLength */
	HID_HID_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0111),					/* bcdHID : 1.11*/
	0x00,							/* bCountryCode */
	0x01,							/* bNumDescriptors */
	HID_REPORT_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(sizeof(HID_ReportDescriptor)),	/* wDescriptorLength */
	/* Endpoint, HID Interrupt In */
	USB_ENDPOINT_DESC_SIZE,			/* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType */
	USB_HID_IN_EP,					/* bEndpointAddress */
	USB_ENDPOINT_TYPE_INTERRUPT,	/* bmAttributes */
	WBVAL(64),						/* wMaxPacketSize  - 64 bytes packets */
	0x05,					        /* bInterval - 5ms */

	/* Terminator */
	0									/* bLength */
};

/**
 * USB String Descriptor (optional)
 */
ALIGNED(4) const uint8_t USB_StringDescriptor[] = {
	/* Index 0x00: LANGID Codes */
	0x04,								/* bLength */
	USB_STRING_DESCRIPTOR_TYPE,			/* bDescriptorType */
	WBVAL(0x0409),	/* US English */    /* wLANGID */
	/* Index 0x01: Manufacturer */
	(3 * 2 + 2),						/* bLength (13 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,			/* bDescriptorType */
	'N', 0,
	'X', 0,
	'P', 0,
	/* Index 0x02: Product */
	(9 * 2 + 2),						/* bLength */
	USB_STRING_DESCRIPTOR_TYPE,			/* bDescriptorType */
	'H', 0,
	'I', 0,
	'D', 0,
	'0', 0,
	' ', 0,
	'P', 0,
	'o', 0,
	'r', 0,
	't', 0,
	/* Index 0x03: Serial Number */
	(6 * 2 + 2),						/* bLength (8 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,			/* bDescriptorType */
	'N', 0,
	'X', 0,
	'P', 0,
	'-', 0,
	'7', 0,
	'7', 0,
	/* Index 0x04: Interface 1, Alternate Setting 0 */
	( 4 * 2 + 2),						/* bLength (4 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,			/* bDescriptorType */
	'H', 0,
	'I', 0,
	'D', 0,
	'0', 0,
};
