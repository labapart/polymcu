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

#ifndef __USBD_ROM_H__
#define __USBD_ROM_H__

#include "usbd/usbd_rom_api.h"

/**
 * @brief	USB Device interface init routine.
 * @param	hUsb		: Handle to USB device stack
 * @param	pDesc		: Pointer to the USB Core descriptors
 * @param	pUsbParam	: Pointer to the USB Parameters
 * @return	On success returns LPC_OK.
 * TODO:OM: Should we update the memory information as the Keyboard_init() was doing?
 */
typedef ErrorCode_t (*USBD_FUNC_INIT)(USBD_HANDLE_T hUsb,
									  USB_CORE_DESCS_T *pDesc,
									  USBD_API_INIT_PARAM_T *pUsbParam);

/**
 * @brief	Initialize the USB Device Interface
 * @param	func_init	: List of USB Device specific initialization function
 * @param	ep_count	: Number of USB EndPoint
 * @return	If found returns the address of requested interface else returns NULL.
 */
ErrorCode_t usbd_rom_init(const USBD_FUNC_INIT func_init_list[], uint32_t ep_count);

/**
 * @brief	Find the address of interface descriptor for given class type.
 * @param	pDesc		: Pointer to configuration descriptor in which the desired class
 *			interface descriptor to be found.
 * @param	intfClass	: Interface class type to be searched.
 * @return	If found returns the address of requested interface else returns NULL.
 */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass);

#endif /* __USBD_ROM_H__ */
