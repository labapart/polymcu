/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 * Copyright (c) 2017 Lab A Part Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "Driver_I2C.h"

#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

/* Pins to connect shield. */
#define NORDIC_I2C_SDA_PIN 3
#define NORDIC_I2C_SCL_PIN 4

/* Indicates if setting mode operation has ended. */
static volatile bool m_twi_evt_done = false;

/* TWI instance */
static const nrf_drv_twi_t m_twi_instance = NRF_DRV_TWI_INSTANCE(0);

/* TWI configuration */
static nrf_drv_twi_config_t m_twi_config = {
       .scl                = NORDIC_I2C_SCL_PIN,
       .sda                = NORDIC_I2C_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };

#define ARM_I2C_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0) /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_I2C_API_VERSION,
    ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_I2C_CAPABILITIES DriverCapabilities = {
    0  /* supports 10-bit addressing */
};

//
//  Functions
//

ARM_DRIVER_VERSION ARM_I2C_GetVersion(void)
{
}

ARM_I2C_CAPABILITIES ARM_I2C_GetCapabilities(void)
{
}

static void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context) {
    switch(p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            m_twi_evt_done = true;
            break;
        default:
            break;
    }
}

int32_t ARM_I2C_Initialize(ARM_I2C_SignalEvent_t cb_event) {
	ret_code_t err_code;

    err_code = nrf_drv_twi_init(&m_twi_instance, &m_twi_config, twi_handler, NULL);
    if (err_code != NRF_SUCCESS) {
    	return ARM_DRIVER_ERROR;
    }

	nrf_drv_twi_enable(&m_twi_instance);

	return ARM_DRIVER_OK;
}

int32_t ARM_I2C_Uninitialize(void) {
	nrf_drv_twi_disable(&m_twi_instance);

	return ARM_DRIVER_OK;
}

int32_t ARM_I2C_PowerControl(ARM_POWER_STATE state)
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
}

int32_t ARM_I2C_MasterTransmit(uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
	ret_code_t err_code;

    m_twi_evt_done = false;
    err_code = nrf_drv_twi_tx(&m_twi_instance, addr, data, num, xfer_pending);
    if (err_code != NRF_SUCCESS) {
    	return ARM_DRIVER_ERROR;
    }

	// Wait for transmission completed
	while(m_twi_evt_done == false);

	return ARM_DRIVER_OK;
}

int32_t ARM_I2C_MasterReceive(uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
	ret_code_t err_code;

    err_code = nrf_drv_twi_rx(&m_twi_instance, addr, data, num);
    if (err_code != NRF_SUCCESS) {
    	return ARM_DRIVER_ERROR;
    }

	return ARM_DRIVER_OK;
}

int32_t ARM_I2C_SlaveTransmit(const uint8_t *data, uint32_t num)
{
}

int32_t ARM_I2C_SlaveReceive(uint8_t *data, uint32_t num)
{
}

int32_t ARM_I2C_GetDataCount(void)
{
}

int32_t ARM_I2C_Control(uint32_t control, uint32_t arg) {
	switch (control) {
	case ARM_I2C_OWN_ADDRESS:
		return ARM_DRIVER_ERROR_UNSUPPORTED;

	case ARM_I2C_BUS_SPEED:
		switch (arg)
		{
		case ARM_I2C_BUS_SPEED_STANDARD:
			m_twi_config.frequency = NRF_TWI_FREQ_100K;
			break;
		case ARM_I2C_BUS_SPEED_FAST:
			m_twi_config.frequency = NRF_TWI_FREQ_400K;
			break;
		default:
			return ARM_DRIVER_ERROR_UNSUPPORTED;
		}
		break;

	case ARM_I2C_BUS_CLEAR:
		return ARM_DRIVER_ERROR_UNSUPPORTED;

	case ARM_I2C_ABORT_TRANSFER:
		return ARM_DRIVER_ERROR_UNSUPPORTED;

	default:
		return ARM_DRIVER_ERROR_UNSUPPORTED;
	}
}

ARM_I2C_STATUS ARM_I2C_GetStatus(void) {
	ARM_I2C_STATUS i2c_status;

	i2c_status.mode = 1;

	return i2c_status;
}

void ARM_I2C_SignalEvent(uint32_t event)
{
    // function body
}

// End I2C Interface

ARM_DRIVER_I2C Driver_I2C = {
    ARM_I2C_GetVersion,
    ARM_I2C_GetCapabilities,
    ARM_I2C_Initialize,
    ARM_I2C_Uninitialize,
    ARM_I2C_PowerControl,
    ARM_I2C_MasterTransmit,
    ARM_I2C_MasterReceive,
    ARM_I2C_SlaveTransmit,
    ARM_I2C_SlaveReceive,
    ARM_I2C_GetDataCount,
    ARM_I2C_Control,
    ARM_I2C_GetStatus
};
