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
#include <cmsis_os.h>
#include <stdio.h>

//
// thread function 'job1'
//
void job1(void const *argument) {
	while (1) {
		printf("job1\n");
		osDelay (2000); // delay execution for 2 seconds
	}
}
osThreadDef(job1, osPriorityAboveNormal, 1, 0); // define job1 as thread function

//
// thread function 'job2'
//
void job2(void const *argument) {
	osDelay (1000); // delay execution for 1 seconds
	while (1) {
		printf("job2\n");
		osDelay (2000); // delay execution for 2 seconds
	}
}
osThreadDef(job2, osPriorityNormal, 1, 0); // define job2 as thread function

// The processor clock is initialized by CMSIS startup + system file
int main(void) {
	// Note1: osKernelInitialize() is called before main() in software_init_hook() when newlib is used.
	//       main() is considered as a task by RTX

	osThreadCreate (osThread(job1), NULL);
	osThreadCreate (osThread(job2), NULL);

	//Note: This line is optional when using RTX but it is required by FreeRTOS
	osThreadTerminate (osThreadGetId());

	// Note2: osKernelStart() is called before main() in software_init_hook() when newlib is used.
}
