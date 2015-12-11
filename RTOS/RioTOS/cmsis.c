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

#include "cmsis_os.h"
#include "thread.h"
#include "cmsis_riotos.h"

// Stack allocations
//TODO: This is a simple allocator, we do not support task termination...
char m_stacks[RTOS_TASK_COUNT * RTOS_TASK_STACK_SIZE];
char* m_stacks_ptr = m_stacks;

osStatus osDelay (uint32_t millisec) {
	//TODO: Implement me
	return osEventTimeout;
}

/// Create a thread and add it to Active Threads and set it to state READY.
/// \param[in]     thread_def    thread definition referenced with \ref osThread.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \return thread ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osThreadCreate shall be consistent in every CMSIS-RTOS.
osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argument) {
	// The RIOT priority is the inverse of the CMSIS RTOS priority (ie: RIOT highest priority is '0')
	char priority = THREAD_PRIORITY_MAIN - thread_def->tpriority;
	char* stack = m_stacks_ptr;
	int stacksize = thread_def->stacksize;

	// Check if we are requesting a default stack
	if (stacksize == 0) {
		stacksize = RTOS_TASK_STACK_SIZE;
	}

	// Allocate stack
	if (m_stacks_ptr + stacksize > m_stacks + sizeof(m_stacks)) {
		return (osThreadId)NULL;
	} else {
		m_stacks_ptr += stacksize;
	}

	return thread_create(stack,
						stacksize,
						priority,
						CREATE_STACKTEST,
						(thread_task_func_t)thread_def->pthread,
						argument,
						thread_def->name
						);
}

/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
/// \note MUST REMAIN UNCHANGED: \b osThreadGetId shall be consistent in every CMSIS-RTOS.
osThreadId osThreadGetId (void) {
	return thread_getpid();
}

/// Terminate execution of a thread and remove it from Active Threads.
/// \param[in]     thread_id   thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osThreadTerminate shall be consistent in every CMSIS-RTOS.
osStatus osThreadTerminate (osThreadId thread_id) {
	// Not supported
	return osErrorOS;
}

/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
/// \note MUST REMAIN UNCHANGED: \b osThreadYield shall be consistent in every CMSIS-RTOS.
osStatus osThreadYield (void) {
	thread_yield();
	return osOK;
}
