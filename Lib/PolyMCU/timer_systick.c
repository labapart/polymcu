/*
 * Copyright (c) 2015-2016, Lab A Part
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

#include "PolyMCU.h"
#include <limits.h>

#define POLYMCU_TIMER_STARTED	(1 << 0)
#define POLYMCU_TIMER_PERIODIC	(1 << 1)
#define POLYMCU_TIMER_ONE_TIME	(1 << 2)

struct polymcu_timer_task {
	unsigned int              attributes;
	polymcu_timer_task_func_t function;
	void*                     arg;
	unsigned int              delta;
	unsigned int              next_tick;
};

struct polymcu_timer_task g_polymcu_timer_tasks[TIMER_TASK_MAX];
uint32_t g_counter;
// Keep track of how many instance are using the timer
uint32_t g_timer_user = 0;

void SysTick_Handler(void) {
	unsigned int tick;

	g_counter++;

	tick = polymcu_timer_get_value() + 1;

	for (int i = 0; i < TIMER_TASK_MAX; i++) {
		if ((g_polymcu_timer_tasks[i].function != NULL) &&
			(g_polymcu_timer_tasks[i].attributes & POLYMCU_TIMER_STARTED) &&
			(tick > g_polymcu_timer_tasks[i].next_tick))
		{
			g_polymcu_timer_tasks[i].function(g_polymcu_timer_tasks[i].arg);

			// Update task state
			if (g_polymcu_timer_tasks[i].attributes & POLYMCU_TIMER_ONE_TIME) {
				g_polymcu_timer_tasks[i].attributes &= ~POLYMCU_TIMER_STARTED;
			} else if (g_polymcu_timer_tasks[i].attributes & POLYMCU_TIMER_PERIODIC) {
				g_polymcu_timer_tasks[i].next_tick += g_polymcu_timer_tasks[i].delta;
			}
		}
	}
}

static polymcu_timer_task_t get_free_timer_task(void) {
	for (int i = 0; i < TIMER_TASK_MAX; i++) {
		if (g_polymcu_timer_tasks[i].function == NULL) {
			return &g_polymcu_timer_tasks[i];
		}
	}
	return NULL;
}

int polymcu_timer_init(unsigned int period) {
	// Generate an interrupt every 'period'
	return SysTick_Config(SystemCoreClock / period);
}

unsigned int polymcu_timer_get_period(void) {
	return SystemCoreClock / (SysTick->LOAD + 1UL);
}

polymcu_timer_task_t polymcu_timer_create_periodic_task(polymcu_timer_task_func_t function, unsigned int period, void* arg) {
	polymcu_timer_task_t task = get_free_timer_task();
	if (task == NULL) {
		return NULL;
	}

	task->attributes = POLYMCU_TIMER_PERIODIC;
	task->function   = function;
	task->arg        = arg;
	task->delta      = period;

	return task;
}

polymcu_timer_task_t polymcu_timer_create_one_time_task(polymcu_timer_task_func_t function, unsigned int delay, void* arg) {
	polymcu_timer_task_t task = get_free_timer_task();
	if (task == NULL) {
		return NULL;
	}

	task->attributes = POLYMCU_TIMER_ONE_TIME;
	task->function   = function;
	task->arg        = arg;
	task->delta      = delay;

	return task;
}

void polymcu_timer_remove_task(polymcu_timer_task_t task) {
	if (!task) return;

	task->function = NULL;
}

int polymcu_timer_start_task(polymcu_timer_task_t task) {
	if (!task) return 1;

	task->attributes |= POLYMCU_TIMER_STARTED;
	task->next_tick   = polymcu_timer_get_value() + task->delta;

	// If we are the first task then resume Systick
	if (++g_timer_user == 1) {
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
	return 0;
}

int polymcu_timer_stop_task(polymcu_timer_task_t task) {
	if (!task) return 1;

	task->attributes &= ~POLYMCU_TIMER_STARTED;

	// If we were the last instance to use the timer then we should stop it
	// to save power
	if (--g_timer_user == 0) {
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	}
	return 0;
}

int polymcu_timer_task_is_scheduled(polymcu_timer_task_t task) {
	if (!task) return 0;

	return (task->attributes & POLYMCU_TIMER_STARTED);
}

void polymcu_wait(unsigned int delay) {
	unsigned int current_value, wait_until;

	// If we are the first task then resume Systick
	if (++g_timer_user == 1) {
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}

	current_value = polymcu_timer_get_value();

	// Check overflow
	if (current_value > UINT_MAX - delay) {
		// Overflow case
		wait_until = delay - (UINT_MAX - current_value);

		// Wait for the counter to be restarted at 0
		while (polymcu_timer_get_value() != 0) {
			__WFI();
		}

		// Wait for the remaining 'delay'
		while (polymcu_timer_get_value() < wait_until) {
			__WFI();
		}
	} else {
		// Non-Overflow case
		wait_until = polymcu_timer_get_value() + delay;

		while (polymcu_timer_get_value() < wait_until) {
			__WFI();
		}
	}

	// If we were the last instance to use the timer then we should stop it
	// to save power
	if (--g_timer_user == 0) {
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	}
}

unsigned int polymcu_timer_get_value(void) {
	return g_counter;
}
