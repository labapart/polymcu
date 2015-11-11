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

#include "board.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

uint32_t const os_tickfreq   = configCPU_CLOCK_HZ;
uint16_t const os_tickus_i   = configCPU_CLOCK_HZ / 1000000;
uint16_t const os_tickus_f   = (((uint64_t)(configCPU_CLOCK_HZ - 1000000 * (configCPU_CLOCK_HZ/1000000)))<<16) / 1000000;

/* Main Thread definition */
extern int main (void);
osThreadDef_t os_thread_def_main = {(os_pthread)main, osPriorityNormal, 1, configMAIN_STACK_SIZE };

__attribute__((naked)) void software_init_hook (void) {
  __asm (
    "movs r0,#0\n"
    "movs r1,#0\n"
    "mov  r4,r0\n"
    "mov  r5,r1\n"
    "ldr  r0,= __libc_fini_array\n"
    "bl   atexit\n"
    "bl   __libc_init_array\n"
    "bl   osKernelInitialize\n"
	"ldr  r0,=os_thread_def_main\n"
	"movs r1,#0\n"
	"bl   osThreadCreate\n"
    "bl   osKernelStart\n"
    "bl   infinite_loop\n"
  );
}

void infinite_loop(void) {
	while(1) {
		__WFI();
	}
}

/// Initialize the RTOS Kernel for creating objects.
/// \return status code that indicates the execution status of the function.
osStatus osKernelInitialize (void) {
	// Nothing to do
	return osOK;
}

/// Start the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
osStatus osKernelStart (void) {
	vTaskStartScheduler();
	return osOK;
}

// Note: Tick has been set up to be every milli second.
//TODO: Port this function to be tick period independent
osStatus osDelay (uint32_t millisec) {
	vTaskDelay(millisec);
	return osOK;
}

osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argument) {
	unsigned short usStackDepth = configMINIMAL_STACK_SIZE;
	TaskHandle_t tskHandle;
	BaseType_t res;

	if (thread_def->stacksize != 0) {
		usStackDepth = thread_def->stacksize;
	}

	res = xTaskCreate((TaskFunction_t)thread_def->pthread,	/* The function that implements the task. */
				"Task", 									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				usStackDepth, 								/* The size of the stack to allocate to the task. */
				argument, 									/* The parameter passed to the task - just to check the functionality. */
				tskIDLE_PRIORITY + thread_def->tpriority,	/* The priority assigned to the task. */
				&tskHandle);										/* The task handle is not required, so NULL is passed. */
	 if (res == pdPASS) {
		 return tskHandle;
	 } else {
		 return NULL;
	 }
}

/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId osThreadGetId (void) {
	return xTaskGetCurrentTaskHandle();
}

/// Terminate execution of a thread and remove it from Active Threads.
/// \param[in]     thread_id   thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus osThreadTerminate (osThreadId thread_id) {
	vTaskDelete(thread_id);
	return osOK;
}

/// Create and Initialize a Semaphore object used for managing resources.
/// \param[in]     semaphore_def semaphore definition referenced with \ref osSemaphore.
/// \param[in]     count         number of available resources.
/// \return semaphore ID for reference by other functions or NULL in case of error.
osSemaphoreId osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def, int32_t count) {
	return xSemaphoreCreateCounting(count + 1, count);
}

/// Wait until a Semaphore token becomes available.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return number of available tokens, or -1 in case of incorrect parameters.
int32_t osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec) {
	int32_t res;

	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		res = xSemaphoreTakeFromISR(semaphore_id, &pxHigherPriorityTaskWoken);
	} else {
		res = xSemaphoreTake(semaphore_id, millisec); //TODO: Convert milliseconds into tick number
	}
	return res;
}

/// Release a Semaphore token.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \return status code that indicates the execution status of the function.
osStatus osSemaphoreRelease (osSemaphoreId semaphore_id) {
	BaseType_t res;

	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		res = xSemaphoreGiveFromISR(semaphore_id, &pxHigherPriorityTaskWoken);
	} else {
		res = xSemaphoreGive(semaphore_id);
	}
	if (res == pdTRUE) {
		return osOK;
	} else {
		return osErrorResource;
	}
}

/// Delete a Semaphore that was created by \ref osSemaphoreCreate.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \return status code that indicates the execution status of the function.
osStatus osSemaphoreDelete (osSemaphoreId semaphore_id) {
	vSemaphoreDelete(semaphore_id);
	return osOK;
}
