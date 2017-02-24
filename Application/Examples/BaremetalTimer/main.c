/*
 * Copyright (c) 2015-2017, Lab A Part
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
#include <assert.h>
#include <stdio.h>
#include "PolyMCU.h"

#define BAREMETAL_TEST_PERIODIC	(1 << 0)
#define BAREMETAL_TEST_ONETIME	(1 << 1)

#ifdef PERIOD_IN_SECOND
  #define TIMER_PERIOD  PERIOD_IN_SECOND
#else
  #define TIMER_PERIOD  TIMER_PERIOD_MILLISECOND
#endif

int g_state;

void task_periodic_func(void* arg) {
	g_state |= BAREMETAL_TEST_PERIODIC;
}

void task_one_time_func(void* arg) {
	g_state |= BAREMETAL_TEST_ONETIME;
}

// The processor clock is initialized by CMSIS startup + system file
int main (void) {
	int ret;

	polymcu_timer_task_t task_periodic;
	polymcu_timer_task_t task_one_time;

	led_on(1);

	// Initialize PolyMCU Timer Support
	ret = polymcu_timer_init(TIMER_PERIOD);
	if (ret) {
		puts("ERROR: Fail to initialize PolyMCU Timer");
		while(1);
	} else {
		printf("The period is %d.\n", polymcu_timer_get_period());
	}

	task_periodic = polymcu_timer_create_periodic_task(task_periodic_func, 4 * TIMER_PERIOD, NULL);
	assert(task_periodic != NULL);
	task_one_time = polymcu_timer_create_one_time_task(task_one_time_func, 10 * TIMER_PERIOD, NULL);
	assert(task_one_time != NULL);

	polymcu_timer_start_task(task_periodic);
	polymcu_timer_start_task(task_one_time);

	while (1) {
		if (g_state & BAREMETAL_TEST_PERIODIC) {
			g_state &= ~BAREMETAL_TEST_PERIODIC;
			puts("# Periodic event");
		}
		if (g_state & BAREMETAL_TEST_ONETIME) {
			g_state &= ~BAREMETAL_TEST_ONETIME;
			puts("# One Time event");

			// Resume one-time task
			polymcu_timer_start_task(task_one_time);
		}
		printf("Wait 2 seconds (current timer value: %d)\n", polymcu_timer_get_value());
		polymcu_wait(2 * TIMER_PERIOD);
	}
}
