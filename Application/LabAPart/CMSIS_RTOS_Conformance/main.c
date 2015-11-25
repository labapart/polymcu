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

#include "conformance.h"

uint32_t g_counter;
uint32_t g_completed;

osMutexId mutex_test;
osMutexDef(mutex_test);

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))
  osSemaphoreId sem_test;
  osSemaphoreDef(sem_test);
#endif

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))
  osPoolId pool_test;
  osPoolDef(pool_test, 3, uint32_t);
#endif

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))
  osMessageQId queue_test;
  osMessageQDef(queue_test, 3, uint32_t);
#endif

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))
  osMailQId  mail_test;
  osMailQDef(mail_test, 3, uint32_t);
#endif

//
// thread function 'job_test1'
//
void job_test1(void const *argument) {
	osThreadId thread_id = osThreadGetId();
	test_assert("Test osThreadGetId()", thread_id != NULL);

	// Tell we have finished our test
	g_completed = 1;

	// Pass control to the next thread
	while(1) {
		osThreadYield();
	}
}
// We set the same priority at the main application to ensure we dispatch both tasks
osThreadDef(job_test1, osPriorityNormal, 1, 0);

//
// thread function 'job_test2'
//
void job_test2(void const *argument) {
	osThreadId thread_id = osThreadGetId();
	osPriority priority;
	osStatus status;

	priority = osThreadGetPriority(thread_id);
	test_assert("Test expected priority", priority == osPriorityAboveNormal);

	status = osThreadSetPriority(thread_id, osPriorityNormal);
	test_status("Test osThreadSetPriority()", status);
	status = osThreadSetPriority(thread_id, osPriorityRealtime + 1);
	test_assert("Test osThreadSetPriority() with invalid priority 'osPriorityRealtime + 1'", status != osOK);
	status = osThreadSetPriority(thread_id, osPriorityIdle - 1);
	test_assert("Test osThreadSetPriority() with invalid priority 'osPriorityIdle - 1'", status != osOK);
	priority = osThreadGetPriority(thread_id);
	test_assert("Test expected priority after changing it", priority == osPriorityNormal);

	// Pass control to the next thread
	while(1) {
		osThreadYield();
	}
}
// We set the priority one level above the main application as we will decrease the priority
osThreadDef(job_test2, osPriorityAboveNormal, 1, 0);

//
// thread function 'job_test_delay'
//
void job_test_delay(void const *argument) {
	osDelay(1);
}
// We set the same priority at the main application to ensure we dispatch both tasks
osThreadDef(job_test_delay, osPriorityNormal, 1, 0);

//
// thread function 'job_mutex_test'
//
void job_mutex_test(void const *argument) {
	osStatus status;

	status = osMutexWait(mutex_test, 100);
	test_assert("Test osMutexWait() when mutex has already be taken", status == osErrorTimeoutResource);
	status = osMutexWait(mutex_test, osWaitForever);
	test_assert("Test osMutexWait() after mutex has been released", status == osOK);
	// We release the mutex
	status = osMutexRelease(mutex_test);
	test_status("Test osMutexRelease()", status);

	// Tell we have finished our test
	g_completed = 1;

	// Pass control to the next thread
	while(1) {
		osThreadYield();
	}
}
// We set the same priority at the main application to ensure we dispatch both tasks
osThreadDef(job_mutex_test, osPriorityNormal, 1, 0);

void job_timer(void const *arg) {
	g_counter++;
}
osTimerDef(test_timer, job_timer);

// The processor clock is initialized by CMSIS startup + system file
int main(void) {
	osStatus status;
	uint32_t index;
	uint32_t tick, tick_delta, tick1, tick2, tick_diff;
	osTimerId timer;
	osThreadId thread;
	int32_t signal1;
	osEvent event;
	int32_t tokens;
#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))
	uint32_t *pool1, *pool2, *pool3, *pool4;
#endif
#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))
	uint32_t *mail1, *mail2, *mail3, *mail4;
#endif

	// Note1: osKernelInitialize() is called before main() in software_init_hook() when newlib is used.
	//       main() is considered as a task by RTX

	puts("# Test osKernelRunning()");
	// Ensure the kernel is running
	test_assert("Test osKernelRunning() with running kernel", osKernelRunning() == 1);

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))
	puts("# Test osKernelSysTick()");
	tick1 = osKernelSysTick();
	test_assert("Test osKernelSysTick()", tick1 > 0);
	for (index = 0; index < 1000; index++) {
		__NOP();
	}
	tick2 = osKernelSysTick();
	test_assert("Test osKernelSysTick() difference", tick1 != tick2);
	test_assert("Test osKernelSysTick() is going up", tick1 < tick2);
#endif

	puts("# Test osThreadCreate() / osThreadTerminate() / osThreadGetId()");
	g_completed = 0;
	thread = osThreadCreate(osThread(job_test1), NULL);
	test_assert("Test osThreadCreate()", thread != NULL);
	while (g_completed == 0) {
		__WFI();
	}
	status = osThreadTerminate(thread);
	test_status("Test osThreadTerminate()", status);
	status = osThreadTerminate(thread);
	test_assert("Test osThreadTerminate() cannot be called on terminated thread", status != osOK);

	puts("# osDelay()");

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))
	puts("# Test osKernelSysTick() / osDelay()");
	tick1 = osKernelSysTick();
	status = osDelay(2000);
	tick2 = osKernelSysTick();
	test_assert("Test osDelay()", status == osEventTimeout);
	test_assert("Test osKernelSysTick()", tick1 != tick2);
	test_assert("Test systick is going up", tick1 < tick2);
	tick_diff = tick2 - tick1;
	tick = osKernelSysTickMicroSec(2000 * 1000);
	// 10 milliseconds delta
	tick_delta = osKernelSysTickMicroSec(10 * 1000);
	test_assert("Test expected systick difference", (tick - tick_delta < tick_diff) && (tick_diff < tick + tick_delta));
#endif

	puts("Test osTimerCreate() / osTimerStart() / osTimerStop() / osTimerDelete(): osTimerOnce");
	g_counter = 0;
	timer = osTimerCreate(osTimer(test_timer), osTimerOnce, NULL);
	test_assert("Test osTimerCreate()", timer != NULL);
	osDelay(1000);
	test_assert("Test the timer has not started yet", g_counter == 0);
	status = osTimerStart(timer, 200);
	test_assert("Test the timer has not ticked yet", g_counter == 0);
	test_status("Test osTimerStart()", status);
	osDelay(500);
	test_assert("Test the timer has only ticked one", g_counter == 1);
	status = osTimerDelete(timer);
	test_status("Test osTimerDelete()", status);
	status = osTimerDelete(timer);
	test_assert("Test osTimerDelete() cannot be called twice", status != osOK);

	puts("Test osTimerCreate() / osTimerStart() / osTimerStop() / osTimerDelete(): osTimerPeriodic");
	g_counter = 0;
	timer = osTimerCreate(osTimer(test_timer), osTimerPeriodic, NULL);
	test_assert("Test osTimerCreate()", timer != NULL);
	osDelay(1000);
	test_assert("Test the timer has not started yet", g_counter == 0);
	status = osTimerStart(timer, 100);
	test_status("Test osTimerStart()", status);
	osDelay(1100);
	test_assert("Test the timer function has been called more than 10 times", g_counter > 10);
	test_assert("Test the timer function has been called more less than 12 times", g_counter < 12);
	status = osTimerDelete(timer);
	test_status("Test osTimerDelete()", status);
	status = osTimerDelete(timer);
	test_assert("Test osTimerDelete() cannot be called twice", status != osOK);

	puts("# Test osThreadSetPriority() / osThreadGetPriority()");
	thread = osThreadCreate (osThread(job_test2), NULL);
	osDelay(1000);
	status = osThreadTerminate(thread);
	test_status("Test osThreadTerminate()", status);

	puts("# Test osSignalSet() / osSignalClear() / osSignalWait()");
	thread = osThreadGetId();
	osSignalClear(thread, ~0);
	signal1 = osSignalSet(thread, 0x7);
	test_assert("Test osSignalClear()", signal1 == 0);
	event = osSignalWait(0x1, 0);
	test_assert("Test osSignalWait()", event.status == osEventSignal);
	event = osSignalWait(0x8, 100);
	test_assert("Test osSignalWait() with timeout", event.status == osEventTimeout);
	signal1 = osSignalClear(thread, 0x2);
	test_assert("Test osSignalClear() returns the previous signal flags", signal1 == 0x6);

	puts("# Test osMutexCreate() / osMutexWait() / osMutexRelease() / osMutexDelete()");
	g_completed = 0;
	mutex_test = osMutexCreate(osMutex(mutex_test));
	test_assert("Test osMutexCreate()", mutex_test != NULL);
	// Get the mutex
	status = osMutexWait(mutex_test, osWaitForever);
	test_status("Test osMutexWait() to get mutex", status);
	// Create thread
	thread = osThreadCreate(osThread(job_mutex_test), NULL);
	// We wait to test the timeout by osMutexWait() in job_mutex_test()
	osDelay(2000);
	// Release the mutex to ensure job_mutex_test() can take the mutex
	status = osMutexRelease(mutex_test);
	test_status("Test osMutexRelease()", status);
	osDelay(5000); //OM: Reduced to 2000
	if (g_completed) {
		test_status("Test task completed", osOK);
	} else {
		test_status("Test task completed", osErrorOS);
	}
	status = osThreadTerminate(thread);
	test_status("Test osThreadTerminate()", status);
	// We should be able to get the mutex
	status = osMutexWait(mutex_test, 100);
	test_status("Test osMutexWait() (2)", status);
	status = osMutexDelete(mutex_test);
	test_status("Test osMutexDelete()", status);
	status = osMutexDelete(mutex_test);
	test_assert("Test osMutexDelete() when the mutex has already been deleted", status != osOK);

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))
	puts("# Test osSemaphoreCreate() / osSemaphoreWait() / osSemaphoreRelease() / osSemaphoreDelete()");
	sem_test = osSemaphoreCreate(osSemaphore(sem_test), 3);
	test_assert("Test osSemaphoreCreate()", sem_test != NULL);
	tokens = osSemaphoreWait(sem_test, osWaitForever);
	test_assert("Test osSemaphoreWait() (1)", tokens == 3);
	tokens = osSemaphoreWait(sem_test, osWaitForever);
	test_assert("Test osSemaphoreWait() (2)", tokens == 2);
	tokens = osSemaphoreWait(sem_test, osWaitForever);
	test_assert("Test osSemaphoreWait() (3)", tokens == 1);
	tokens = osSemaphoreWait(sem_test, 100);
	test_assert("Test osSemaphoreWait() when out of resource", tokens == 0);
	status = osSemaphoreRelease(sem_test);
	test_status("Test osSemaphoreRelease() (1)", status);
	status = osSemaphoreRelease(sem_test);
	test_status("Test osSemaphoreRelease() (2)", status);
	status = osSemaphoreRelease(sem_test);
	test_status("Test osSemaphoreRelease() (3)", status);
	status = osSemaphoreRelease(sem_test);
	test_assert("Test osSemaphoreRelease() when there is no semaphore to release", status != osOK);
	status = osSemaphoreDelete(sem_test);
	test_status("Test osSemaphoreDelete()", status);
	status = osSemaphoreDelete(sem_test);
	test_assert("Test osSemaphoreDelete() when semaphore has already been deleted", status != osOK);
#endif

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))
	puts("# osPoolCreate() / osPoolAlloc() / osPoolCAlloc() / osPoolFree()");
	pool_test = osPoolCreate(osPool(pool_test));
	test_assert("Test osPoolCreate()", pool_test != NULL);
	pool1 = osPoolAlloc(pool_test);
	test_assert("Test osPoolAlloc() (1)", pool1 != NULL);
	pool2 = osPoolAlloc(pool_test);
	test_assert("Test osPoolAlloc() (2)", pool2 != NULL);
	pool3 = osPoolAlloc(pool_test);
	test_assert("Test osPoolAlloc() (3)", pool3 != NULL);
	*pool3 = ~0;
	status = osPoolFree(pool_test, pool3);
	test_status("Test osPoolFree()", status);
	//Note: Preventing double osPoolFree() will make CMSIS RTOS be non deterministic.
	//status = osPoolFree(pool_test, pool3);
	//test_assert("Test osPoolFree() when the release has been freed", status != osOK);
	pool3 = osPoolCAlloc(pool_test);
	test_assert("Test osPoolCAlloc()", pool3 != NULL);
	test_assert("Test osPoolCAlloc() set the memory to zero", *pool3 == 0);
	pool4 = osPoolAlloc(pool_test);
	test_assert("Test osPoolAlloc() when there is no resource available", pool4 == NULL);
#endif

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))
	puts("# osMessageCreate() / osMessagePut() / osMessageGet()");
	queue_test = osMessageCreate(osMessageQ(queue_test), NULL);
	test_assert("Test osMessageCreate()", queue_test != NULL);
	event = osMessageGet(queue_test, 100);
	test_assert("Test osMessageGet() when there is no message", event.status != osOK);
	status = osMessagePut(queue_test, 0x12345678, 100);
	test_status("Test osMessagePut()", status);
	event = osMessageGet(queue_test, 100);
	test_assert("Test osMessageGet()", event.status == osEventMessage);
	test_assert("Test osMessageGet() returned value", event.value.v == 0x12345678);
	status = osMessagePut(queue_test, 0x1, 100);
	test_status("Test osMessagePut() (1)", status);
	status = osMessagePut(queue_test, 0x2, 100);
	test_status("Test osMessagePut() (2)", status);
	status = osMessagePut(queue_test, 0x3, 100);
	test_status("Test osMessagePut() (3)", status);
	status = osMessagePut(queue_test, 0x4, 100);
	test_assert("Test osMessagePut() when there is no space", status != osOK);
#endif

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))
	puts("# Test osMailCreate() / osMailAlloc() / osMailCAlloc() / osMailPut() / osMailGet() / osMailFree()");
	mail_test = osMailCreate(osMailQ(mail_test), NULL);
	test_assert("Test osMailCreate()", mail_test != NULL);
	event = osMailGet(mail_test, 100);
	test_assert("Test osMailGet() when there is no message", event.status != osOK);
	status = osMailPut(mail_test, NULL);
	test_assert("Test osMailPut() with NULL message", status != osOK);
	status = osMailPut(mail_test, (void*)0x1);
	test_assert("Test osMailPut() with invalid address", status != osOK);
	mail1 = osMailAlloc(mail_test, osWaitForever);
	test_assert("Test osMailAlloc() (1)", mail1 != NULL);
	status = osMailPut(mail_test, mail1);
	test_status("Test osMailPut() (1)", status);
	mail2 = osMailAlloc(mail_test, osWaitForever);
	test_assert("Test osMailAlloc() (2)", mail2 != NULL);
	status = osMailPut(mail_test, mail2);
	test_status("Test osMailPut() (2)", status);
	mail3 = osMailAlloc(mail_test, osWaitForever);
	test_assert("Test osMailAlloc() (3)", mail3 != NULL);
	status = osMailPut(mail_test, mail3);
	test_status("Test osMailPut() (3)", status);
	mail4 = osMailAlloc(mail_test, 100);
	test_assert("Test osMailAlloc() when out of resource", mail4 == NULL);
	status = osMailPut(mail_test, mail3);
	test_assert("Test osMailPut() when out of resource", status != osOK);
	status = osMailFree(mail_test, mail3);
	test_assert("Test osMailFree() with message which has not been removed from the mailbox", status != osOK);
	event = osMailGet(mail_test, osWaitForever);
	test_status("Test osMailGet()", status);
	status = osMailFree(mail_test, mail3);
	test_status("Test osMailFree()", status);
	status = osMailFree(mail_test, mail3);
	test_assert("Test osMailFree() with message already freed", status != osOK);
#endif

	test_result();

	//Note: This line is optional when using RTX but it is required by FreeRTOS
	osThreadTerminate (osThreadGetId());

	// Note2: osKernelStart() is called before main() in software_init_hook() when newlib is used.
}
