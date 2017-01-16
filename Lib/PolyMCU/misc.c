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

#include "board.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "PolyMCU.h"

#ifndef NDEBUG
  #define PRINT_DEBUG(str) write(1, str, strlen(str))
#else
  #define PRINT_DEBUG(str)
#endif

__attribute__((weak, noreturn)) void polymcu_watchdog_trigger(void) {
	NVIC_SystemReset();
	// We should never reach this line...
	while(1);
}

void abort(void) {
	while(1) {
		__WFI();
	}
}

caddr_t _sbrk_r(void *reent, size_t incr) {
	extern uint8_t __HeapBase, __StackLimit;	/* Defined by the linker */
	static uint8_t* heap_end;
	uint8_t* prev_heap_end;
	if (heap_end == 0) {
		heap_end = &__HeapBase;
	}
	prev_heap_end = heap_end;
	if (heap_end + incr > &__StackLimit) {
#ifdef SUPPORT_WATCHDOG
		polymcu_watchdog_trigger();
#else
		PRINT_DEBUG("Heap and stack collision\n");
		abort();
#endif
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

void print_buffer_hex(uint8_t* ptr, size_t size) {
	for (unsigned int i = 0; i < size; i++) {
		printf("%02x", *(ptr + i));
	}
}

#ifndef NDEBUG
static void print_int(int i) {
    unsigned digit_count = 0;
    char c;
    if (i < 0) {
    	c = '-';
    	write(1, &c, 1);
        i *= -1;
    }
    // Count number of digit
    int j = i, k;
    do {
    	j = j / 10;
    	digit_count++;
    } while (j > 10);

    // Print digit
    for (; digit_count > 0; digit_count--) {
    	for (j = 1, k = 10; j < digit_count; j++) k = k * 10;
    	c = '0' + (i / k);
    	i = i % k;
    	write(1, &c, 1);
    }
    c = '0' + (i % 10);
    write(1, &c, 1);
}
#endif

#ifndef NDEBUG
static void print_hex(uint32_t i) {
	unsigned digit_count = 0;
	char c;

	write(1, "0x", 2);

	// Count number of digit
	int j = i, k;
	do {
		j = j / 0x10;
		digit_count++;
	} while (j > 0x10);

	// Print digit
	for (; digit_count > 0; digit_count--) {
		for (j = 1, k = 0x10; j < digit_count; j++) k = k * 0x10;
		j = (i / k);
		if (j < 10) c = '0' + j;
		else        c = 'a' + (j - 10);
		i = i % k;
		write(1, &c, 1);
	}
	if (i < 10) c = '0' + i;
	else        c = 'a' + (i - 10);
	write(1, &c, 1);
}
#endif

void __assert_func(const char *filename, int line, const char *function, const char *txt) {
#ifndef NDEBUG
	PRINT_DEBUG("ASSERT: '");
	if (txt) PRINT_DEBUG(txt);
	PRINT_DEBUG("' in ");
	if (filename) PRINT_DEBUG(filename);
	PRINT_DEBUG(":");
	if (function) PRINT_DEBUG(function);
	PRINT_DEBUG(":");
	print_int(line);
	PRINT_DEBUG("\n");
#endif

#ifdef SUPPORT_WATCHDOG
	polymcu_watchdog_trigger();
#else
	__BKPT(0);
	while(1);
#endif
}

#if defined (__GNUC__) &&  !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress) {
	/* These are volatile to try and prevent the compiler/linker optimising them
	away as the variables never actually get used.  If the debugger won't show the
	values of the variables, make them global my moving their declaration outside
	of this function. */
	volatile uint32_t r0;
	volatile uint32_t r1;
	volatile uint32_t r2;
	volatile uint32_t r3;
	volatile uint32_t r12;
	volatile uint32_t lr; /* Link register. */
	volatile uint32_t pc; /* Program counter. */
	volatile uint32_t psr;/* Program status register. */

	r0 = pulFaultStackAddress[ 0 ];
	r1 = pulFaultStackAddress[ 1 ];
	r2 = pulFaultStackAddress[ 2 ];
	r3 = pulFaultStackAddress[ 3 ];

	r12 = pulFaultStackAddress[ 4 ];
	lr = pulFaultStackAddress[ 5 ];
	pc = pulFaultStackAddress[ 6 ];
	psr = pulFaultStackAddress[ 7 ];

	/* When the following line is hit, the variables contain the register values. */
#ifndef NDEBUG
	PRINT_DEBUG("Exception Error: PC:");
	print_hex(pc);
	PRINT_DEBUG(" ");
	PRINT_DEBUG("LR:");
	print_hex(lr);
	PRINT_DEBUG("\r\n");
#endif

	__BKPT(0);
	for( ;; );
}
#if defined (__GNUC__) &&  !defined(__clang__)
#pragma GCC diagnostic pop
#endif

/* The prototype shows it is a naked function - in effect this is just an
assembly function. */
__attribute__((naked)) void HardFault_Handler(void) {
#ifdef SUPPORT_WATCHDOG
		polymcu_watchdog_trigger();
#else
	__asm volatile
	(
		"  mov  r0, lr                                                \n"
		"  movs r1, #4                                                \n"
		"  tst  r0, r1                                                \n"
		"  mrs  r0, msp                                               \n"
		"  beq  load_stack                                            \n"
		"  mrs  r0, psp                                               \n"
		"load_stack:                                                 \n"
		"  ldr  r1, [r0, #24]                                         \n"
		"  ldr  r2, handler2_address_const                            \n"
		"  bx   r2                                                     \n"
		"  nop                                                       \n"
		"  handler2_address_const: .word prvGetRegistersFromStack    \n"
	);
#endif
}

static uint32_t m_in_critical_section = 0;

__attribute__((weak)) void critical_section_enter(void) {
	__disable_irq();
	m_in_critical_section++;
}

__attribute__((weak)) void critical_section_exit(void) {
	m_in_critical_section--;
	if (m_in_critical_section == 0) {
		__enable_irq();
	}
}
