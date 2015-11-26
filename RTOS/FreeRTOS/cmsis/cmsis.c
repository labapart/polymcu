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
#include "event_groups.h"
#include "semphr.h"
#include "task.h"

uint32_t const os_tickfreq   = configCPU_CLOCK_HZ;
uint16_t const os_tickus_i   = configCPU_CLOCK_HZ / 1000000;
uint16_t const os_tickus_f   = (((uint64_t)(configCPU_CLOCK_HZ - 1000000 * (configCPU_CLOCK_HZ/1000000)))<<16) / 1000000;
uint32_t const os_tick_period_ms = portTICK_PERIOD_MS;

typedef struct {
	TaskHandle_t		handle;
	EventGroupHandle_t	event_group;
} os_thread_t;

typedef struct {
	TimerHandle_t		timer;
} os_timer_t;

typedef struct {
	SemaphoreHandle_t *mutex;
} os_mutex_t;

typedef struct {
	SemaphoreHandle_t *sem;
	int32_t            max_count;
} os_semaphore_t;

typedef struct {
	QueueHandle_t *queue;
} os_message_t;

os_thread_t g_threads[RTOS_TASK_COUNT];

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
osStatus osDelay (uint32_t millisec) {
	vTaskDelay(millisec / portTICK_PERIOD_MS);
	return osEventTimeout;
}

/// Get the RTOS kernel system timer counter.
/// \return RTOS kernel system timer as 32-bit value
uint32_t osKernelSysTick (void) {
	if (__get_IPSR() != 0) {
		return xTaskGetTickCountFromISR();
	} else {
		return xTaskGetTickCount();
	}
}

osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argument) {
	unsigned short usStackDepth = configMINIMAL_STACK_SIZE;
	os_thread_t *thread_id = NULL;
	BaseType_t res;
	uint32_t index;

	if (thread_def->stacksize != 0) {
		usStackDepth = thread_def->stacksize;
	}

	// Find free thread slot
	for (index = 0; index < RTOS_TASK_COUNT; index++) {
		if (g_threads[index].handle == NULL) {
			thread_id = &g_threads[index];
		}
	}
	if (thread_id == NULL) {
		return NULL;
	}

	res = xTaskCreate((TaskFunction_t)thread_def->pthread,	/* The function that implements the task. */
				"Task", 									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				usStackDepth, 								/* The size of the stack to allocate to the task. */
				argument, 									/* The parameter passed to the task - just to check the functionality. */
				tskIDLE_PRIORITY + thread_def->tpriority,	/* The priority assigned to the task. */
				&thread_id->handle);										/* The task handle is not required, so NULL is passed. */
	 if (res == pdPASS) {
		 thread_id->event_group = xEventGroupCreate();
		 return (osThreadId)thread_id;
	 } else {
		 return NULL;
	 }
}

/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId osThreadGetId (void) {
	uint32_t index;
	TaskHandle_t handle = xTaskGetCurrentTaskHandle();

	for (index = 0; index < RTOS_TASK_COUNT; index++) {
		if (g_threads[index].handle == handle) {
			return (osThreadId)&g_threads[index];
		}
	}
	return NULL;
}

/// Terminate execution of a thread and remove it from Active Threads.
/// \param[in]     thread_id   thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus osThreadTerminate (osThreadId thread_id) {
	os_thread_t *thread = (os_thread_t *)thread_id;

	if (thread->handle != NULL) {
		if (thread->event_group != NULL) {
			vEventGroupDelete(thread->event_group);
		}
		vTaskDelete(thread->handle);
		thread->handle = NULL;

		return osOK;
	} else {
		return osErrorParameter;
	}
}

/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
osStatus osThreadYield (void) {
	taskYIELD();
	return osOK;
}

/// Change priority of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus osThreadSetPriority (osThreadId thread_id, osPriority priority) {
	if ((priority < osPriorityIdle) || (priority > osPriorityRealtime)) {
		return osErrorParameter;
	}

	os_thread_t *thread = (os_thread_t *)thread_id;
	vTaskPrioritySet(thread->handle, priority);
	return osOK;
}

/// Get current priority of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return current priority value of the thread function.
osPriority osThreadGetPriority (osThreadId thread_id) {
	os_thread_t *thread = (os_thread_t *)thread_id;

	UBaseType_t priority = uxTaskPriorityGet(thread->handle);
	return priority;
}

/// Create a timer.
/// \param[in]     timer_def     timer object referenced with \ref osTimer.
/// \param[in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer call back function.
/// \return timer ID for reference by other functions or NULL in case of error.
osTimerId osTimerCreate (const osTimerDef_t *timer_def, os_timer_type type, void *argument) {
	os_timer_t *timer = timer_def->timer;

	timer->timer = xTimerCreate("timer",
								1, // xTimerPeriod - cannot be '0'
								(const TickType_t)type, // 0 = osTimerOnce, 1 = osTimerPeriodic
								timer,
								(TimerCallbackFunction_t)timer_def->ptimer);
	return (osTimerId)timer;
}

/// Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \param[in]     millisec      time delay value of the timer.
/// \return status code that indicates the execution status of the function.
osStatus osTimerStart (osTimerId timer_id, uint32_t millisec) {
	os_timer_t *timer = (os_timer_t*)timer_id;
	BaseType_t ret;

	ret =  xTimerChangePeriod(timer->timer, millisec / portTICK_PERIOD_MS, 0);
	if (ret != pdPASS) {
		return osErrorParameter;
	}

	ret = xTimerStart(timer->timer, 0);
	if (ret == pdPASS) {
		return osOK;
	} else {
		return osErrorParameter;
	}
}

/// Stop the timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \return status code that indicates the execution status of the function.
osStatus osTimerStop (osTimerId timer_id) {
	os_timer_t *timer = (os_timer_t*)timer_id;
	BaseType_t ret = xTimerStop(timer->timer, 0);
	if (ret == pdPASS) {
		return osOK;
	} else {
		return osErrorParameter;
	}
}

/// Delete a timer that was created by \ref osTimerCreate.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \return status code that indicates the execution status of the function.
osStatus osTimerDelete(osTimerId timer_id) {
	os_timer_t *timer = (os_timer_t*)timer_id;

	if (timer->timer) {
		BaseType_t ret = xTimerDelete(timer->timer, 0);
		if (ret == pdPASS) {
			timer->timer = NULL;
			return osOK;
		} else {
			return osErrorParameter;
		}
	} else {
		return osErrorParameter;
	}
}


//  ==== Signal Management ====
/* The following bit fields convey control information in a task's event list
item value.  It is important they don't clash with the
taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#if configUSE_16_BIT_TICKS == 1
	#define eventEVENT_BITS_CONTROL_BYTES	0xff00U
#else
	#define eventEVENT_BITS_CONTROL_BYTES	0xff000000UL
#endif

/// Set the specified Signal Flags of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     signals       specifies the signal flags of the thread that should be set.
/// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
int32_t osSignalSet (osThreadId thread_id, int32_t signals) {
	EventBits_t previous_bits;

	os_thread_t *thread = (os_thread_t *)thread_id;
	if (thread->event_group == NULL) {
		thread->event_group = xEventGroupCreate();
	}

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )
	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		previous_bits = xEventGroupGetBitsFromISR(thread->event_group);

		xEventGroupSetBitsFromISR(thread->event_group, signals, &pxHigherPriorityTaskWoken);
	} else {
		previous_bits = xEventGroupGetBits(thread->event_group);

		xEventGroupSetBits(thread->event_group, signals);
	}
#else
	previous_bits = xEventGroupGetBits(thread->event_group);

	xEventGroupSetBits(thread->event_group, signals);
#endif
	return previous_bits;
}

/// Clear the specified Signal Flags of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     signals       specifies the signal flags of the thread that shall be cleared.
/// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters or call from ISR.
int32_t osSignalClear (osThreadId thread_id, int32_t signals) {
	EventBits_t previous_bits;

	os_thread_t *thread = (os_thread_t *)thread_id;
	if (thread->event_group == NULL) {
		thread->event_group = xEventGroupCreate();
	}

	// Ignore the control bits
	signals = signals & ~eventEVENT_BITS_CONTROL_BYTES;

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )
	if (__get_IPSR() != 0) {
		previous_bits = xEventGroupGetBitsFromISR(thread->event_group);

		xEventGroupClearBitsFromISR(thread->event_group, signals);
	} else {
		previous_bits = xEventGroupGetBits(thread->event_group);

		xEventGroupClearBits(thread->event_group, signals);
	}
#else
	previous_bits = xEventGroupGetBits(thread->event_group);

	xEventGroupClearBits(thread->event_group, signals);
#endif

	return previous_bits;
}

/// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
/// \param[in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return event flag information or error code.
os_InRegs osEvent osSignalWait (int32_t signals, uint32_t millisec) {
	osEvent event;

	os_thread_t *thread = (os_thread_t *)osThreadGetId();
	if (thread->event_group == NULL) {
		thread->event_group = xEventGroupCreate();
	}

	EventBits_t bits = xEventGroupWaitBits(
								thread->event_group,
								signals,
								pdTRUE, // Clear on exit
								pdTRUE, // WaitForAllBits,
								millisec / portTICK_PERIOD_MS);
	if (bits & signals) {
		event.status = osEventSignal;
		event.value.v = bits;
	} else {
		event.status = osEventTimeout;
	}
	return event;
}


/// Create and Initialize a Mutex object.
/// \param[in]     mutex_def     mutex definition referenced with \ref osMutex.
/// \return mutex ID for reference by other functions or NULL in case of error.
osMutexId osMutexCreate (const osMutexDef_t *mutex_def) {
	os_mutex_t* mutex = mutex_def->mutex;
	mutex->mutex = xSemaphoreCreateMutex();
	return (osMutexId)mutex;
}

/// Wait until a Mutex becomes available.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus osMutexWait (osMutexId mutex_id, uint32_t millisec) {
	os_mutex_t* mutex = (os_mutex_t*)mutex_id;
	BaseType_t res;

	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		res = xSemaphoreTakeFromISR(mutex->mutex, &pxHigherPriorityTaskWoken);
	} else {
		res = xSemaphoreTake(mutex->mutex, millisec / portTICK_PERIOD_MS);
	}
	if (res == pdTRUE) {
		return osOK;
	} else {
		return osErrorTimeoutResource;
	}
}

/// Release a Mutex that was obtained by \ref osMutexWait.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \return status code that indicates the execution status of the function.
osStatus osMutexRelease (osMutexId mutex_id) {
	os_mutex_t* mutex = (os_mutex_t*)mutex_id;

	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		xSemaphoreGiveFromISR(mutex->mutex, &pxHigherPriorityTaskWoken);
	} else {
		xSemaphoreGive(mutex->mutex);
	}
	return osOK;
}

/// Delete a Mutex that was created by \ref osMutexCreate.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \return status code that indicates the execution status of the function.
osStatus osMutexDelete (osMutexId mutex_id) {
	os_mutex_t* mutex = (os_mutex_t*)mutex_id;

	if (mutex->mutex) {
		vSemaphoreDelete(mutex->mutex);
		mutex->mutex = NULL;
		return osOK;
	} else {
		return osErrorParameter;
	}
}


/// Create and Initialize a Semaphore object used for managing resources.
/// \param[in]     semaphore_def semaphore definition referenced with \ref osSemaphore.
/// \param[in]     count         number of available resources.
/// \return semaphore ID for reference by other functions or NULL in case of error.
osSemaphoreId osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def, int32_t count) {
	os_semaphore_t* sem = semaphore_def->semaphore;
	sem->sem = xSemaphoreCreateCounting(count + 1, count);
	sem->max_count = count + 1;
	return (osSemaphoreId)sem;
}

/// Wait until a Semaphore token becomes available.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return number of available tokens, or -1 in case of incorrect parameters.
int32_t osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec) {
	os_semaphore_t* sem = (os_semaphore_t*)semaphore_id;
	int32_t res;

	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		res = xSemaphoreTakeFromISR(sem->sem, &pxHigherPriorityTaskWoken);
	} else {
		res = xSemaphoreTake(sem->sem, millisec / portTICK_PERIOD_MS);
	}
	if (res == pdTRUE) {
		return sem->max_count - uxQueueSpacesAvailable((QueueHandle_t)sem->sem) + 1;
	} else {
		return 0;
	}
}

/// Release a Semaphore token.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \return status code that indicates the execution status of the function.
osStatus osSemaphoreRelease (osSemaphoreId semaphore_id) {
	os_semaphore_t* sem = (os_semaphore_t*)semaphore_id;
	BaseType_t res;

	if (__get_IPSR() != 0) {
		BaseType_t pxHigherPriorityTaskWoken;

		res = xSemaphoreGiveFromISR(sem->sem, &pxHigherPriorityTaskWoken);
	} else {
		res = xSemaphoreGive(sem->sem);
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
	os_semaphore_t* sem = (os_semaphore_t*)semaphore_id;
	if (sem->sem) {
		vSemaphoreDelete(sem->sem);
		sem->sem = NULL;
		return osOK;
	} else {
		return osErrorParameter;
	}
}

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))     // Message Queues available

/// Create and Initialize a Message Queue.
/// \param[in]     queue_def     queue definition referenced with \ref osMessageQ.
/// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
/// \return message queue ID for reference by other functions or NULL in case of error.
osMessageQId osMessageCreate (const osMessageQDef_t *queue_def, osThreadId thread_id) {
	os_message_t* message = (os_message_t*)queue_def->queue;
	message->queue = xQueueCreate(queue_def->queue_sz, sizeof(uint32_t));
	return (osMessageQId)message;
}

/// Put a Message to a Queue.
/// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
/// \param[in]     info          message information.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec) {
	os_message_t* message = (os_message_t*)queue_id;

	BaseType_t ret = xQueueSend(message->queue, &info, millisec / portTICK_PERIOD_MS);
	if (ret == pdTRUE) {
		return osOK;
	} else {
		return osErrorResource;
	}
}

/// Get a Message or Wait for a Message from a Queue.
/// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
/// \param[in]     millisec      timeout value or 0 in case of no time-out.
/// \return event information that includes status code.
os_InRegs osEvent osMessageGet (osMessageQId queue_id, uint32_t millisec) {
	os_message_t* message = (os_message_t*)queue_id;
	osEvent event;
	uint32_t info;

	BaseType_t ret = xQueueReceive(message->queue, &info, millisec / portTICK_PERIOD_MS);
	if (ret == pdTRUE) {
		event.status = osEventMessage;
		event.value.v = info;
	} else {
		event.status = osEventTimeout;
	}
	return event;
}

#endif     // Message Queues available
