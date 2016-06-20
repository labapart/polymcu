Debug Support
=============

`DEBUG_MASK` defines which debug message to enable. The debug level is used in
the macro `DEBUG_PRINTF(level, args...)` and `DEBUG_PUTS(level, txt)`.  
By default `DEBUG_MASK` has the value `0xF` for _Debug_ build and `0x0` for
_Release_ build.

Debug mask can be redefine in your application `Application.cmake` with the line:

        set(DEBUG_MASK 0xFFFFFFFF CACHE STRING "PolyMCU Debug Mask")

or by passing `-DDEBUG_MASK=0x...` to your CMake command line.

| Debug Macro Name        | Debug Value  |
|-------------------------|--------------|
| DEBUG_ERROR             | (1 << 0)     |
| DEBUG_WARN              | (1 << 1)     |
| DEBUG_INFO              | (1 << 2)     |
| DEBUG_VERBOSE           | (1 << 3)     |
| DEBUG_BOARD             | (1 << 4)     |
| DEBUG_RTOS              | (1 << 5)     |
| DEBUG_CMSIS             | (1 << 6)     |
| DEBUG_USB               | (1 << 7)     |
| DEBUG_APP               | (0xFF << 16) |
| DEBUG_LIB               | (0xFF << 24) |

Applications and respectively libraries can define their own debug
levels using the range from bit16 to bit23 and bit24 to bit 31.

The macro `DEBUG_NOT_IMPLEMENTED()` highlight code section that are
not implemented.
