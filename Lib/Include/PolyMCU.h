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
	  type entries[count]; \
  	  uint8_t status[count]; \
	  uint32_t fifo[count]; \
  } polymcu_mailbox_##name; \
  polymcu_mailbox_t polymcu_mailbox_##name##_def = { sizeof(type), count, (uint8_t*)&polymcu_mailbox_##name, \
	  	  (uint8_t*)&polymcu_mailbox_##name + (sizeof(type) * count), \
		  (uint32_t**)((uint8_t*)&polymcu_mailbox_##name + ((sizeof(type) + sizeof(uint8_t)) * count))};

#define HOST_MAILBOX_DECLARE_EXTERN(name)	extern polymcu_mailbox_t polymcu_mailbox_##name##_def
#define HOST_MAILBOX_NAME(name)				&polymcu_mailbox_##name##_def

void polymcu_mailbox_init(polymcu_mailbox_t *mail);
void* polymcu_mailbox_allocate(polymcu_mailbox_t *mail);
void polymcu_mailbox_free(polymcu_mailbox_t *mail, void* buffer);
int polymcu_mailbox_put(polymcu_mailbox_t *mail, void* buffer);
void* polymcu_mailbox_get(polymcu_mailbox_t *mail);
int polymcu_mailbox_insert_first(polymcu_mailbox_t *mail, void* buffer);
uint32_t polymcu_mailbox_length(polymcu_mailbox_t *mail);

void critical_section_enter(void);
void critical_section_exit(void);

typedef int status_t;

void print_hex(uint8_t* ptr, unsigned size);

#ifdef DEBUG
  #define DEBUG_PRINTF(args...)	printf(args)
  #define DEBUG_PUTS(txt)		puts(txt)

  #define DEBUG_NOT_IMPLEMENTED() assert(0)
#else
  #define DEBUG_PRINTF(args...)
  #define DEBUG_PUTS(txt)

  #define DEBUG_NOT_IMPLEMENTED() assert(0)
#endif

#endif
