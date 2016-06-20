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

#include <assert.h>
#include <strings.h>
#include "PolyMCU.h"

#define POLYMCU_MAIL_ENTRY_STATUS_FREE         0
#define POLYMCU_MAIL_ENTRY_STATUS_ALLOCATED	(1 << 0)
#define POLYMCU_MAIL_ENTRY_STATUS_PUSHED		(1 << 2)

void polymcu_mailbox_init(polymcu_mailbox_t *mail) {
	bzero(mail->buffer, mail->type_size * mail->count);
	bzero(mail->status, sizeof(uint8_t) * mail->count);
	bzero(mail->fifo, sizeof(uint32_t) * mail->count);
	mail->fifo_head = 0;
}

void* polymcu_mailbox_allocate(polymcu_mailbox_t *mail) {
	int i;

	critical_section_enter();
	for (i = 0; i < mail->count; i++) {
		if (mail->status[i] == POLYMCU_MAIL_ENTRY_STATUS_FREE) {
			mail->status[i] = POLYMCU_MAIL_ENTRY_STATUS_ALLOCATED;
			critical_section_exit();

			return mail->buffer + (i * mail->type_size);
		}
	}
	critical_section_exit();

	return NULL;
}

void polymcu_mailbox_free(polymcu_mailbox_t *mail, void* buffer) {
	uint32_t offset = (uint32_t)buffer - (uint32_t)mail->buffer;

	if (offset % mail->type_size != 0) {
		// The buffer must be a multiple of 'type_size'
		DEBUG_NOT_VALID();
	} else {
		assert(mail->status[offset / mail->type_size] == POLYMCU_MAIL_ENTRY_STATUS_ALLOCATED);
		mail->status[offset / mail->type_size] = POLYMCU_MAIL_ENTRY_STATUS_FREE;
	}
}

static void check_fifo(polymcu_mailbox_t *mail, void* buffer) {
	for (int i = 0; i < mail->count; i++) {
		assert(mail->fifo[i] != buffer);
	}
}

int polymcu_mailbox_put(polymcu_mailbox_t *mail, void* buffer) {
	uint32_t offset = (uint32_t)buffer - (uint32_t)mail->buffer;
	uint32_t index, fifo_index;
	uint32_t found;

	check_fifo(mail, buffer);
	assert(buffer != NULL);
	assert(((uint32_t)buffer >= (uint32_t)mail->buffer) &&
			((uint32_t)buffer < (uint32_t)(mail->buffer + (mail->type_size * mail->count))));

	if (offset % mail->type_size != 0) {
		// The buffer must be a multiple of 'type_size'
		DEBUG_NOT_VALID();
		return -1;
	} else {
		index = offset / mail->type_size;

		assert(mail->status[index] == POLYMCU_MAIL_ENTRY_STATUS_ALLOCATED);

		critical_section_enter();
		// Add to the FIFO
		if (mail->fifo[mail->fifo_head] == NULL) {
			// Case the FIFO is empty
			mail->fifo[mail->fifo_head] = buffer;
			found = 1;
		} else {
			//
			// Find the next free entry
			//
			found = 0;

			// Look for the entries [fifo_head + 1; count - 1]
			for (fifo_index = mail->fifo_head + 1; fifo_index < mail->count; fifo_index++) {
				if (mail->fifo[fifo_index] == NULL) {
					mail->fifo[fifo_index] = buffer;
					found = 1;
					break;
				}
			}

			// If an entry has not been found look between [0; fifo_head - 1]
			if (found == 0) {
				for (fifo_index = 0; fifo_index < mail->fifo_head; fifo_index++) {
					if (mail->fifo[fifo_index] == NULL) {
						mail->fifo[fifo_index] = buffer;
						found = 1;
						break;
					}
				}
			}
		}
		if (found) {
			// Mark as pushed
			mail->status[index] = POLYMCU_MAIL_ENTRY_STATUS_PUSHED;
			critical_section_exit();
			return 0;
		} else {
			DEBUG_NOT_VALID();
			critical_section_exit();
			return -2;
		}
	}
}

int polymcu_mailbox_insert_first(polymcu_mailbox_t *mail, void* buffer) {
	uint32_t offset = (uint32_t)buffer - (uint32_t)mail->buffer;
	uint32_t index, fifo_index;

	check_fifo(mail, buffer);
	assert(buffer != NULL);
	assert(((uint32_t)buffer >= (uint32_t)mail->buffer) &&
			((uint32_t)buffer < (uint32_t)(mail->buffer + (mail->type_size * mail->count))));

	if (offset % mail->type_size != 0) {
		// The buffer must be a multiple of 'type_size'
		DEBUG_NOT_VALID();
		return -1;
	} else {
		index = offset / mail->type_size;

		assert(mail->status[index] == POLYMCU_MAIL_ENTRY_STATUS_ALLOCATED);

		critical_section_enter();
		// Add to the FIFO
		if (mail->fifo[mail->fifo_head] == NULL) {
			// Case the FIFO is empty
			mail->fifo[mail->fifo_head] = buffer;
		} else {
			if (mail->fifo_head == 0) {
				fifo_index = mail->count - 1;
			} else {
				fifo_index = mail->fifo_head - 1;
			}

			assert(mail->fifo[fifo_index] == NULL);
			mail->fifo[fifo_index] = buffer;
			mail->fifo_head = fifo_index;
		}

		// Mark as pushed
		mail->status[index] = POLYMCU_MAIL_ENTRY_STATUS_PUSHED;
		critical_section_exit();
		return 0;
	}
}

void* polymcu_mailbox_get(polymcu_mailbox_t *mail) {
	void* buffer = mail->fifo[mail->fifo_head];
	uint32_t offset, index;

	if (buffer == NULL) {
		// Case the FIFO is empty
		return NULL;
	} else {
		offset = (uint32_t)buffer - (uint32_t)mail->buffer;
		index = offset / mail->type_size;

		critical_section_enter();
		// Free the FIFO
		mail->fifo[mail->fifo_head] = NULL;

		// Update the fifo_index
		mail->fifo_head++;
		if (mail->fifo_head == mail->count) {
			mail->fifo_head = 0; // Start again at the beginning
		}

		assert(mail->status[index] == POLYMCU_MAIL_ENTRY_STATUS_PUSHED);
		// Mark the buffer as allocated
		mail->status[index] = POLYMCU_MAIL_ENTRY_STATUS_ALLOCATED;

		critical_section_exit();
		return buffer;
	}
}

uint32_t polymcu_mailbox_length(polymcu_mailbox_t *mail) {
	if (mail->fifo[mail->fifo_head] == NULL) {
		return 0;
	} else {
		uint32_t fifo_index, length = 1;

		critical_section_enter();
		// Look for the entries [fifo_head + 1; count - 1]
		for (fifo_index = mail->fifo_head + 1; fifo_index < mail->count; fifo_index++) {
			if (mail->fifo[fifo_index] == NULL) {
				critical_section_exit();
				return length;
			} else {
				length++;
			}
		}

		// Look between [0; fifo_head - 1]
		for (fifo_index = 0; fifo_index < mail->fifo_head; fifo_index++) {
			if (mail->fifo[fifo_index] == NULL) {
				critical_section_exit();
				return length;
			} else {
				length++;
			}
		}
		critical_section_exit();
		return length;
	}
}
