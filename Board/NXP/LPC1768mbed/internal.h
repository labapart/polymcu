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

#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include "board.h"
#include "Driver_USART.h"
#include "lpc_types.h"
#include "chip_lpc175x_6x.h"
#include "gpio_17xx_40xx.h"
#include "mbed.h"
#ifdef SUPPORT_DEVICE_USB
  #include "USBD_ROM.h"
#endif

#ifdef SUPPORT_DEVICE_USB
/**
 * @brief	USB port init routine
 * @param	hUsb		: Handle to USBD stack instance
 * @param	pDesc		: Pointer to configuration descriptor
 * @param	pUsbParam	: Pointer USB param structure returned by previous init call
 * @return	Always returns LPC_OK.
 */
ErrorCode_t vcom_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam);
ErrorCode_t hid_generic_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam);

extern const uint32_t ep_count;
extern const USBD_FUNC_INIT usb_interface_inits[];
#endif

/**
 * @}
 */

#endif
