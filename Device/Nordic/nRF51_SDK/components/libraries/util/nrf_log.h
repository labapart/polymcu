#ifndef NRF_LOG_H_
#define NRF_LOG_H_

#include <stdint.h>
#include <stdarg.h>
#include <app_util.h>

#define NUM_VA_ARGS(...) (sizeof((const char*[]){ 0, ##__VA_ARGS__ })/sizeof(const char*)-1)

#ifndef NRF_LOG_USES_RTT
#define NRF_LOG_USES_RTT 0
#endif

#ifndef NRF_LOG_USES_UART
#define NRF_LOG_USES_UART 0
#endif

#ifndef NRF_LOG_USES_RAW_UART
#define NRF_LOG_USES_RAW_UART 0
#endif

#if NRF_LOG_USES_RTT == 1

#define LOG_TERMINAL_NORMAL         (0)
#define LOG_TERMINAL_ERROR          (1)
#define LOG_TERMINAL_INPUT          (0)

/**@brief Function to initialize Segger Real-Time Terminal logger
 *
 * @details The Segger Real-Time Terminal stores I/O data in a ring buffer
 *          consumable by the onboard JLink debugging probe.
 *          To learn more about the Segger RTT, please visit the following web-page: 
 *          <a href="https://www.segger.com/jlink-real-time-terminal.html">Segger RTT website</a>
 *
 * @warning This function is only available when LOG_USES_RTT is defined as 1 
 *
 * @warning Do not call this function directly. Use the macro NRF_LOG_INIT
 *
 * @returns     NRF_SUCCESS     If initialization was successful
 * @returns     NRF_ERROR
 */
uint32_t log_rtt_init(void);

/**@brief Function to write printf string using Segger Real-Time Terminal logger
 *
 * @details The printf implementation is more efficient than the standard implementation
 *          To learn more about the Segger RTT, please visit the following web-page: 
 *          <a href="https://www.segger.com/jlink-real-time-terminal.html">Segger RTT website</a>
 *
 * @warning This function is only available when LOG_USES_RTT is defined as 1
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG/NRF_LOG_DEBUG/NRF_LOG_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output
 *          format_msg      Printf format string
 *          ...             Variable length arguments
 */
void log_rtt_printf(int terminal_index, char * format_msg, ...);

/**@brief Function to write string using Segger Real-Time Terminal logger
 *
 * @details The string sent in must be null-terminated but the null-termination will not be stored
 *          in the ring-buffer.
 *          The impact of running this application should be very low compared to writing to UART
 *          To learn more about the Segger RTT, please visit the following web-page: 
 *          <a href="https://www.segger.com/jlink-real-time-terminal.html">Segger RTT website</a>
 *
 * @warning This function is only available when LOG_USES_RTT is defined as 1
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_STR/NRF_LOG_DEBUG_STR/NRF_LOG_ERROR_STR    
 *
 * @param   terminal_index  Segger RTT terminal index to use as output
 * @param   num_args        Number of arguments
 * @param   msg             Null-terminated character array to use be printed
 */
void log_rtt_write_string(int terminal_index, int num_args, ...);

/**@brief Function to write integer as hex using Segger Real-Time Terminal logger
 *
 * @details Insert
 *          To learn more about the Segger RTT, please visit the following web-page: 
 *          <a href="https://www.segger.com/jlink-real-time-terminal.html">Segger RTT website</a>
 *
 * @warning This function is only available when LOG_USES_RTT is defined as 1
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_HEX/NRF_LOG_DEBUG_HEX/NRF_LOG_ERROR_HEX    
 *
 * @param   terminal_index  Segger RTT terminal index to use as output
 * @param   value           Integer value to be printed has hex
 */
void log_rtt_write_hex(int terminal_index, uint32_t value);

/**@brief Function to write character as hex using Segger Real-Time Terminal logger
 *
 * @details Insert
 *          To learn more about the Segger RTT, please visit the following web-page: 
 *          <a href="https://www.segger.com/jlink-real-time-terminal.html">Segger RTT website</a>
 *
 * @warning This function is only available when LOG_USES_RTT is defined as 1
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_HEX_CHAR/NRF_LOG_HEX_CHAR_DEBUG/NRF_LOG_HEX_CHAR_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output
 * @param   value           Character to print as hex
 */
void log_rtt_write_hex_char(int terminal_index, uint8_t value);

/**@brief Function to check if there is available data in the input-buffer
 *
 * @retval      1 if character(s) available to read
 * @retval      0 if no character(s) available to read
 */
int log_rtt_has_input(void);

/**@brief Function to read one character from the input-buffer
 *
 * @param[out]  pointer to put the character to store     
 *
 * @retval      NRF_SUCCESS if character was read out
 * @retval      NRF_ERROR_INVALID_DATA if a character could not be read
 */
uint32_t log_rtt_read_input(char* p_char);

#define NRF_LOG_INIT()                  log_rtt_init()                                                                          /*!< Initialize the nrf_log submodule*/

#define NRF_LOG_PRINTF(...)             log_rtt_printf(LOG_TERMINAL_NORMAL, ##__VA_ARGS__)                                      /*!< nrf log utilizing printf*/
#define NRF_LOG_PRINTF_DEBUG(...)       log_rtt_printf(LOG_TERMINAL_NORMAL, ##__VA_ARGS__)                                      /*!< nrf log utilzinging printf. Will only log if DEBUG is set*/
#define NRF_LOG_PRINTF_ERROR(...)       log_rtt_printf(LOG_TERMINAL_ERROR, ##__VA_ARGS__)                                       /*!< nrf log utilizing printf */
                                                                                    
#define NRF_LOG(...)                    log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< nrf log for null-terminated strings */
#define NRF_LOG_DEBUG(...)              log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< nrf log for null-terminated strings. Will only log if DEBUG is set*/
#define NRF_LOG_ERROR(...)              log_rtt_write_string(LOG_TERMINAL_ERROR, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)       /*!< nrf log for null-terminated strings.*/

#define NRF_LOG_HEX(val)                log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex)*/
#define NRF_LOG_HEX_DEBUG(val)          log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex). Will only log if DEBUG is set*/
#define NRF_LOG_HEX_ERROR(val)          log_rtt_write_hex(LOG_TERMINAL_ERROR, val)                                              /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex).*/

#define NRF_LOG_HEX_CHAR(val)           log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< nrf log characters logged as hex. Formats as AA (ex).*/
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< nrf log characters logged as hex. Formats as AA (ex).Will only log if DEBUG is set*/
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_rtt_write_hex_char(LOG_TERMINAL_ERROR, val)                                         /*!< nrf log characters logged as hex. Formats as AA (ex).*/

#define NRF_LOG_HAS_INPUT()             log_rtt_has_input()                                                                     /*!< Check if input buffer has unconsumed characters */
#define NRF_LOG_READ_INPUT(p_char)      log_rtt_read_input(p_char)                                                              /*!< Consume character from input-buffer */

#if !defined(DEBUG) && !defined(DOXYGEN)

#undef NRF_LOG_DEBUG
#define NRF_LOG_DEBUG(...)

#undef NRF_LOG_STR_DEBUG
#define NRF_LOG_STR_DEBUG(...)

#undef NRF_LOG_HEX_DEBUG
#define NRF_LOG_HEX_DEBUG(...)

#undef NRF_LOG_HEX_CHAR_DEBUG
#define NRF_LOG_HEX_CHAR_DEBUG(...)

#endif // !defined(DEBUG) && !defined(DOXYGEN)

#elif NRF_LOG_USES_UART == 1 

/**@brief Function to initialize UART logger
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1 
 *
 * @warning Do not call this function directly. Use the macro NRF_LOG_INIT
 *
 * @returns     NRF_SUCCESS     If initialization was successful
 * @returns     NRF_ERROR
 */
uint32_t log_uart_init(void);

/**@brief Function to log to uart using printf
 *
 * @details The Segger RTT has its own implementation of printf
 *          that is more efficient than the standard
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning This function is non-blocking. If too much data is sent to the UART,
 *          some characters may be skipped
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG/NRF_LOG_DEBUG/NRF_LOG_ERROR
 *
 * @params  format_msg      printf format string
 * @params  ...             variable length arguments    
 */
void log_uart_printf(const char * format_msg, ...);

/**@brief Function to log single character
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @params  c     character
 */
void log_uart_write_char(const char c);

/**@brief Function to log null-terminated strings
 *
 * @defails This function is more efficient than using printf
 *          The null-termination will not be logged
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG/NRF_LOG_DEBUG/NRF_LOG_ERROR
 *
 * @params  str     Null-terminated string
 */
void log_uart_write_string_many(int num_args, ...);


/**@brief Function to log null-terminated string
 *
 * @defails This function is more efficient than using printf
 *          The null-termination will not be logged
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG/NRF_LOG_DEBUG/NRF_LOG_ERROR
 *
 * @params  str     Null-terminated string
 */
void log_uart_write_string(const char* msg);


/**@brief Function to log integer value as string
 *
 * @details The formatted data sent on the UART will be formatted as 0x89ABCDEF (example)
 * This function is more efficient than printf
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning This function is non-blocking. If too much data is sent to the UART,
 *          some characters may be skipped
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_HEX/NRF_LOG_DEBUG_HEX/NRF_LOG_ERROR_HEX
 *
 * @param   value   Integer value to be printed as hex
 */
void log_uart_write_hex(uint32_t value);

/**@brief Function to log single character using hex formatting
 *
 * @details The formatted string sent on the UART will be formatted as AA (ex.)
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning This function is non-blocking. If too much data is sent to the UART,
 *          some characters may be skipped
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_HEX_CHAR/NRF_LOG_DEBUG_HEX_CHAR/NRF_LOG_ERROR_HEX_CHAR
 */
void log_uart_write_hex_char(uint8_t c);

/**@brief Function to check if there is available data in the input-buffer
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning Do not call this function directly. Use NRF_LOG_HAS_INPUT
 *
 * @retval      1 if character(s) available to read
 * @retval      0 if no character(s) available to read
 */
int log_uart_has_input(void);

/**@brief Function to read one character from the input-buffer
 *
 * @param[out]  pointer to put the character to store     
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning Do not call this function directly. Use NRF_LOG_HAS_INPUT
 *
 * @retval      NRF_SUCCESS if character was read out
 * @retval      NRF_ERROR_INVALID_DATA if a character could not be read
 */
uint32_t log_uart_read_input(char* p_char);


#define NRF_LOG_INIT()                  log_uart_init()                                                         /*!< Initialize the nrf_log submodule*/

#define NRF_LOG_PRINTF(...)             log_uart_printf(__VA_ARGS__)                                            /*!< nrf log utilizing printf*/
#define NRF_LOG_PRINTF_DEBUG(...)       log_uart_printf(__VA_ARGS__)                                            /*!< nrf log utilzinging printf. Will only log if DEBUG is set*/
#define NRF_LOG_PRINTF_ERROR(...)       log_uart_printf(__VA_ARGS__)                                            /*!< nrf log utilizing printf */
                                                                                    
#define NRF_LOG(...)                    log_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< nrf log for null-terminated strings */
#define NRF_LOG_DEBUG(...)              log_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< nrf log for null-terminated strings. Will only log if DEBUG is set*/
#define NRF_LOG_ERROR(...)              log_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< nrf log for null-terminated strings.*/
                                                                                    
#define NRF_LOG_HEX(val)                log_uart_write_hex(val)                                                 /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex)*/
#define NRF_LOG_HEX_DEBUG(val)          log_uart_write_hex(val)                                                 /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex). Will only log if DEBUG is set*/
#define NRF_LOG_HEX_ERROR(val)          log_uart_write_hex(val)                                                 /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex).*/

#define NRF_LOG_HEX_CHAR(val)           log_uart_write_hex_char(val)                                            /*!< nrf log characters logged as hex. Formats as AA (ex).*/
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_uart_write_hex_char(val)                                            /*!< nrf log characters logged as hex. Formats as AA (ex).Will only log if DEBUG is set*/
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_uart_write_hex_char(val)                                            /*!< nrf log characters logged as hex. Formats as AA (ex).*/

#define NRF_LOG_HAS_INPUT()             log_uart_has_input()                                                    /*!< Check if input buffer has unconsumed characters */
#define NRF_LOG_READ_INPUT(p_char)      log_uart_read_input(p_char)                                             /*!< Consume character from input-buffer */

#if !defined(DEBUG) && !defined(DOXYGEN)

#undef NRF_LOG_DEBUG
#define NRF_LOG_DEBUG(...)

#undef NRF_LOG_STR_DEBUG
#define NRF_LOG_STR_DEBUG(...)

#undef NRF_LOG_HEX_DEBUG
#define NRF_LOG_HEX_DEBUG(...)

#undef NRF_LOG_HEX_CHAR_DEBUG
#define NRF_LOG_HEX_CHAR_DEBUG(...)

#endif // !defined(DEBUG) && !defined(DOXYGEN)

#elif NRF_LOG_USES_RAW_UART

/**@brief Function to initialize raw UART logger
 *
 * @warning Do not call this function directly. Use the macro NRF_LOG_INIT
 *
 * @returns     NRF_SUCCESS     If initialization was successful
 * @returns     NRF_ERROR
 */
uint32_t log_raw_uart_init(void);

/**@brief Function to log to raw uart using printf
 *
 * @details The Segger RTT has its own implementation of printf
 *          that is more efficient than the standard
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning This function is non-blocking. If too much data is sent to the UART,
 *          some characters may be skipped
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG/NRF_LOG_DEBUG/NRF_LOG_ERROR
 *
 * @params  format_msg      printf format string
 * @params  ...             variable length arguments    
 */
void log_raw_uart_printf(const char * format_msg, ...);

/**@brief Function to log single character
 *
 * @params  c     character
 */
void log_raw_uart_write_char(const char c);

/**@brief Function to log null-terminated strings
 *
 * @defails This function is more efficient than using printf
 *          The null-termination will not be logged
 *
 * @params  str     Null-terminated string
 */
void log_raw_uart_write_string_many(int num_args, ...);


/**@brief Function to log null-terminated string
 *
 * @defails This function is more efficient than using printf
 *          The null-termination will not be logged
 *
 * @params  str     Null-terminated string
 */
void log_raw_uart_write_string(const char * str);

/**@brief Function to log integer value as string
 *
 * @details The formatted data sent on the UART will be formatted as 0x89ABCDEF (example)
 * This function is more efficient than printf
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning This function is non-blocking. If too much data is sent to the UART,
 *          some characters may be skipped
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_HEX/NRF_LOG_DEBUG_HEX/NRF_LOG_ERROR_HEX
 *
 * @param   value   Integer value to be printed as hex
 */
void log_raw_uart_write_hex(uint32_t value);

/**@brief Function to log single character using hex formatting
 *
 * @details The formatted string sent on the UART will be formatted as AA (ex.)
 *
 * @warning This function is only available when LOG_USES_UART is defined as 1
 *
 * @warning This function is non-blocking. If too much data is sent to the UART,
 *          some characters may be skipped
 *
 * @warning Do not call this function directly. Use one of the following macros:
 *          NRF_LOG_HEX_CHAR/NRF_LOG_DEBUG_HEX_CHAR/NRF_LOG_ERROR_HEX_CHAR
 */
void log_raw_uart_write_hex_char(uint8_t c);

/**@brief Function to check if there is available data in the input-buffer
 *
 * @retval      1 if character(s) available to read
 * @retval      0 if no character(s) available to read
 */
int log_raw_uart_has_input(void);

/**@brief Function to read one character from the input-buffer
 *
 * @param[out]  pointer to put the character to store     
 *
 * @retval      NRF_SUCCESS if character was read out
 * @retval      NRF_ERROR_INVALID_DATA if a character could not be read
 */
uint32_t log_raw_uart_read_input(char* p_char);

#define NRF_LOG_INIT()                  log_raw_uart_init()                                                         /*!< Initialize the nrf_log submodule*/

#define NRF_LOG_PRINTF(...)             log_raw_uart_printf(__VA_ARGS__)                                            /*!< nrf log utilizing printf*/
#define NRF_LOG_PRINTF_DEBUG(...)       log_raw_uart_printf(__VA_ARGS__)                                            /*!< nrf log utilzinging printf. Will only log if DEBUG is set*/
#define NRF_LOG_PRINTF_ERROR(...)       log_raw_uart_printf(__VA_ARGS__)                                            /*!< nrf log utilizing printf */
                                                                                    
#define NRF_LOG(...)                    log_raw_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< nrf log for null-terminated strings */
#define NRF_LOG_DEBUG(...)              log_raw_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< nrf log for null-terminated strings. Will only log if DEBUG is set*/
#define NRF_LOG_ERROR(...)              log_raw_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< nrf log for null-terminated strings.*/
                                                                                    
#define NRF_LOG_HEX(val)                log_raw_uart_write_hex(val)                                                 /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex)*/
#define NRF_LOG_HEX_DEBUG(val)          log_raw_uart_write_hex(val)                                                 /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex). Will only log if DEBUG is set*/
#define NRF_LOG_HEX_ERROR(val)          log_raw_uart_write_hex(val)                                                 /*!< nrf log for integers logged as hex. Formats as 0x89ABCDEF (ex).*/

#define NRF_LOG_HEX_CHAR(val)           log_raw_uart_write_hex_char(val)                                            /*!< nrf log characters logged as hex. Formats as AB (ex).*/
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_raw_uart_write_hex_char(val)                                            /*!< nrf log characters logged as hex. Formats as AB (ex).Will only log if DEBUG is set*/
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_raw_uart_write_hex_char(val)                                            /*!< nrf log characters logged as hex. Formats as AB (ex).*/

#define NRF_LOG_HAS_INPUT()             log_raw_uart_has_input()                                                    /*!< Check if input buffer has unconsumed characters */
#define NRF_LOG_READ_INPUT(p_char)      log_raw_uart_read_input(p_char)                                             /*!< Consume character from input-buffer */

#if !defined(DEBUG) && !defined(DOXYGEN)

#undef NRF_LOG_DEBUG
#define NRF_LOG_DEBUG(...)

#undef NRF_LOG_STR_DEBUG
#define NRF_LOG_STR_DEBUG(...)

#undef NRF_LOG_HEX_DEBUG
#define NRF_LOG_HEX_DEBUG(...)

#undef NRF_LOG_HEX_CHAR_DEBUG
#define NRF_LOG_HEX_CHAR_DEBUG(...)

#endif // !defined(DEBUG) && !defined(DOXYGEN)

#else

#include "nrf_error.h"
#include "nordic_common.h"

// Empty definitions

__INLINE int dummy_func(void* ignore)
{
    UNUSED_PARAMETER(ignore);
    return NRF_SUCCESS;
}

#define NRF_LOG_INIT()                 dummy_func(0)
#define NRF_LOG(...)
#define NRF_LOG_DEBUG(...)
#define NRF_LOG_ERROR(...)

#define NRF_LOG_PRINTF(...)      
#define NRF_LOG_PRINTF_DEBUG(...)
#define NRF_LOG_PRINTF_ERROR(...)

#define NRF_LOG_HEX(val)
#define NRF_LOG_HEX_DEBUG(val)
#define NRF_LOG_HEX_ERROR(val)

#define NRF_LOG_HEX_CHAR(val)
#define NRF_LOG_HEX_CHAR_DEBUG(val)
#define NRF_LOG_HEX_CHAR_ERROR(val)

#define NRF_LOG_HAS_INPUT()              0
#define NRF_LOG_READ_INPUT(ignore)       dummy_func(ignore) 

#endif

/**@brief Function to write hex-values
 *
 * @warning This function is by no means tread-safe. It is written for convenience
 *          If you log from different application-contexts you may get different results  
 *
 * @returns NULL by default
 */
const char* log_hex(uint32_t value);

/**@brief Function to write hex-char
 *
 * @warning This function is by no means tread-safe. It is written for convenience
 *          If you log from different application-contexts you may get different results  
 *
 * @returns NULL by default
 */
const char* log_hex_char(const char value);

#endif // NRF_LOG_H_
