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

#include "Driver_USART.h"
#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_hal.h"
#include "PolyMCU.h"

#ifdef __CMSIS_RTOS
  #include "cmsis_os.h"
#endif

#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's NVIC IRQ and IRQ Handlers */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0)  /* driver version */

void _ttywrch(int ch);

static UART_HandleTypeDef gUartHandle;

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

//
//   Functions
//

ARM_DRIVER_VERSION ARM_USART_GetVersion(void) {
	return DriverVersion;
}

ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void) {
	return DriverCapabilities;
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

#ifndef SUPPORT_RTOS
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void) {
  HAL_IncTick();
}
#endif

int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event) {
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
	  - Word Length = 8 Bits (7 data bit + 1 parity bit) : BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	  - Stop Bit    = One Stop bit
	  - Parity      = ODD parity
	  - BaudRate    = 9600 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	gUartHandle.Instance        = USARTx;

	gUartHandle.Init.BaudRate   = 115200;
	gUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	gUartHandle.Init.StopBits   = UART_STOPBITS_1;
	gUartHandle.Init.Parity     = UART_PARITY_NONE;
	gUartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	gUartHandle.Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(&gUartHandle) != HAL_OK) {
		return ARM_DRIVER_ERROR;
	}

	return ARM_DRIVER_OK;
}

int32_t ARM_USART_Uninitialize(void) {
	return ARM_DRIVER_OK;
}

int32_t ARM_USART_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t ARM_USART_Send(const void *data, uint32_t num) {
	const char *ptr = data; int len = num;

	HAL_StatusTypeDef status = HAL_UART_Transmit(&gUartHandle, (uint8_t *)data, num, HAL_MAX_DELAY);
	if (status == HAL_OK) {
		// Ensure we return carriage
		if ((len >= 2) && (ptr[len-1] == '\n') && (ptr[len-2] != '\r')) {
			_ttywrch('\r');
		}

		return num;
	} else {
		return 0;
	}
}

int32_t ARM_USART_Receive(void *data, uint32_t num) {
	return 0;
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
