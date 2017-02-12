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

#ifndef __POLYMCU_H__
#define __POLYMCU_H__

#include <assert.h>
#include <stdio.h>
#include "board.h"

//
// Mailbox support
//
typedef struct {
	uint32_t type_size;
	uint32_t count;
	uint8_t* buffer;
	uint8_t* status;
	uint32_t** fifo;
	uint32_t fifo_head; // FIFO Head Index
} polymcu_mailbox_t;

#define POLYMCU_MAILBOX_DEFINE(name, type, count) \
  struct { \
	  uint32_t fifo[count]; \
	  type entries[count]; \
  	  uint8_t status[count]; \
  } polymcu_mailbox_##name; \
  polymcu_mailbox_t polymcu_mailbox_##name##_def = { sizeof(type), count, \
		  (uint8_t*)&polymcu_mailbox_##name + (sizeof(uint32_t) * count), \
		  (uint8_t*)&polymcu_mailbox_##name + ((sizeof(uint32_t) + sizeof(type)) * count), \
		  (uint32_t**)((uint8_t*)&polymcu_mailbox_##name), \
		  0};

#define POLYMCU_MAILBOX_DECLARE_EXTERN(name)	extern polymcu_mailbox_t polymcu_mailbox_##name##_def
#define POLYMCU_MAILBOX_NAME(name)				&polymcu_mailbox_##name##_def

void polymcu_mailbox_init(polymcu_mailbox_t *mail);
void* polymcu_mailbox_allocate(polymcu_mailbox_t *mail);
void polymcu_mailbox_free(polymcu_mailbox_t *mail, void* buffer);
int polymcu_mailbox_put(polymcu_mailbox_t *mail, void* buffer);
void* polymcu_mailbox_get(polymcu_mailbox_t *mail);
int polymcu_mailbox_insert_first(polymcu_mailbox_t *mail, void* buffer);
uint32_t polymcu_mailbox_length(polymcu_mailbox_t *mail);

void critical_section_enter(void);
void critical_section_exit(void);

#ifdef SUPPORT_TIMER

#define TIMER_PERIOD_MILLISECOND    1000
#define TIMER_PERIOD_SECOND         1

typedef struct polymcu_timer_task* polymcu_timer_task_t;
typedef void (*polymcu_timer_task_func_t)(void* arg);

/*
 * Initialize PolyMCU timer to tick every `period`.
 * The period is defined as how many `period` are in one second.
 * For instance:
 *   - `period` = 1 means that PolyMCU timer tick would be every second
 *   - `period` = 1000 means that PolyMCU timer tick would be every millisecond
 */
int polymcu_timer_init(unsigned int period);

/*
 * Return the `period` used at initialization when calling `polymcu_timer_init()`.
 */
unsigned int polymcu_timer_get_period(void);

polymcu_timer_task_t polymcu_timer_create_periodic_task(polymcu_timer_task_func_t function, unsigned int period, void* arg);
polymcu_timer_task_t polymcu_timer_create_one_time_task(polymcu_timer_task_func_t function, unsigned int delay, void* arg);
void polymcu_timer_remove_task(polymcu_timer_task_t task);
int polymcu_timer_start_task(polymcu_timer_task_t task);
int polymcu_timer_stop_task(polymcu_timer_task_t task);
int polymcu_timer_task_is_scheduled(polymcu_timer_task_t task);

/*
 * Wait number of `period`. `period` is defined when calling `polymcu_timer_init()`
 */
void polymcu_wait(unsigned int delay);
unsigned int polymcu_timer_get_value(void);

/*
 * Support for boards who do not use systick support for PolyMCU Timer API
 */

// This function must be called by the timer IRQ handler
void polymcu_timer_irq_handler(void);

// Board specific function to start HW timer
void polymcu_timer_hw_start(void);
// Board specific function to stop HW timer
void polymcu_timer_hw_stop(void);

#endif

#ifdef SUPPORT_WATCHDOG
void polymcu_watchdog_init(void);
void polymcu_watchdog_alive(void);
void polymcu_watchdog_set_timeout(uint32_t timeout_ms);
void polymcu_watchdog_restore_default_timeout(void);
void polymcu_watchdog_trigger(void);
#endif

void print_buffer_hex(uint8_t* ptr, size_t size);

//
// PolyMCU Debug Support
//
#define DEBUG_ERROR       (1 << 0)
#define DEBUG_WARN        (1 << 1)
#define DEBUG_INFO        (1 << 2)
#define DEBUG_VERBOSE     (1 << 3)

#define DEBUG_BOARD       (1 << 4)
#define DEBUG_RTOS        (1 << 5)
#define DEBUG_CMSIS       (1 << 6)
#define DEBUG_USB         (1 << 7)
#define DEBUG_APP         (0xFF << 16)
#define DEBUG_LIB         (0xFF << 24)

// Define the levels of debug messages applications and libraries can declare
// their own debug level
#define DEBUG_APP_LEVEL   16
#define DEBUG_LIB_LEVEL   24

#if DEBUG_MASK > 0
  #define DEBUG_PRINTF(level, args...)	if (level & DEBUG_MASK) printf(args)
  #define DEBUG_PUTS(level, txt)		if (level & DEBUG_MASK) puts(txt)

  #define DEBUG_NOT_IMPLEMENTED() assert(0)
  #define DEBUG_NOT_SUPPORTED() assert(0)
  #define DEBUG_NOT_VALID() assert(0)
#else
  #define DEBUG_PRINTF(level, args...)
  #define DEBUG_PUTS(level, txt)

  #define DEBUG_NOT_IMPLEMENTED()
  #define DEBUG_NOT_SUPPORTED()
  #define DEBUG_NOT_VALID()
#endif

#endif
