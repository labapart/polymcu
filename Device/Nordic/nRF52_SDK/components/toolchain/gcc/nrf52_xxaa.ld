/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x80000
  /* Note from Lab A Part: nRF52 Preview DK RAM is only 0x8000 byte at 0x20000000. */
  /*      The initial line was: RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x10000 */
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x8000
}


INCLUDE "nrf52_common.ld"
