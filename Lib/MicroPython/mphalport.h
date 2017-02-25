#include "mphalplatformport.h"

static inline mp_uint_t mp_hal_ticks_ms(void) { return 0; }
static inline void mp_hal_set_interrupt_char(char c) {}
void mp_hal_cmsis_raise(int32_t status);
