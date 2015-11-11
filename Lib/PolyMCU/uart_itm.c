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

#ifdef ARMCM3
  #include <ARMCM3.h>
#else
  #error Device not supported
#endif

//TODO: Review me
volatile int32_t ITM_RxBuffer = ITM_RXBUFFER_EMPTY; // Initialize as EMPTY

/* Write one char "ch" to the default console */
void _ttywrch(int ch) {
	ITM_SendChar(ch);
}

/* Write "len" of char from "ptr" to file id "fd"
 * Return number of char written. */
int _write (int fd, char *ptr, int len)
{
	for (unsigned i = 0; i < len; i++) {
		ITM_SendChar(ptr[i]);
	}
	return len;
}

/* Read "len" of char to "ptr" from file id "fd"
 * Return number of char read. */
int _read (int fd, char *ptr, int len)
{
	unsigned i = 0;

	while (i < len) {
		ptr[i++] = ITM_ReceiveChar();
	}

	return len;
}
